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
//470mhz,1.2kbps,+-30khz,12ppm,4432b1,8052,UART,1602��ʵ��տ��������600m.��ǽЧ���á�
//20120714copy4432,1602,delay,main.c=Program Size: data=68.0(payload[32]) xdata=0 code=2626
//����510MHZ����ǽЧ���//����500MHZ//����490MHZ//����480MHZ  //0714����475MHZ����ǽЧ���á�
//����430MHZ//����440MHZ//����450MHZ//����460MHZ//����465MHZ
//��������װ������20120714/����472��471mhz����ͨ�š�/����40kbps��ǽЧ���Բ/����9.6kbps=1.2k
//���ʹ��9.6kbps�������ʣ�ÿ�ֽ�Ҫ0.833333ms����һ��64+11�ֽ�Ҫ63.333ms����һ��ok+�̵�ַff+11Ҫ13.333ms
//4432����buf���ݺ�����
//XXTEA����1kROM������1kROM
//��Ҫ��Ϊ9.6kbps��4432��delay��UART��key��led��deep��bee�ӿڣ�����Ҫ����c51��avr��stm���������ڵ㣬�ֽڵ㡣
//��Ҫcode�Դ��̵�ַ������code�Դ���оƬid������ʹ�á�
//�����û�����ͨ��232�ӿڸ��Ķ̵�ַ���û��绰���룬�û����룬�������ݣ��ֶ�����·�ɱ���2����8��sleep1������1����1���˿�1��
//�����������������Լ�ʹ�����
//��4432�ĺ���ȫ��ת��4432
///���ھ���--����bch��ǰ�����㷨�������3db����/
//������ò��ö�·ת����ʵ��Ч���ź�
//������ַ��������0000��������ַ�ֻ�����0001���û�����EPROM����������ȷ��ַ������ʡ����������
//20130612��Ƭ������STM32��LGT������EPRROM��������ŵ����5110Һ�����ֻ�LGT��˫�򴮿�
//20130613δ��·��ǰProgram Size: data=9.0 xdata=29 const=0 code=3397
//20130613����ODMRP·�ɳ��򣬵�δ���ԣ���Ҫ�Ƿ���ͽ��յ�ַ��

//**********************************************************************************************//
//define headerfile.h
// #define _STM32 1		   //STM32��LGT��STM8��Ƭ��ѡ��
//#define _DEBUG 1	  //����״̬
// #define _FFD 1		   //FFD���ڵ�		RFD�ֽڵ�
/******************************************************************************/



	
char code SST516[3] _at_ 0x003b;   //Keil Monitor-51 Driver c51������ר��
U8 sss,fff,ccc,ddd;
//f����  s     ACK  c     CRC d

  //   Si4432-25K����65�ֽ�ʱ��ʾ������ʱ��ֶ�5ms���͵�ƽʱ��27.0ms
 	unsigned char   payloadTx[1]={0};  //���ͻ���///��4432����
	unsigned char   payloadRx[1]={0};  //���ջ���,�ظ���ת��
    unsigned char    XXTEA_key[16]="0123456789abcdef";	//8����+mac�����Էֿ���ͬ�˵��豸
 /*********************************************************************************
	   unsigned char   payloadTx[32]={0x42,0x55,0x54,0x54,0x4F,0x4E,0x31,0x0D};  //���ͻ���///��4432����
	unsigned char   payloadRx[32]={0x42,0x55,0x54,0x54,0x4F,0x4E,0x31,0x0D};  //���ջ���,�ظ���ת��
  unsigned char    XXTEA_key[16]="0123456789abcdef";	//8����+mac�����Էֿ���ͬ�˵��豸
  U8 code  TEL[6]={0x13,0x09,0x32,0x54,0x55,0x50};   //�������ܣ�����Ĭ�Ϻ��룬��EEPROM�������˵绰��Ϣ��
  //U8 code  mac[8]="zbee0001";   //ǰ4λ�豸��ţ���4λ�Ǳ��//���Զ���Ƭ��64bitΨһid
  //470��510MHzƵ�ο���Ϊ�������ߵ�����Ǳ�
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

							  };//�̶���Ƶ��  /61,00,00-500MHZ/

         U16 FangWeiID;//	ID�̺ţ�0Ϊ�ܻ���255Ϊ�㲥��
          struct stu{unsigned char TiaoShu;   //����,ͨ��ʧ��f�Σ�+1��,
                     unsigned char DianYuan_ShiBaiLv;  //��Դˮƽ+ʧ����,ͨ��ʧ��1�Σ�+1
					unsigned char YiDong_JianTing; //�ƶ���ֹͣ״̬+������˯��״̬
					unsigned char JiaoDu; //�Ƕ�
					unsigned char Juli; //����
					unsigned char Juli0; //����
					 unsigned char Mac[8];	 //mac64bitΨһid
					unsigned int FangWeiID;  //id��2
                     } LuYouBiao[4];  //  һ���ڵ���4��·��
/**********************************************************************************/
U16  ODMRP_main(void);
void main(void)
{

   
	U8 ItStatus1,ItStatus2;		 // ItStatus1��Ч���ݰ�,ItStatus2??
//	U16 delay;
	U8 length,temp8;			//length�ж��ٸ�����//temp8���ڽ��ջ���������±꣬

  
  
	si4432_SDN = 0; // ����4432
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
//	 DisplayDataListChar(0,0,&payloadRx[0]);//��ʾ���ջ�������
//    XXTEA_JiaMi(); // XXTEA_Encrypt(payloadRx,XXTEA_key);      //TEA���ܣ�����dat����������ݾ��Ǽ��ܺ�����ݡ�// XXTEA_JiaMi();
//	 DisplayDataListChar(0,0,&payloadRx[0]);//��ʾ��������
//   XXTEA_JieMi();  //  XXTEA_Decrpyt(payloadRx,XXTEA_key);      //�������ݴ����dat���棬��TEA���ܾ��ܵõ�֮ǰ�����ġ� // XXTEA_JieMi();
//	 DisplayDataListChar(0,1,&payloadRx[0]);//��ʾ��������
//     LCD_cls();

 ODMRP_main();

//*********************************************************************
  

    RF_INIT();	
	 //DisplayDataListChar(0,1,&payloadRx[0]);//��ʾ���ջ�������



		
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
			RFSetIdleMode();		  //����ģʽ?/	//write 0x01 to the Operating Function Control 1 register			

			//turn on the LED to show the packet transmission
			TX_LED = 0; 																			
			//The Tx deviation register has to set according to the deviation before every transmission (+-45kHz)
		  SpiWriteRegister(0x72, 0x30);		//Ƶƫ30write 0x30 to the Frequency Deviation register 
		  //SET THE CONTENT OF THE PACKET
			//set the length of the payload to 8bytes	
		      //	SpiWriteRegister(0x3E, 8);		//length�������ݰ�����8 write 8 to the Transmit Packet Length register		
			

	RFWriteFIFO(8,&payloadTx[0]);//	RFWriteFIFO(8,&payloadTx[0]);//�������ݰ�����+�������ݵ�ַ	RFWriteFIFO(8,&payloadF[0]);
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
          //�������������ж�,�������BUTTON1
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
			RFSetTxMode();	      	  //����ģʽ    //write 0x09 to the Operating Function Control 1 register
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
			RFSetRxMode();				  //����ģʽ		//write 0x05 to the Operating Function Control 1 register				
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
			if( (ItStatus1 & 0x01) == 0x01 )    //CRC����
			{
			  // UART_Send_Str("CRC Error\n");
				#ifdef _FFD	  
				DisplayListChar(9,1,"CrcE");
				#endif
				jc(14,1,++ccc);
				//reset the RX FIFO
				//if(ccc=0xf)  RF_INIT();  //16��CRC���������4432
       	   		SpiWriteRegister(0x08, 0x02);											//write 0x02 to the Operating Function Control 2 register
        	    SpiWriteRegister(0x08, 0x00);											//write 0x00 to the Operating Function Control 2 register
				//blink all LEDs to show the error
		
				 delay_ms(5); //for(delay = 0; delay < 10000;delay++);
			
			}

			//packet received interrupt occured
			if( (ItStatus1 & 0x02) == 0x02 ) //�����Ч���ݰ������
			{
 				//Read the length of the received payload
				length = SpiReadRegister(0x4B);		//�ж��ٸ�����		//read the Received Packet Length register
				//check whether the received payload is not longer than the allocated buffer in the MCU
				if(length < 11)
				{
	             RX_LED = 0;
					//Get the reeived payload from the RX FIFO
					for(temp8=0;temp8 < length;temp8++)
					{
						payloadRx[temp8] = SpiReadRegister(0x7F);	//��������read the FIFO Access register
					}
			
					//check whether the acknowledgement packet received
					if( length == 4 ) 
	            	{
	            		if( memcmp(&payloadRx[0], "ACK", 3) == 0 ) //�Ƚ�s1��s2��ǰ3���ֽڵ�ascII��ֵ��
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
					   // DisplayDataListChar(0,0,&payloadRx[0]);//��ʾ���ջ�������//DisplayDataListChar(8,0,&payload[0]);    
	            		if( memcmp(&payloadRx[0], "BUTTON1", 7) == 0 )    //�Ƚ�s1��s2��ǰ7���ֽڵ�ascII��ֵ
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

						 // RFSetAckTxMode(); //����ACKӦ��
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

							//wait a bit for showing the LED a bit longer
							//delay_ms(5); //for(delay = 0; delay < 10000;delay++);
							//turn off the LED
							//TX_LED = 0; 	
							   //DisplayDataListChar(0,0,&payloadRx[0]);//��ʾ���ջ�������	 ///test
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
			RFSetRxMode();					  //����ģʽ			//write 0x05 to the Operating Function Control 1 register
		} 	
	}


 
}
/**********************************************************************************************/









	//���ý����ж��ȣ�������������������

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
//IA4432_Register((REG_WRITE | NominalCarrierFrequency0), 0x00); 	  //77//Ƶ�ʿ��� 

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
