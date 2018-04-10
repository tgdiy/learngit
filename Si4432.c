#include "Si4432.h"
#include "delay.h"



#define TxGPIOSetting()   SpiWriteRegister(0x0E, 0x02);
#define RxGPIOSetting()   SpiWriteRegister(0x0E, 0x01);

#define	REG_READ					(0x00)
#define	REG_WRITE					(0x80)



 	U8 ItStatus1,ItStatus2;
//unsigned char xdata TxBuffer[64]

//**********************************************************************************************//
void RF_INIT(void)
   {
    
 	U8 ItStatus1,ItStatus2;
//	U16 delay;
//	U8 temp8;


	si4432_SDN  (ON); // ����4432	//Turn on the radio by pulling down the PWRDN pin
	///Wait at least 15ms befory any initialization SPI commands are sent to the radio
	/// (wait for the power on reset sequence) 
	///for (temp8=0;temp8<15;temp8++)
	///{
	///	for(delay=0;delay<10000;delay++);
	///}
	//read interrupt status registers to clear the interrupt flags and release si4432_NIRQ pin
	ItStatus1 = SpiReadRegister(0x03);			//read the Interrupt Status1 register
	ItStatus2 = SpiReadRegister(0x04);			//read the Interrupt Status2 register

	//SW reset   
   	SpiWriteRegister(0x07, 0x80);	//write 0x80 to the Operating & Function Control1 register 
	    //wait for chip ready interrupt from the radio (while the si4432_NIRQ pin is high)
	while ( si4432_NIRQ == ON )  //read interrupt status registers to clear the interrupt flags and release si4432_NIRQ pinzigbee
	{  	
	ItStatus1 = SpiReadRegister(0x03);			//read the Interrupt Status1 register
	ItStatus2 = SpiReadRegister(0x04);			//read the Interrupt Status2 register
	}					
							/*set the physical parameters*/
	SpiWriteRegister(0x75, 0x57);	            //set the center frequency to yunjia470 MHz	//write data to the Frequency Band Select register             
	SpiWriteRegister(0x76, 0x00);	          	//00  write data to the Nominal Carrier Frequency1 register
	SpiWriteRegister(0x77, 0x00);             //����Ƶ��470mhz	//write data to the Nominal Carrier Frequency0 register

	SpiWriteRegister(0x6D, 0x1F);	            //���书��//write 0x1F to the TX Power register 					
	



	SpiWriteRegister(0x34, 0x0A);	          // 0x0C //ǰ���볤��(A+1)*4=44=5.5��//write 0x14 to the Preamble Length register 	

	//set preamble detection threshold to 20bits
	SpiWriteRegister(0x35, 0x28);            //0x28=00101  000������ֽ���=5  xxx��������������=0	
    //0x2A==00101  010//write 0x2A to the Preamble Detection Control  register

	//Disable header bytes; set variable packet length (the length of the payload is defined by the
	//received packet length field of the packet); set the synch word to two bytes long
	SpiWriteRegister(0x33, 0x02);	            //ͬ���ֿ���3��2ͬ����(��2λ��	//write 0x02 to the Header Control2 register      
	
	//Set the sync word pattern to 0x2DD4
	SpiWriteRegister(0x36, 0x2D);             //36Hͬ����3Ϊ2D,	//write 0x2D to the Sync Word 3 register
	SpiWriteRegister(0x37, 0xD4);	           //37Hͬ����2ΪD4  //write 0xD4 to the Sync Word 2 register
	
	//enable the TX & RX packet handler and CRC-16 (IBM) check
	SpiWriteRegister(0x30, 0x8D);//0x8D		//���ݴ�������
   //ʹ�����ݰ�FIFO�ķ�������յĵ��Զ�����,ѭ����У����
	//ʹ��CRC������Ϊ16λCRC///////�ش����ã� //write 0x8D to the Data Access Control register
	
	//Disable the receive header filters
   	SpiWriteRegister(0x32, 0x00 );	   	//֡ͷ����1//write 0x00 to the Header Control1 register            
	//�޹㲥��ַ��û�н��յ�֡ͷ���




	//enable FIFO mode and GFSK modulation
   	SpiWriteRegister(0x71, 0x23);          //0x22=fsk//write 0x63 to the Modulation Mode Control 2 register										
   			      //FIFOģʽ��GFSK	//Set GPIO0 output��

              //����RF PA����
    	SpiWriteRegister(0x0B, 0x14);      //��������(���)//�����޸����ŵ�����(���)=0x1C
		SpiWriteRegister(0x0C, 0x12);				//����״̬(���)Set GPIO1 output
   	SpiWriteRegister(0x0D, 0x15);	         //����״̬(���) //Set GPIO2 output///���Ը���ΪGPIO0������GPIO2�������ʱ��

	//Set Crystal Oscillator Load Capacitance register, the user should change the value of this register while use different crystal.
	   SpiWriteRegister(0x09, 0xDA);	        //12PPM  //����г����ݣ����Ҹ���0xB8
		
		SpiWriteRegister(0x6E, 0x0A);		   	//09write data to the TXDataRate 1 register
		SpiWriteRegister(0x6F, 0x3D);	      	//D5���䲨����1.2kbps	//write data to the TXDataRate 0 register	
		
		SpiWriteRegister(0x58, 0x80);	        //Ĭ�ϵ�ɱö�PLL��������оƬĬ��   //set the Tx deviation register (+-20kHz)	                                                    
		SpiWriteRegister(0x72, 0x30);         //30Ƶƫ//write data to the Frequency Deviation register 
	    SpiWriteRegister(0x70, 0x0C); //0x2C//����˹�ط��ݣ�//write data to the Modulation Mode Control 1 register





			/*Si443x Register Settings_RevB1-v5.xls20120624-set the modem parameters according to the exel calculator
			//(parameters: 1.2 kbps, deviation: 30 kHz*/
		SpiWriteRegister(0x1C, 0x05);	      //16 //IF�˲�������	//write data to the IF Filter Bandwidth register		
		SpiWriteRegister(0x20, 0x64);        //0x83	//ʱ�ӻָ��������//write data to the Clock Recovery Oversampling Ratio register		
		SpiWriteRegister(0x21, 0x01);       //0xC0	//ʱ�ӻָ�ƫ��2//write data to the Clock Recovery Offset 2 register		
		SpiWriteRegister(0x22, 0x47);			//13ʱ�ӻָ�ƫ��1//write data to the Clock Recovery Offset 1 register		
		SpiWriteRegister(0x23, 0xAE);			//A9ʱ�ӻָ�ƫ��0//write data to the Clock Recovery Offset 0 register		
		SpiWriteRegister(0x24, 0x03);		   //00ʱ�ӻָ���ʱѭ������1	//write data to the Clock Recovery Timing Loop Gain 1 register			
		SpiWriteRegister(0x25, 0x6C);       //0X04	//ʱ�ӻָ���ʱѭ������0//write data to the Clock Recovery Timing Loop Gain 0 register		
		SpiWriteRegister(0x1D, 0x40);	   	///40//AFCѭ�����ټ���  ��AFCʹ��//write data to the AFC Loop Gearshift Override register		
		SpiWriteRegister(0x1E, 0x0A);	   	///0A//AFC��ʱ
		SpiWriteRegister(0x2A, 0x28);       ///14//0X21//AFC�Է�//write data to the AFC Limiter register				
		SpiWriteRegister(0x1F, 0x03);       //ʱ�ӻָ����ٳ���    ʱ�ӻָ�������ֵ		
		SpiWriteRegister(0x69, 0x60);	   	 //�Զ����� (AGC��
									
		/*enable receiver chain*/
	RFSetRxMode();			  //����ģʽ//write 0x05 to the Operating Function Control 1 register
	//Enable two interrupts: 
	// a) one which shows that a valid packet received: 'ipkval'
	// b) second shows if the packet received with incorrect CRC: 'icrcerror' 
	SpiWriteRegister(0x05, 0x03); 		   	//03//write 0x03 to the Interrupt Enable 1 register
	SpiWriteRegister(0x06, 0x00); 				//write 0x00 to the Interrupt Enable 2 register
	//read interrupt status registers to release all pending interrupts
	ItStatus1 = SpiReadRegister(0x03);				//read the Interrupt Status1 register
	ItStatus2 = SpiReadRegister(0x04);				//read the Interrupt Status2 register
   }  

/***********************************************************

void delay(unsigned char delaytime)
{
	unsigned char i = 0;
	for(i = 0; i < delaytime; i++);
}

/*****************************************************************************************/
void SpiRfWrite8Bit(unsigned char byte) 
{ 
  unsigned char temp; 
  si4432_SCK (ON); 
  delay_ms(5);  
  for(temp=0;temp<8;temp++) 
  {  
    si4432_MOSI = (byte&0x80)>>7;  
    byte = byte<<1; 
    si4432_SCK (OFF); 
    si4432_SCK (OFF); 
    si4432_SCK (ON); 
  } 
} 

/*****************************************************************************************/
unsigned char SpiRfRead8Bit(void) 
{ 
  unsigned char temp; 
  unsigned char byte = 0x00; 
  si4432_SCK (ON); 
  delay_ms(5);  
  for(temp=0;temp<8;temp++) 
  { 
    byte = byte<<1; 
    si4432_SCK (OFF); 
   delay_ms(5);   
    byte = byte|(si4432_MISO&0x01); 
 

    si4432_SCK (ON); 
     delay_ms(5);   
  } 
  return(byte); 
} 

/*****************************************************************************************/
void SpiWriteRegister(unsigned char add, unsigned char reg)  
{ 
   
   si4432_NSEL (ON); 
   add |=REG_WRITE; 
   SpiRfWrite8Bit(add);  
   SpiRfWrite8Bit(reg);  
   si4432_NSEL (OFF);  
} 

/*****************************************************************************************/
unsigned char SpiReadRegister(unsigned char add)  
{ 
   unsigned char shuju;  
   si4432_NSEL (ON);  
   add &=~REG_WRITE; 
   SpiRfWrite8Bit(add);  
   shuju = SpiRfRead8Bit();  
   si4432_NSEL (OFF);  
   return shuju;  
} 

/*****************************************************************************************
void RF_GPIO(void)
{
	SpiWriteRegister(0x0B, 0xCA);															//Set GPIO0 output
   	SpiWriteRegister(0x0C, 0xCA);															//Set GPIO1 output
   	SpiWriteRegister(0x0D, 0xCA);															//Set GPIO2 output
	//3��GPIO���ó�ֱ���������
	//Set Crystal Oscillator Load Capacitance register, the user should change the value of this register while use different crystal.
	SpiWriteRegister(0x09, 0xB8);
	//����г����ݣ����Ҹ���0xB8

}//30H�����ã��Ƿ����ó��Զ���31H�Ķ��������������õĶ��٣����ܶ��Ƿ�Ӧ�ö�����Щ��
 //�Գ�ֽ���

/*****************************************************************************************
unsigned char wendu()
{

 unsigned char tt;

 SpiWriteRegister((REG_WRITE | IOPortConfiguration), 0x00); 	 		 //0E  ADC����
 SpiWriteRegister((REG_WRITE | TemperatureSensorControl), 0x00); 	 //12  �¶ȼĴ�������
 SpiWriteRegister((REG_WRITE | ADCConfiguration), 0x80); 			 //0F  ADC����

																					    
 tt = SpiReadRegister(ADCValue);									 //11 ADC����Ĵ���

 return tt	;

}

/*****************************************************************************************/
//RFSetRxMode();
void RFSetRxMode(void)				  //����ģʽ
{  

	//ItStatus1 = SpiReadRegister(0x03);												    	//read the Interrupt Status1 register
	//ItStatus2 = SpiReadRegister(0x04);													    //read the Interrupt Status2 register

	RxGPIOSetting();  //SpiWriteRegister(0x0E, 0x01)//����ģʽ;

	SpiWriteRegister(0x07, 0x05);  
}

/*****************************************************************************************/
//RFSetTxMode();
void RFSetTxMode(void)				   //����ģʽ
{
	
	//ItStatus1 = SpiReadRegister(0x03);												//read the Interrupt Status1 register
	//ItStatus2 = SpiReadRegister(0x04);												//read the Interrupt Status2 register


	TxGPIOSetting();  //SpiWriteRegister(0x0E, 0x02)//����ģʽ;


	SpiWriteRegister(0x07, 0x09);
}


/*****************************************************************************************/
//RFSetIdleMode();
void RFSetIdleMode(void)                      //����ģʽ?//RFSetIdleMode()
{

	SpiWriteRegister(0x07, 0x01);													//write 0x09 to the Operating Function Control 1 register

	
	ItStatus1 = SpiReadRegister(0x03);												//read the Interrupt Status1 register
	ItStatus2 = SpiReadRegister(0x04);												//read the Interrupt Status2 register

}


/*************************************************************20120714****************************
void phyWriteFIFO (U8 n, VARIABLE_SEGMENT_POINTER(buffer, U8, BUFFER_MSPACE))
{
   unsigned char cnt_num;
   for(cnt_num=0;cnt_num<n;cnt_num++)
    {
      burst_phyWrite (EZRADIOPRO_FIFO_ACCESS,*buffer++);             // 0x7F  //FIFO��ȡwrite buffer
	}
}
 void SpiWriteRegister(unsigned char add, unsigned char reg)  
{ 
   
   si4432_NSEL (ON); 
   add |=REG_WRITE; 
   SpiRfWrite8Bit(add);  
   SpiRfWrite8Bit(reg);  
   si4432_NSEL  (OFF);  
} 

/*****************************************************************************************
unsigned char SpiReadRegister(unsigned char add)  
{ 
   unsigned char shuju;  
   si4432_NSEL (ON);  
   add &=~REG_WRITE; 
   SpiRfWrite8Bit(add);  
   shuju = SpiRfRead8Bit();  
   si4432_NSEL (OFF);  
   return shuju;  
} 
/*************************************************************20120714****************************/
  			void   RFWriteFIFO( U8 z,  U8 *(buffer))
                 {        
                    unsigned char shuju,temp8; 
					SpiWriteRegister(0x3E,z); //���㷢�����ݰ�����
                 	for(temp8=0;temp8 < z;temp8++)
	             	{  shuju=buffer[temp8];
	                	SpiWriteRegister(0x7F,shuju);	//д������Write the FIFO Access register
	                }
                  }
/*************************************************************20120714****************************
void phyWriteFIFO (U8 n, VARIABLE_SEGMENT_POINTER(buffer, U8, BUFFER_MSPACE))
{
   unsigned char cnt_num;
   for(cnt_num=0;cnt_num<n;cnt_num++)
    {
	 burst_phyWrite (EZRADIOPRO_FIFO_ACCESS,*buffer++);             // 0x7F  //FIFO��ȡwrite buffer
	}
}
void burst_phyWrite (U8 reg, U8 value)
{
    si4432_NSEL (ON);
	SPI_Write(reg|WR);
    SPI_Write(value);
	si4432_NSEL (OFF);
}
void SPI_Write(unsigned char txdata)
{
  unsigned char i;
  for(i=0;i<8;i++)
  {
     si4432_SCK (ON);
	 if(txdata&0x80)
	 {
	   si4432_MOSI (OFF);
	 }
	 else
	 {
	   si4432_MOSI (ON);
	 }
	 txdata=txdata<<1;
	 delay_n5us(1);
	 si4432_SCK (OFF);
	 delay_n5us(1);
  }   
}
/*************************************************************20120714****************************/
/*************************************************************20120712****************************
void RFFIFOSendData(U8 length, U8 *payload)
{
	U8 i;

	RFSetIdleMode();
											
	SpiWriteRegister(0x3E, length);

	for(i = 0; i < length; i++)
	{
		SpiWriteRegister(0x7F, payload[i]);	
	}

		//Disable all other interrupts and enable the packet sent interrupt only.
							//This will be used for indicating the successfull packet transmission for the MCU
							SpiWriteRegister(0x05, 0x04);	//ʹ�ܰ�����//write 0x04 to the Interrupt Enable 1 register	
							SpiWriteRegister(0x06, 0x00);	//write 0x03 to the Interrupt Enable 2 register	
							//Read interrupt status regsiters. It clear all pending interrupts and the si4432_NIRQ pin goes back to high.
							ItStatus1 = SpiReadRegister(0x03);							//read the Interrupt Status1 register
							ItStatus2 = SpiReadRegister(0x04);							//read the Interrupt Status2 register

							//enable transmitter
							//The radio forms the packet and send it automatically.
							RFSetTxMode();				  //����ģʽ				//write 0x09 to the Operating Function Control 1 register

							//wait for the packet sent interrupt
							//The MCU just needs to wait for the 'ipksent' interrupt.
							while(si4432_NIRQ == 1);
							//read interrupt status registers to release the interrupt flags
							ItStatus1 = SpiReadRegister(0x03);							//read the Interrupt Status1 register
							ItStatus2 = SpiReadRegister(0x04);							//read the Interrupt Status2 register

}

/*****************************************************************************************
unsigned char jieshou()
{
		unsigned char payload[8],length,temp8;   //����������payload[8]
		if( si4432_NIRQ == 0 )
		{
			RFSetIdleMode();	//����ģʽ	                                        
			if( (ItStatus1 & 0x01) == 0x01 )//��03H���CRC������������ݰ�
			{
				RFResetRxFIFO();
			Delay5Ms();   ///	Delay5Ms1();
			}
			  //�����Ч���ݰ������
			if( (ItStatus1 & 0x02) == 0x02 )
			{
				length = SpiReadRegister(0x4B);		//�ж��ٸ�����
				if(length <= 8)
				{
					//Get the received payload from the RX FIFO
					for(temp8=0;temp8 < length;temp8++)
					{
               	payload[temp8] = SpiReadRegister(0x7F);		//��������read the FIFO Access register
									
					}			
		  		}
			}
		}
		
	ItStatus1 = SpiReadRegister(0x03);												//read the Interrupt Status1 register
	ItStatus2 = SpiReadRegister(0x04);												//read the Interrupt Status2 register
		return *payload;     //����������ָ��payload[8]
}


/****************************************************************************************
void RFResetRxFIFO(void)		 //�������FIFO
{
	U8 value;

	value = SpiReadRegister(0x08);
	SpiWriteRegister(0x08, value | 0x02);
	SpiWriteRegister(0x08, value & (~0x02));
}

/****************************************************************************************
void RFResetTxFIFO(void)         //�������FIFO
{
	U8 value;

	value = SpiReadRegister(0x08);
	SpiWriteRegister(0x08, value | 0x01);
	SpiWriteRegister(0x08, value & (~0x01));
}


/*****************************************************************************************/

