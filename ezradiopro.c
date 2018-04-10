#include "si4432.h"
#include "AT89X52.H"
#include "delay.h"
#include "ezradiopro.h"
//-----------------------------------------------------------------------------
#define RR		0x00
#define WR		0x80

//-----------------------------------------------------------------------------
bit RxPacketReceived;
unsigned char data  RxPacketLength;
///unsigned char xdata  RxErrors;
unsigned char xdata  RxIntBuffer[64];
//-----------------------------------------------------------------------------
SI4432_STATUS InitSoftwareReset (void);
//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
unsigned char SPI_Read(void);
void SPI_Write(unsigned char txdata);
U8    RxIntPhyRead (U8);
void  RxIntPhyWrite (U8, U8);
void  RxIntphyReadFIFO (U8, VARIABLE_SEGMENT_POINTER(buffer, U8, BUFFER_MSPACE));
//=============================================================================
//
// API Functions
//
//=============================================================================
//-----------------------------------------------------------------------------
// Function Name
//    ppPhyInit()
//
// Parameters   : none
// Return Value : status
//
// After a pin reset or other reset the state of the Radio is unknown.
// The MCU will wait for full POR duration, then figure out if the
// Radio needs a software reset.
//
//-----------------------------------------------------------------------------
SI4432_STATUS ppPhyInit(void)
{
   U8 volatile status;
   unsigned int idata cnt_num=0;
   bit flag_ready=0;
   si4432_SDN = 0;
   delay_ms(100);

   status = phyRead(EZRADIOPRO_DEVICE_VERSION); // check version
   if(status == 0xFF)
      return SI4432_STATUS_ERROR_SPI;
   else if (status == 0x00)
      return  SI4432_STATUS_ERROR_SPI;
   else if (status < MIN_RADIO_VERSION)
      return SI4432_STATUS_ERROR_UNSUPPORTED_RADIO;

   status = phyRead(EZRADIOPRO_INTERRUPT_STATUS_2);

   if((status & EZRADIOPRO_IPOR)==0)
   {
      // radio needs a software reset
      return InitSoftwareReset();
   }
   else if((status & EZRADIOPRO_ICHIPRDY)==0)
   {
      // enable Chip read only
      phyWrite(EZRADIOPRO_INTERRUPT_ENABLE_1, 0);
      phyWrite(EZRADIOPRO_INTERRUPT_ENABLE_2, EZRADIOPRO_ENCHIPRDY);

	  for(cnt_num=0;cnt_num<5000;cnt_num++)
	  {
	    if(si4432_NIRQ==0)
        {
		  flag_ready=1;
          break;
        }
	  }
	  if(flag_ready)
	  {
	    flag_ready=0;
	  }
	  else
	  {
	    return SI4432_STATUS_ERROR_RADIO_XTAL;
	  }
   }
   return SI4432_STATUS_SUCCESS; // success
}
//-----------------------------------------------------------------------------
// Function Name
//    InitSoftwareReset()
//
// Parameters   : none
// Return Value : status
//
// This function uses a software reset to reset the radio. This can be used
// to reset the radio when a radio POR has not occured.
// A T0 interrupt timeout is used to minimize start-up time.
//-----------------------------------------------------------------------------
U8 InitSoftwareReset(void)
{
   U8 volatile status;
   unsigned int xdata cnt_num=0;
   bit flag_ready=0;
   // Clear interrupt enable and interrupt flag bits
   phyWrite(EZRADIOPRO_INTERRUPT_ENABLE_1, 0);
   phyWrite(EZRADIOPRO_INTERRUPT_ENABLE_2, 0);
   status = phyRead(EZRADIOPRO_INTERRUPT_STATUS_1);
   status = phyRead(EZRADIOPRO_INTERRUPT_STATUS_2);

   // SWReset
   phyWrite(EZRADIOPRO_OPERATING_AND_FUNCTION_CONTROL_1, (EZRADIOPRO_SWRES|EZRADIOPRO_XTON));
   status = phyRead(EZRADIOPRO_OPERATING_AND_FUNCTION_CONTROL_1); //test
   // wait on IRQ with 2 MS timeout
   for(cnt_num=0;cnt_num<5000;cnt_num++)
   {
     if(si4432_NIRQ==0)
     {
	   flag_ready=1;
       break;
     }
   }
   if(flag_ready)
   {
     flag_ready=0;
   }
   else
   {
     return SI4432_STATUS_ERROR_NO_IRQ;
   }
   status = phyRead(EZRADIOPRO_INTERRUPT_STATUS_2);

   if((status & EZRADIOPRO_ICHIPRDY)==0)
   {
      // enable Chip read only
      phyWrite(EZRADIOPRO_INTERRUPT_ENABLE_1, 0);
      phyWrite(EZRADIOPRO_INTERRUPT_ENABLE_2, EZRADIOPRO_ENCHIPRDY);

      // wait on IRQ with 2 MS timeout
	   for(cnt_num=0;cnt_num<5000;cnt_num++)
	  {
	    if(si4432_NIRQ==0)
        {
		  flag_ready=1;
          break;
        }
	  }
	   if(flag_ready)
	  {
	    flag_ready=0;
	  }
	  else
	  {
	    return SI4432_STATUS_ERROR_RADIO_XTAL;
	  }
   }
   return SI4432_STATUS_SUCCESS; // success
}
//-----------------------------------------------------------------------------
// Function Name
//
// Return Value : None
// Parameters   :
//
//-----------------------------------------------------------------------------
SI4432_STATUS ppPhyIdle(void)
{
   U8 volatile status;
   unsigned int xdata cnt_num=0;
   bit flag_ready=0;

   status = phyRead(EZRADIOPRO_INTERRUPT_STATUS_2);

   if((status & EZRADIOPRO_ICHIPRDY)==EZRADIOPRO_ICHIPRDY)
      return SI4432_STATUS_SUCCESS;
   else
   {
      // enable just the chip ready IRQ
      phyWrite(EZRADIOPRO_INTERRUPT_ENABLE_1, 0x00);
      phyWrite(EZRADIOPRO_INTERRUPT_ENABLE_2, EZRADIOPRO_ENCHIPRDY);

      // read Si4432 interrupts to clear
      status = phyRead(EZRADIOPRO_INTERRUPT_STATUS_1);
      status = phyRead(EZRADIOPRO_INTERRUPT_STATUS_2);

      // enable XTON
      phyWrite (EZRADIOPRO_OPERATING_AND_FUNCTION_CONTROL_1, EZRADIOPRO_XTON);
      // wait on IRQ with 2 MS timeout
	   for(cnt_num=0;cnt_num<5000;cnt_num++)
	  {
	    if(si4432_NIRQ==0)
        {
		  flag_ready=1;
          break;
        }
	  }
	   if(flag_ready)
	  {
	    flag_ready=0;
	  }
	  else
	  {
	    return SI4432_STATUS_ERROR_RADIO_XTAL;
	  }

      return SI4432_STATUS_SUCCESS;
   }
}
//-----------------------------------------------------------------------------
// Function Name
//
// Return Value : None
// Parameters   :
//
//-----------------------------------------------------------------------------
SI4432_STATUS ppPhyStandby (void)
{
   U8 volatile status;
   unsigned int xdata cnt_num=0;
   bit flag_ready=0;

   // disable interrupts
   phyWrite(EZRADIOPRO_INTERRUPT_ENABLE_1, 0x00);
   phyWrite(EZRADIOPRO_INTERRUPT_ENABLE_2, 0x00);

   // read Si4432 interrupts to clear
   status = phyRead(EZRADIOPRO_INTERRUPT_STATUS_1);
   status = phyRead(EZRADIOPRO_INTERRUPT_STATUS_2);

   // stop XTAL
   phyWrite (EZRADIOPRO_OPERATING_AND_FUNCTION_CONTROL_1, 0);

   return SI4432_STATUS_SUCCESS; // success
}
//-----------------------------------------------------------------------------
// Function Name
//
// Return Value : None
// Parameters   :
//
//-----------------------------------------------------------------------------
SI4432_STATUS ppPhyShutDown (void)
{
   si4432_SDN = 1;

   return SI4432_STATUS_SUCCESS; // success
}
//-----------------------------------------------------------------------------
// Function Name
//
// Return Value : None
// Parameters   :
//
//-----------------------------------------------------------------------------
SI4432_STATUS ppPhyReStart(void)
{
   U8 volatile status;
   unsigned int xdata cnt_num=0;
   bit flag_ready=0;

   si4432_SDN = 0;

   delay_ms(2);
   // wait on IRQ with 25 MS timeout
   
   for(cnt_num=0;cnt_num<10000;cnt_num++)
  {
    if(si4432_NIRQ==0)
    {
	  flag_ready=1;
      break;
    }
  }
   if(flag_ready)
  {
    flag_ready=0;
  }
  else
  {
    return SI4432_STATUS_ERROR_RADIO_XTAL;
  }

   status = phyRead(EZRADIOPRO_INTERRUPT_STATUS_2);

   if((status & EZRADIOPRO_IPOR)==0)
   {
      // radio needs a software reset
      return InitSoftwareReset();
   }
   else if((status & EZRADIOPRO_ICHIPRDY)==0)
   {
      // disable POR interrupt
      phyWrite(EZRADIOPRO_INTERRUPT_ENABLE_2, EZRADIOPRO_ENCHIPRDY);

      // wait on IRQ with 2 MS timeout
	  
      // wait on IRQ with 2 MS timeout
	   for(cnt_num=0;cnt_num<5000;cnt_num++)
	  {
	    if(si4432_NIRQ==0)
        {
		  flag_ready=1;
          break;
        }
	  }
	   if(flag_ready)
	  {
	    flag_ready=0;
	  }
	  else
	  {
	    return SI4432_STATUS_ERROR_RADIO_XTAL;
	  }
   }
   return SI4432_STATUS_SUCCESS; // success
}
//-----------------------------------------------------------------------------
// Function Name
//
// Return Value : None
// Parameters   :
//
//-----------------------------------------------------------------------------
/*SI4432_STATUS ppPhyInitRadio(void)
{
   U8 volatile status;

   // disable interrupts
   phyWrite(EZRADIOPRO_INTERRUPT_ENABLE_1, 0x00); 
   phyWrite(EZRADIOPRO_INTERRUPT_ENABLE_2, 0x00);

   // read Si4432 interrupts to clear
   status = phyRead(EZRADIOPRO_INTERRUPT_STATUS_1);                 //不使能中断，但状态寄存器的值可以变化
   status = phyRead(EZRADIOPRO_INTERRUPT_STATUS_2);

#ifdef ENABLE_RF_SWITCH                                             //设置RF PA开关
   phyWrite(EZRADIOPRO_GPIO2_CONFIGURATION, 0x14);
   phyWrite(EZRADIOPRO_GPIO0_CONFIGURATION, 0x12);	                //配置射频开关
   phyWrite(EZRADIOPRO_GPIO1_CONFIGURATION, 0x15);
#endif
   //设置负载电容                                                  
   phyWrite(EZRADIOPRO_CRYSTAL_OSCILLATOR_LOAD_CAPACITANCE, EZRADIOPRO_OSC_CAP_VALUE);	   // 模块使用12pbm


   //set the center frequency to 433 MHz
   phyWrite(EZRADIOPRO_FREQUENCY_BAND_SELECT, CALC_FREQUENCY_BAND_SELECT(TRX_FREQUENCY));                // 0x75
   phyWrite(EZRADIOPRO_NOMINAL_CARRIER_FREQUENCY_1,CALC_NOMINAL_CARRIER_FREQUENCY_1(TRX_FREQUENCY));     // 0x76
   phyWrite(EZRADIOPRO_NOMINAL_CARRIER_FREQUENCY_0,  CALC_NOMINAL_CARRIER_FREQUENCY_0(TRX_FREQUENCY));   // 0x77


   //set the desired TX data rate (9.6kbps)
   phyWrite(EZRADIOPRO_TX_DATA_RATE_1, CALC_TX_DATA_RATE_1(TRX_DATA_RATE));  	// 0x6E  
   phyWrite(EZRADIOPRO_TX_DATA_RATE_0, CALC_TX_DATA_RATE_0(TRX_DATA_RATE));     // 0x6F
   phyWrite(EZRADIOPRO_MODULATION_MODE_CONTROL_1, 0x2c);                        // 0X70


   //set the desired TX deviatioin (+-45 kHz)
   phyWrite(EZRADIOPRO_FREQUENCY_DEVIATION, CALC_TX_DEVIATION(TRX_DEVIATION));	// 0x72 
   phyWrite(EZRADIOPRO_MODULATION_MODE_CONTROL_2, 0x22); //0X71  编码和 FIFO模式，FSK
   
   //************************跳频 
   phyWrite(EZRADIOPRO_FREQUENCY_HOPPING_STEP_SIZE, CALC_FREQUENCY_HOPPING_STEP_SIZE(TRX_CHANNEL_SPACING));	// 0x7A
   phyWrite(EZRADIOPRO_FREQUENCY_HOPPING_CHANNEL_SELECT,0);

   //set the TX power to MAX 
   phyWrite(EZRADIOPRO_TX_POWER,DEFAULT_TX_POWER);	      // 0x6D  

   // 相关PH的设置
   phyWrite(EZRADIOPRO_DATA_ACCESS_CONTROL, 0x8D);    //  0x30 - SI4432_DATA_ACCESS_CONTROL - enable TX & RX packet handler, enable CRC
   phyWrite(EZRADIOPRO_HEADER_CONTROL_1, 0x00);	      //  0x32 - SI4432_HEADER_CONTROL_1 - no header
   phyWrite(EZRADIOPRO_HEADER_CONTROL_2, 0x02);	      //  0x33 - SI4432_HEADER_CONTROL_2 - 2 byte sync word, variable packet length
   phyWrite(EZRADIOPRO_PREAMBLE_LENGTH, 0x0A);	     //  0x34 - SI4432_PREAMBLE_LENGTH - 10 nibbles, 40 bits  5bytes
   phyWrite(EZRADIOPRO_PREAMBLE_DETECTION_CONTROL, 0x28); //  0x35 - SI4432_PREAMBLE_DETECTION_CONTROL -  5 nibbles, 20 bits

 //  phyWrite(EZRADIOPRO_IF_FILTER_BANDWIDTH, IF_FILTER_BANDWIDTH);  // 0x1C       // 中频率滤波器带宽  
   phyWrite(EZRADIOPRO_IF_FILTER_BANDWIDTH,0x1e);  // 0x1C       // 中频率滤波器带宽  
   phyWrite(EZRADIOPRO_AFC_LOOP_GEARSHIFT_OVERRIDE, DEFAULT_AFC_LOOP_GEARSHIFT_OVERRIDE);   // 0x1D       //使能AFC频率校正
   phyWrite(EZRADIOPRO_CLOCK_RECOVERY_OVERSAMPLING_RATIO, CALC_CLOCK_RECOVERY_OVERSAMPLING_RATIO(RX_DWN3_BYP,RX_NDEC_EXP,TRX_DATA_RATE)); // 0x20  .  //时钟恢复过抽样率
   phyWrite(EZRADIOPRO_CLOCK_RECOVERY_OFFSET_2,  CALC_CLOCK_RECOVERY_OFFSET_2(RX_DWN3_BYP,RX_NDEC_EXP,TRX_DATA_RATE));                   // 0x21 
   phyWrite(EZRADIOPRO_CLOCK_RECOVERY_OFFSET_1, CALC_CLOCK_RECOVERY_OFFSET_1(RX_DWN3_BYP,RX_NDEC_EXP,TRX_DATA_RATE));                    // 0x22
   phyWrite(EZRADIOPRO_CLOCK_RECOVERY_OFFSET_0, CALC_CLOCK_RECOVERY_OFFSET_0(RX_DWN3_BYP,RX_NDEC_EXP,TRX_DATA_RATE));                    // 0x23
   phyWrite(EZRADIOPRO_CLOCK_RECOVERY_TIMING_LOOP_GAIN_1, CALC_CLOCK_RECOVERY_TIMING_LOOP_GAIN_1(TRX_DATA_RATE,RX_DWN3_BYP,RX_NDEC_EXP,TRX_DEVIATION)); // 0x24
   phyWrite(EZRADIOPRO_CLOCK_RECOVERY_TIMING_LOOP_GAIN_0,CALC_CLOCK_RECOVERY_TIMING_LOOP_GAIN_0(TRX_DATA_RATE,RX_DWN3_BYP,RX_NDEC_EXP,TRX_DEVIATION));  // 0x25
 //  phyWrite(EZRADIOPRO_AFC_LIMITER,AFC_LIMIT );	            // 0x2A
   phyWrite(EZRADIOPRO_AFC_LIMITER,0x20);	            // 0x2A
   // errata for B1 radio
   phyWrite(0x69, 0x60);  //

   return SI4432_STATUS_SUCCESS;
}*/
SI4432_STATUS ppPhyInitRadio (void)
{
   U8 volatile status;

   // disable interrupts
   phyWrite(EZRADIOPRO_INTERRUPT_ENABLE_1, 0x00); 
   phyWrite(EZRADIOPRO_INTERRUPT_ENABLE_2, 0x00);

   // read Si4432 interrupts to clear
   status = phyRead(EZRADIOPRO_INTERRUPT_STATUS_1);                 //不使能中断，但状态寄存器的值可以变化
   status = phyRead(EZRADIOPRO_INTERRUPT_STATUS_2);

#ifdef ENABLE_RF_SWITCH                                             //设置RF PA开关
   phyWrite(EZRADIOPRO_GPIO0_CONFIGURATION, 0x14);
   phyWrite(EZRADIOPRO_GPIO1_CONFIGURATION, 0x12);	                //配置射频开关
   phyWrite(EZRADIOPRO_GPIO2_CONFIGURATION, 0x15);
#endif
   //设置负载电容                                                  
 //  phyWrite(EZRADIOPRO_CRYSTAL_OSCILLATOR_LOAD_CAPACITANCE, EZRADIOPRO_OSC_CAP_VALUE);	   // 模块使用12pbm
	phyWrite(0x09,0xda);
   
   //set the center frequency to 470 MHz
   phyWrite(EZRADIOPRO_FREQUENCY_BAND_SELECT, 0x57);          // 0x75
   phyWrite(EZRADIOPRO_NOMINAL_CARRIER_FREQUENCY_1,0x00);     // 0x76
   phyWrite(EZRADIOPRO_NOMINAL_CARRIER_FREQUENCY_0, 0x00);   // 0x77

   //set the desired TX data rate (1.2kbps)
   phyWrite(EZRADIOPRO_TX_DATA_RATE_1,0x09);  	// 0x6E  
   phyWrite(EZRADIOPRO_TX_DATA_RATE_0,0xd5);     // 0x6F
   phyWrite(EZRADIOPRO_MODULATION_MODE_CONTROL_1, 0x2c);  //70                    
   phyWrite(EZRADIOPRO_CHARGEPUMP_CURRENT_TRIMMING_OVERRIDE, 0x80);   // 0X58 
   //set the desired TX deviatioin (+-30 kHz)
   phyWrite(EZRADIOPRO_FREQUENCY_DEVIATION,0x30);	// 0x72 
   phyWrite(EZRADIOPRO_MODULATION_MODE_CONTROL_2, 0x22); //0X71  编码和 FIFO模式，FSK
   
   //************************跳频 
   phyWrite(EZRADIOPRO_FREQUENCY_HOPPING_STEP_SIZE,0x00);	// 0x7A
   phyWrite(EZRADIOPRO_FREQUENCY_HOPPING_CHANNEL_SELECT,0);

   //set the TX power to MAX 
   phyWrite(EZRADIOPRO_TX_POWER,DEFAULT_TX_POWER);	      // 0x6D  

   // 相关PH的设置
   phyWrite(EZRADIOPRO_DATA_ACCESS_CONTROL, 0x8D);    //  0x30 - SI4432_DATA_ACCESS_CONTROL - enable TX & RX packet handler, enable CRC
   phyWrite(EZRADIOPRO_HEADER_CONTROL_1, 0x00);	      //  0x32 - SI4432_HEADER_CONTROL_1 - no header
   phyWrite(EZRADIOPRO_HEADER_CONTROL_2, 0x02);	      //  0x33 - SI4432_HEADER_CONTROL_2 - 2 byte sync word, variable packet length
   phyWrite(EZRADIOPRO_PREAMBLE_LENGTH, 0x0A);	     //  0x34 - SI4432_PREAMBLE_LENGTH - 10 nibbles, 40 bits  5bytes
   phyWrite(EZRADIOPRO_PREAMBLE_DETECTION_CONTROL, 0x28); //  0x35 - SI4432_PREAMBLE_DETECTION_CONTROL -  5 nibbles, 20 bits


   phyWrite(EZRADIOPRO_IF_FILTER_BANDWIDTH,0x16);  // 0x1C       // 中频率滤波器带宽  
   phyWrite(EZRADIOPRO_AFC_LOOP_GEARSHIFT_OVERRIDE,0X40);   // 0x1D       //使能AFC频率校正
   phyWrite(EZRADIOPRO_CLOCK_RECOVERY_OVERSAMPLING_RATIO,0X83 ); // 0x20  .  //时钟恢复过抽样率
   phyWrite(EZRADIOPRO_CLOCK_RECOVERY_OFFSET_2,0Xc0);                   // 0x21 
   phyWrite(EZRADIOPRO_CLOCK_RECOVERY_OFFSET_1,0X13);                    // 0x22
   phyWrite(EZRADIOPRO_CLOCK_RECOVERY_OFFSET_0,0Xa9);                    // 0x23
   phyWrite(EZRADIOPRO_CLOCK_RECOVERY_TIMING_LOOP_GAIN_1,0X00); // 0x24
   phyWrite(EZRADIOPRO_CLOCK_RECOVERY_TIMING_LOOP_GAIN_0,0X04);  // 0x25
   phyWrite(EZRADIOPRO_AFC_TIMING_CONTROL,0X0A);	            // 0x1E
   phyWrite(EZRADIOPRO_CLOCK_RECOVERY_GEARSHIFT_OVERRIDE,0x03);	            // 0x1F
   phyWrite(EZRADIOPRO_AFC_LIMITER,0x14);	                     // 0x2A
   // errata for B1 radio
   phyWrite(0x69, 0x60);  //

   return SI4432_STATUS_SUCCESS;
}
//-----------------------------------------------------------------------------
// Function Name
//
// Return Value : None
// Parameters   :
//
//-----------------------------------------------------------------------------
SI4432_STATUS ppPhyTx (U8 length, VARIABLE_SEGMENT_POINTER(txBuffer, U8, BUFFER_MSPACE))
{
   U8 status;

   phyWrite(EZRADIOPRO_TRANSMIT_PACKET_LENGTH, length);      // 0x3e 设置发射包数据长度

   phyWriteFIFO(length, txBuffer);                           //

   // enable just the packet sent IRQ
   phyWrite(EZRADIOPRO_INTERRUPT_ENABLE_1, EZRADIOPRO_ENPKSENT);   //使能发射中断
   phyWrite(EZRADIOPRO_INTERRUPT_ENABLE_2, 0x00);

   // read Si4432 interrupts to clear
   status = phyRead(EZRADIOPRO_INTERRUPT_STATUS_1);                //读清除终端标志
   status = phyRead(EZRADIOPRO_INTERRUPT_STATUS_2);

   // start TX
   phyWrite(EZRADIOPRO_OPERATING_AND_FUNCTION_CONTROL_1,(EZRADIOPRO_TXON|EZRADIOPRO_XTON));
 //  status = phyRead(EZRADIOPRO_OPERATING_AND_FUNCTION_CONTROL_1);
   while(si4432_NIRQ);
   status = phyRead(EZRADIOPRO_INTERRUPT_STATUS_1);            //读清除终端标志
   status = phyRead(EZRADIOPRO_INTERRUPT_STATUS_2);
   return SI4432_STATUS_SUCCESS;
}
//-----------------------------------------------------------------------------
// Function Name
//
// Return Value : None
// Parameters   :
//
//-----------------------------------------------------------------------------
SI4432_STATUS ppPhyRxOn (void)
{
   U8 status;

   RxPacketReceived = 0;

   // enable packet valid and CRC error IRQ
   phyWrite(EZRADIOPRO_INTERRUPT_ENABLE_1, EZRADIOPRO_ENPKVALID|EZRADIOPRO_ENCRCERROR);//0X05
   phyWrite(EZRADIOPRO_INTERRUPT_ENABLE_2, 0x00); //0X06

   // read Si4432 interrupts to clear
   status = phyRead(EZRADIOPRO_INTERRUPT_STATUS_1);	//0X03
   status = phyRead(EZRADIOPRO_INTERRUPT_STATUS_2);	 //0X04

   // enable RX
   phyWrite(EZRADIOPRO_OPERATING_AND_FUNCTION_CONTROL_1,(EZRADIOPRO_RXON|EZRADIOPRO_XTON));
   return SI4432_STATUS_SUCCESS;
}
//-----------------------------------------------------------------------------
// Function Name
//
// Return Value : None
// Parameters   :
//
//-----------------------------------------------------------------------------
SI4432_STATUS ppPhyRxOff (void)
{
   U8 status;

   // clear interrupt enables
   phyWrite(EZRADIOPRO_INTERRUPT_ENABLE_1, 0x00);
   phyWrite(EZRADIOPRO_INTERRUPT_ENABLE_2, 0x00);

   // read Si4432 interrupts to clear
   status = phyRead(EZRADIOPRO_INTERRUPT_STATUS_1);
   status = phyRead(EZRADIOPRO_INTERRUPT_STATUS_2);

   // disable RX
   phyWrite(EZRADIOPRO_OPERATING_AND_FUNCTION_CONTROL_1,(EZRADIOPRO_XTON));

   return SI4432_STATUS_SUCCESS;
}

unsigned char SPI_Read(void)
{
  unsigned char i, rxdata;
  rxdata=0;
  for(i=0;i<8;i++)
  {
     rxdata=rxdata<<1;
	 si4432_SCK=0;
	// si4432_SDO=1;
	 if(si4432_MISO==1)
	 {
	   rxdata|=0x01;
	 }
	 else
	 {
	   rxdata&=~0x01;
	 }
	 delay_n5us(1);
	 si4432_SCK=1;
	 delay_n5us(1);
  }
  return rxdata;
}
void SPI_Write(unsigned char txdata)
{
  unsigned char i;
  for(i=0;i<8;i++)
  {
     si4432_SCK=0;
	 if(txdata&0x80)
	 {
	   si4432_MOSI=1;
	 }
	 else
	 {
	   si4432_MOSI=0;
	 }
	 txdata=txdata<<1;
	 delay_n5us(1);
	 si4432_SCK=1;
	 delay_n5us(1);
  }   
}
void phyWrite (U8 reg, U8 value)
{
    si4432_NSEL=0;
	SPI_Write(reg|WR);
    SPI_Write(value);
	si4432_NSEL=1;
}
void burst_phyWrite (U8 reg, U8 value)
{
    si4432_NSEL=0;
	SPI_Write(reg|WR);
    SPI_Write(value);
	si4432_NSEL=1;
}
//-----------------------------------------------------------------------------
// Function Name
//
// Return Value : U8 value - value returned from the si4432 register
// Parameters   : U8 reg - register address from the si4432.h file.
//
//-----------------------------------------------------------------------------
U8 phyRead (U8 reg)
{
   U8 value;
   si4432_NSEL=0;
   SPI_Write(reg|RR);
   value=SPI_Read();
   si4432_NSEL=1;
   return value;
}
U8 burst_phyRead (U8 reg)
{
   U8 value;
   si4432_NSEL=0;
   SPI_Write(reg|RR);
   value=SPI_Read();
   si4432_NSEL=1;
   return value;
}

//-----------------------------------------------------------------------------
// Function Name
//
// Return Value : None
// Parameters   :
//
//-----------------------------------------------------------------------------
/*void phyWriteFIFO (U8 n, VARIABLE_SEGMENT_POINTER(buffer, U8, BUFFER_MSPACE))
{
   unsigned char cnt_num;
   for(cnt_num=0;cnt_num<n;cnt_num++)
    {
      phyWrite (EZRADIOPRO_FIFO_ACCESS,*buffer++);             // write buffer
    }
}*/
void phyWriteFIFO (U8 n, VARIABLE_SEGMENT_POINTER(buffer, U8, BUFFER_MSPACE))
{
   unsigned char cnt_num;
   for(cnt_num=0;cnt_num<n;cnt_num++)
    {
      burst_phyWrite (EZRADIOPRO_FIFO_ACCESS,*buffer++);             // write buffer
	}
}
//=============================================================================
//
// Receiver Functions
//
//=============================================================================
//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
SI4432_STATUS  ppPhyGetRxPacket(U8 pLength, VARIABLE_SEGMENT_POINTER(rxBuffer, U8, BUFFER_MSPACE))
{
   U8 i;

   if(RxPacketReceived)
   {
      // disable interrupts during copy
      for(i=0;i<pLength;i++)
      {
         rxBuffer[i]=RxIntBuffer[i];
      }

      RxPacketReceived = 0;
     // *pLength = RxPacketLength;

      return SI4432_STATUS_SUCCESS;
   }
   else
   {
      return SI4432_STATUS_ERROR_NO_PACKET;
   }
}

//-----------------------------------------------------------------------------
void RxIntphyReadFIFO (U8 n, VARIABLE_SEGMENT_POINTER(buffer, U8, BUFFER_MSPACE))
{
   unsigned char cnt_num;
   for(cnt_num=0;cnt_num<n;cnt_num++)
   {
      buffer[cnt_num]=burst_phyRead(EZRADIOPRO_FIFO_ACCESS);             // write buffer
   } 
  
}
//=============================================================================
// end ppPhy..c
//=============================================================================
