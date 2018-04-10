#include "headerfile.h"	
#include <string.h>
#include "string.h"
#include <stdio.h>
#include "bee.h" 
#include "led.h" 
#include "key.h"
#include "uart.h"
#include "1602lcd.h" 
#include "delay.h"		  
//#include "ODMRP.h"  //#include "wsn.h"
#include "si4432.h"
#include "XXTEA.h"
#include "ezradiopro.h"	  


//****************************************************************************//
//470mhz,1.2kbps,+-30khz,12ppm,4432b1,8052,UART,1602，实测空旷距离大于600m.穿墙效果好。
//20120714copy4432,1602,delay,main.c=Program Size: data=68.0(payload[32]) xdata=0 code=2626
//测试510MHZ，穿墙效果差。//测试500MHZ//测试490MHZ//测试480MHZ  //0714测试475MHZ，穿墙效果好。
//测试430MHZ//测试440MHZ//测试450MHZ//测试460MHZ//测试465MHZ
//发送数据装入数组20120714/测试472和471mhz不能通信。/测试40kbps穿墙效果略差。/测试9.6kbps=1.2k
//如果使用9.6kbps空中速率，每字节要0.833333ms。发一个64+11字节要63.333ms。发一个ok+短地址ff+11要13.333ms
//4432发送buf数据函数。
//XXTEA加密1kROM。解密1kROM
//需要改为9.6kbps，4432，delay，UART，key，led，deep，bee接口；另外要定义c51，avr，stm；还有主节点，分节点。
//需要code自带短地址，密码code自带，芯片id做加密使用。
//另外用户可以通过232接口更改短地址，用户电话号码，用户密码，设置数据，手动更新路由表（短2，长8，sleep1，跳数1，组1，端口1。
//编制自组网。开机自检和错误码
//将4432的函数全部转到4432
///关于纠错--采用bch向前纠错算法可以提高3db增益/
//本例最好采用多路转发，实际效果才好
//出厂地址主机设置0000，出厂地址分机设置0001，用户更改EPROM数据设置正确地址。这样省掉自组网。
//20130612单片机更改STM32，LGT，加上EPRROM，主机带诺基亚5110液晶，分机LGT带双向串口
//20130613未加路由前Program Size: data=9.0 xdata=29 const=0 code=3397
//20130613加入ODMRP路由程序，但未调试，主要是发射和接收地址。

//**********************************************************************************************//
//define headerfile.h
// #define _STM32 1		   //STM32，LGT，STM8单片机选择
//#define _DEBUG 1	  //调试状态
// #define _FFD 1		   //FFD主节点		RFD分节点
/******************************************************************************/



	
char code SST516[3] _at_ 0x003b;   //Keil Monitor-51 Driver c51仿真器专用
U8 sss,fff,ccc,ddd;
//f发射  s     ACK  c     CRC d

  //   Si4432-25K发射65字节时，示波器（时间分度5ms）低电平时间27.0ms
 	unsigned char   payloadTx[1]={0};  //发送缓存///在4432内有
	unsigned char   payloadRx[1]={0};  //接收缓存,回复，转发
    unsigned char    XXTEA_key[16]="0123456789abcdef";	//8密码+mac，可以分开不同人的设备
 /*********************************************************************************
	   unsigned char   payloadTx[32]={0x42,0x55,0x54,0x54,0x4F,0x4E,0x31,0x0D};  //发送缓存///在4432内有
	unsigned char   payloadRx[32]={0x42,0x55,0x54,0x54,0x4F,0x4E,0x31,0x0D};  //接收缓存,回复，转发
  unsigned char    XXTEA_key[16]="0123456789abcdef";	//8密码+mac，可以分开不同人的设备
  U8 code  TEL[6]={0x13,0x09,0x32,0x54,0x55,0x50};   //防盗功能，出厂默认号码，在EEPROM更改主人电话信息。
  //U8 code  mac[8]="zbee0001";   //前4位设备编号，后4位是编号//可以读单片机64bit唯一id
  //470－510MHz频段可作为民用无线电计量仪表
  U8 code TiaoPinBiao[21][2]={(0x56,0x00),	//460MHZ	//55,00,00-450MHZ/55,7d,00-455MHZ
							  (0x56,0x19),	//461MHZ
							  (0x56,0x32),	//462MHZ
							  (0x56,0x4B),	//463MHZ
							  (0x56,0x64),	//464MHZ
							  (0x56,0x7D),	//465MHZ                          
                              (0x56,0x96),  //466MHZ
							  (0x56,0xAF),	//467MHZ
							  (0x56,0xC8),	//468MHZ
							  (0x56,0xE1),	//469MHZ

							  (0x57,0x00),	//470MHZ
							  (0x57,0x19),	//471MHZ
							  (0x57,0x32),	//472MHZ
							  (0x57,0x4B),	//473MHZ
							  (0x57,0x64),	//474MHZ
							  (0x57,0x7D),	//475MHZ
							  (0x57,0x96),  //476MHZ
							  (0x57,0xAF),	//477MHZ
							  (0x57,0xC8),	//478MHZ
							  (0x57,0xE1),	//479MHZ

							  (0x60,0x00)	//480MHZ //0C,19,2580,32,3E80,4B,5780,64,7080,7d-490MHZ

							  };//固定跳频表  /61,00,00-500MHZ/

         U16 FangWeiID;//	ID短号，0为总机，255为广播。
          struct stu{unsigned char TiaoShu;   //跳数,通信失败f次，+1跳,
                     unsigned char DianYuan_ShiBaiLv;  //电源水平+失败率,通信失败1次，+1
					unsigned char YiDong_JianTing; //移动或停止状态+监听或睡眠状态
					unsigned char JiaoDu; //角度
					unsigned char Juli; //距离
					unsigned char Juli0; //距离
					 unsigned char Mac[8];	 //mac64bit唯一id
					unsigned int FangWeiID;  //id号2
                     } LuYouBiao[4];  //  一个节点有4条路由
/**********************************************************************************/
U16  ODMRP_main(void);
void main(void)
{

   
	U8 ItStatus1,ItStatus2;		 // ItStatus1有效数据包,ItStatus2??
//	U16 delay;
	U8 length,temp8;			//length有多少个数据//temp8用于接收缓存的数组下标，

  
  
	si4432_SDN = 0; // 开启4432
    UART_Init();//

	#ifdef _DEBUG  
	UART_Send_Str("UartInit.ok!\n");	 
    #endif

    #ifdef _FFD	
	//LCD_Init(); 
    //LCD_cls();
	//UART_Send_Str("lcdinit.ok!\n");
   // DisplayListChar(0,0,"LcdInit.ok!     ");
    #endif
	  bee_test();
//	 DisplayDataListChar(0,0,&payloadRx[0]);//显示接收缓存内容
//    XXTEA_JiaMi(); // XXTEA_Encrypt(payloadRx,XXTEA_key);      //TEA加密，数组dat里面的新内容就是加密后的数据。// XXTEA_JiaMi();
//	 DisplayDataListChar(0,0,&payloadRx[0]);//显示加密内容
//   XXTEA_JieMi();  //  XXTEA_Decrpyt(payloadRx,XXTEA_key);      //密文数据存放在dat里面，经TEA解密就能得到之前的明文。 // XXTEA_JieMi();
//	 DisplayDataListChar(0,1,&payloadRx[0]);//显示解密内容
//     LCD_cls();

 ODMRP_main();

//*********************************************************************
  

    RF_INIT();	
	 //DisplayDataListChar(0,1,&payloadRx[0]);//显示接收缓存内容



		
 //   UART_Send_Str("RFOK!TX(P3.7)+RX\n");
//	 DisplayListChar(0,0,"RFOK!TX(P3.7)+RX     ");
//**********************************************************************************************
while(1)
	{     

		//Poll the port pins of the MCU to figure out whether the push button is pressed or not
		if(KeyB == 0)
		{	
			//Wait for releasing the push button
			while( KeyB == 0 );
			//disable the receiver chain (but keep the XTAL running to have shorter TX on time!)
			RFSetIdleMode();		  //空闲模式?/	//write 0x01 to the Operating Function Control 1 register			

			//turn on the LED to show the packet transmission
			TX_LED = 0; 																			
			//The Tx deviation register has to set according to the deviation before every transmission (+-45kHz)
		  SpiWriteRegister(0x72, 0x30);		//频偏30write 0x30 to the Frequency Deviation register 
		  //SET THE CONTENT OF THE PACKET
			//set the length of the payload to 8bytes	
		      //	SpiWriteRegister(0x3E, 8);		//length发射数据包长度8 write 8 to the Transmit Packet Length register		
			

	RFWriteFIFO(8,&payloadTx[0]);//	RFWriteFIFO(8,&payloadTx[0]);//发射数据包长度+发射数据地址	RFWriteFIFO(8,&payloadF[0]);
			//fill the payload into the transmit FIFO
		/**********************************
			SpiWriteRegister(0x7F, 0x42);		//write 0x42 ('B') to the FIFO Access register	
			SpiWriteRegister(0x7F, 0x55);		//write 0x55 ('U') to the FIFO Access register	
			SpiWriteRegister(0x7F, 0x54);		//write 0x54 ('T') to the FIFO Access register	
			SpiWriteRegister(0x7F, 0x54);		//write 0x54 ('T') to the FIFO Access register	
			SpiWriteRegister(0x7F, 0x4F);		//write 0x4F ('O') to the FIFO Access register	
			SpiWriteRegister(0x7F, 0x4E);		//write 0x4E ('N') to the FIFO Access register	
			SpiWriteRegister(0x7F, 0x31);		//write 0x31 ('1') to the FIFO Access register	
			SpiWriteRegister(0x7F, 0x0D);		//write 0x0D (CR) to the FIFO Access register	
          //禁用所有其他中断,封包传输BUTTON1
		  /**********************************/
			//Disable all other interrupts and enable the packet sent interrupt only.
			//This will be used for indicating the successfull packet transmission for the MCU
			SpiWriteRegister(0x05, 0x04);		//write 0x04 to the Interrupt Enable 1 register	
			SpiWriteRegister(0x06, 0x00);		//write 0x03 to the Interrupt Enable 2 register	
			//Read interrupt status regsiters. It clear all pending interrupts and the si4432_NIRQ pin goes back to high.
			ItStatus1 = SpiReadRegister(0x03);			//read the Interrupt Status1 register
			ItStatus2 = SpiReadRegister(0x04);			//read the Interrupt Status2 register

			//enable transmitter
			//The radio forms the packet and send it automatically.
			RFSetTxMode();	      	  //发送模式    //write 0x09 to the Operating Function Control 1 register
	//		UART_Send_Str("TX Transmit\n");
		//	DisplayListChar(0,0,"TX Transmit     "); //TX Transmit     
			jc(14,0,++fff);
			//wait for the packet sent interrupt
			//The MCU just needs to wait for the 'ipksent' interrupt.
			while(si4432_NIRQ == 1); //
			//read interrupt status registers to release the interrupt flags
			ItStatus1 = SpiReadRegister(0x03);		//read the Interrupt Status1 register
			ItStatus2 = SpiReadRegister(0x04);		//read the Interrupt Status2 register

			//wait a bit for showing the LED a bit longer
			delay_ms(5); //for(delay = 0; delay < 10000;delay++);
			//turn off the LED
			TX_LED = 1; 
			
			//after packet transmission set the interrupt enable bits according receiving mode
			//Enable two interrupts: 
			// a) one which shows that a valid packet received: 'ipkval'
			// b) second shows if the packet received with incorrect CRC: 'icrcerror' 
			SpiWriteRegister(0x05, 0x03); 												//write 0x03 to the Interrupt Enable 1 register
			SpiWriteRegister(0x06, 0x00); 												//write 0x00 to the Interrupt Enable 2 register
			//read interrupt status registers to release all pending interrupts
			ItStatus1 = SpiReadRegister(0x03);											//read the Interrupt Status1 register
			ItStatus2 = SpiReadRegister(0x04);											//read the Interrupt Status2 register
			//set the Frequency Deviation register according to the AFC limiter												
			//SpiWriteRegister(0x72, 0x1F);												//write 0x1F to the Frequency Deviation register		

			//enable receiver chain again
			RFSetRxMode();				  //接受模式		//write 0x05 to the Operating Function Control 1 register				
		}

		
	    //wait for the interrupt event
		//If it occurs, then it means a packet received or CRC error happened
       
//**********************************************************************************************
		if( si4432_NIRQ == 0 )
		{
			//disable the receiver chain 
			SpiWriteRegister(0x07, 0x01);												//write 0x01 to the Operating Function Control 1 register
			//read interrupt status registers 
			ItStatus1 = SpiReadRegister(0x03);											//read the Interrupt Status1 register
			ItStatus2 = SpiReadRegister(0x04);											//read the Interrupt Status2 register

			//CRC Error interrupt occured
			if( (ItStatus1 & 0x01) == 0x01 )    //CRC错误
			{
			  // UART_Send_Str("CRC Error\n");
				#ifdef _FFD	  
				DisplayListChar(9,1,"CrcE");
				#endif
				jc(14,1,++ccc);
				//reset the RX FIFO
				//if(ccc=0xf)  RF_INIT();  //16次CRC错误后重起4432
       	   		SpiWriteRegister(0x08, 0x02);											//write 0x02 to the Operating Function Control 2 register
        	    SpiWriteRegister(0x08, 0x00);											//write 0x00 to the Operating Function Control 2 register
				//blink all LEDs to show the error
		
				 delay_ms(5); //for(delay = 0; delay < 10000;delay++);
			
			}

			//packet received interrupt occured
			if( (ItStatus1 & 0x02) == 0x02 ) //如果有效数据包则继续
			{
 				//Read the length of the received payload
				length = SpiReadRegister(0x4B);		//有多少个数据		//read the Received Packet Length register
				//check whether the received payload is not longer than the allocated buffer in the MCU
				if(length < 11)
				{
	             RX_LED = 0;
					//Get the reeived payload from the RX FIFO
					for(temp8=0;temp8 < length;temp8++)
					{
						payloadRx[temp8] = SpiReadRegister(0x7F);	//接收数据read the FIFO Access register
					}
			
					//check whether the acknowledgement packet received
					if( length == 4 ) 
	            	{
	            		if( memcmp(&payloadRx[0], "ACK", 3) == 0 ) //比较s1和s2的前3个字节的ascII码值？
					   	{
	                  		//blink LED2 to show that ACK received
							//UART_Send_Str("ACK OK\n");
	     				//	DisplayListChar(0,1,"AckOk");
							jc(5,1,++sss);
					delay_ms(5); 	//	for(delay = 0; delay < 10000;delay++);
	  		   				
						}
					}

					//check whether an expected packet received, this should be acknowledged
					if( length == 8 ) 
	            	{   
					   // DisplayDataListChar(0,0,&payloadRx[0]);//显示接收缓存内容//DisplayDataListChar(8,0,&payload[0]);    
	            		if( memcmp(&payloadRx[0], "BUTTON1", 7) == 0 )    //比较s1和s2的前7个字节的ascII码值
					   	{
							//UART_Send_Str("DATA OK\n");
					//	DisplayListChar(0,1,"DataOk");
							jc(6,1,++ddd);
						
	                  		//blink LED2 to show that the packet received
	     				 	RX_LED = 1;
					   	 delay_ms(5); //	for(delay = 0; delay < 10000;delay++);
	  		   			

							//send back an acknowledgement
							//turn on LED1 to show packet transmission
							//TX_LED = 1; 	

						 // RFSetAckTxMode(); //发送ACK应答
						/*********************RFSetAckTxMode start20120712**********************/				
							//The Tx deviation register has to set according to the deviation before every transmission (+-45kHz)
							SpiWriteRegister(0x72, 0x30);	//write 0x30 to the Frequency Deviation register 
							//set packet content
							//set the length of the payload to 4bytes	
							SpiWriteRegister(0x3E, 4);									//write 4 to the Transmit Packet Length register		
							//fill the payload into the transmit FIFO
							SpiWriteRegister(0x7F, 0x41);	//write 0x42 ('A') to the FIFO Access register	
							SpiWriteRegister(0x7F, 0x43);	//write 0x55 ('C') to the FIFO Access register	
							SpiWriteRegister(0x7F, 0x4B);	//write 0x54 ('K') to the FIFO Access register	
							SpiWriteRegister(0x7F, 0x0D);	//write 0x0D (CR) to the FIFO Access register	

							//Disable all other interrupts and enable the packet sent interrupt only.
							//This will be used for indicating the successfull packet transmission for the MCU
							SpiWriteRegister(0x05, 0x04);	//使能包发送//write 0x04 to the Interrupt Enable 1 register	
							SpiWriteRegister(0x06, 0x00);	//write 0x03 to the Interrupt Enable 2 register	
							//Read interrupt status regsiters. It clear all pending interrupts and the si4432_NIRQ pin goes back to high.
							ItStatus1 = SpiReadRegister(0x03);							//read the Interrupt Status1 register
							ItStatus2 = SpiReadRegister(0x04);							//read the Interrupt Status2 register

							//enable transmitter
							//The radio forms the packet and send it automatically.
							RFSetTxMode();				  //发送模式				//write 0x09 to the Operating Function Control 1 register

							//wait for the packet sent interrupt
							//The MCU just needs to wait for the 'ipksent' interrupt.
							while(si4432_NIRQ == 1);
							//read interrupt status registers to release the interrupt flags
							ItStatus1 = SpiReadRegister(0x03);							//read the Interrupt Status1 register
							ItStatus2 = SpiReadRegister(0x04);							//read the Interrupt Status2 register

							//wait a bit for showing the LED a bit longer
							//delay_ms(5); //for(delay = 0; delay < 10000;delay++);
							//turn off the LED
							//TX_LED = 0; 	
							   //DisplayDataListChar(0,0,&payloadRx[0]);//显示接收缓存内容	 ///test
							//after packet transmission set the interrupt enable bits according receiving mode
							//Enable two interrupts: 
							// a) one which shows that a valid packet received: 'ipkval'
							// b) second shows if the packet received with incorrect CRC: 'icrcerror' 
							SpiWriteRegister(0x05, 0x03); 								//write 0x03 to the Interrupt Enable 1 register
							SpiWriteRegister(0x06, 0x00); 								//write 0x00 to the Interrupt Enable 2 register
							//read interrupt status registers to release all pending interrupts
							ItStatus1 = SpiReadRegister(0x03);							//read the Interrupt Status1 register
							ItStatus2 = SpiReadRegister(0x04);							//read the Interrupt Status2 register
							//set the Frequency Deviation register according to the AFC limiter												
							//SpiWriteRegister(0x72, 0x1F);								//write 0x1F to the Frequency Deviation register		
						   	/*********************RFSetAckTxMode end**********************/
						}
					}
				}
       		}
			//enable receiver chain again
			RFSetRxMode();					  //接受模式			//write 0x05 to the Operating Function Control 1 register
		} 	
	}


 
}
/**********************************************************************************************/









	//设置接受中断先！！！！！！！！！！

/**********************************************************************************************

void xianshi()
{ 
 jc(0,1,SpiReadRegister(0x00));
 jc(2,1,SpiReadRegister(0x01));


 //DisplayListChar(3,1,"02H ");
 //DisplayListChar(6,1,"03H ");
 //DisplayListChar(9,1,"04H ");
 //DisplayListChar(12,1,"05H ");
 //DisplayListChar(15,1,"06H ");
 //DisplayListChar(18,1,"07H ");

 jc(3,2,SpiReadRegister(0x02));
 jc(6,2,SpiReadRegister(0x03));
 jc(9,2,SpiReadRegister(0x04));

 jc(12,2,SpiReadRegister(0x05));
 jc(15,2,SpiReadRegister(0x06));
 jc(18,2,SpiReadRegister(0x07));
 }

void main1(void)
{

 unsigned char *txp,i;

 U8 ItStatus1,ItStatus2;

 for(i=0;i<8;i++)
 {
  *txp++ = i;
 }

 LCMInit(); 
 LCD_cls();

	si4432_SDN = 0;
	si4432_SDN = 0;
	si4432_SDN = 0;
	si4432_SDN = 0;

	Delay400Ms();



    ItStatus1 = SpiReadRegister(0x03);													//read the Interrupt Status1 register
	ItStatus2 = SpiReadRegister(0x04);													//read the Interrupt Status2 register
	

    SpiWriteRegister(0x07, 0x80);														//write 0x80 to the Operating & Function Control1 register 
	

	ItStatus1 = SpiReadRegister(0x03);													//read the Interrupt Status1 register
	ItStatus2 = SpiReadRegister(0x04);	

	RFSetIdleMode();



 RF_init(); 



 DisplayListChar(0,0,"LCD+Si4432 ok!  TX ");


xianshi();

//IA4432_Register((REG_WRITE | FrequencyBandSelect), 0x53);         //75//set frequency 
//IA4432_Register((REG_WRITE | NominalCarrierFrequency1), 0x4b);    //76//433HZ 
//IA4432_Register((REG_WRITE | NominalCarrierFrequency0), 0x00); 	  //77//频率控制 

 //jc(4,2,SpiReadRegister(0x20));
 //jc(6,2,SpiReadRegister(0x21));
 //jc(8,2,SpiReadRegister(0x22));
 //jc(10,2,SpiReadRegister(0x23));
 //jc(12,2,SpiReadRegister(0x24));
 //jc(14,2,SpiReadRegister(0x25));

 //jc(16,3,SpiReadRegister(0x6E));
 //jc(18,3,SpiReadRegister(0x6F));

 //jc(0,3,SpiReadRegister(0x02));

  Delay400Ms();

while(1)
	{

	    RFResetRxFIFO();
		RFResetTxFIFO();

	  RFFIFOSendData(8, txp);

	  //jc(0,3,SpiReadRegister(0x02));

	  xianshi();

	  Delay400Ms();
	  Delay400Ms();		

	  RFSetIdleMode();

	  xianshi();

	  Delay400Ms();


	  //jc(11,3,wendu);


	}

}


*******************************************************************************/
