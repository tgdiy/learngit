#include "uart.h"

#include <string.h>

//#include"public.h"
/**************************************************************************
 - ����������51��Ƭ���Ĵ��ڳ�ʼ��
 - ����ģ�飺STC51���ڲ���
 - �������ԣ��ⲿ��ʹ�û�ʹ��
 - ����˵������
 - ����˵������
 **************************************************************************/

void UART_Init()     
{
 PCON|=0x80; //PCON�����λSMOD=1ʱ�����ʼӱ� 
 TMOD=0x20;  //ʱ��1Ϊ��ʽ2 ��ֵ�Զ�װ�� ����������
 TH1=0xf3;   //��ʱ����ʼΪTH1=0xfa; TL1=0xfa;������?1.0592MHz��������ʵ�ʲ����Ĳ�����Ϊ9600bps//
 TL1=0xf3;	 //// TH1 = 0xF3; TL1 = TH1;������Ϊ12MHz��ʵ�ʲ����Ĳ�����Ϊ4807bps//
 SCON=0x50;	 //��������Ϊ��ʽ1,REN=1,��������
 TR1=1;      //������ʱ��1
 ES=1;       //ʹ�ܴ��ڽ����жϣ�
 EA=1;       //�������ж�
}

/**************************************************************************
 - ����������51��Ƭ���Ĵ����жϴ�������
 - ����ģ�飺STC51���ڲ���
 - �������ԣ��ⲿ��ʹ�û�ʹ��(�ڴ��жϺ����г����������Ӵ����յ�������)
 - ����˵������
 - ����˵������
 **************************************************************************/

void sio_int() interrupt 4 using 3   //�����жϺ���
{
 ES=0;
  //�����жϴ���
  /*
 if(RI)
 {
  if(SBUF!=0x08)  //������յ������˸�(ASCII��Ϊ0x08)
   cmd_buf[counter++]=SBUF;
  else
   counter--;
  RI=0;
 }
 if(SBUF==0x0d)
 {
  cmd_buf[counter-1]=0;
  counter=0;
  flag=1;
 }
 */
 ES=1;
}

/**************************************************************************
 - ����������51��Ƭ���Ĵ��ڷ����ֽڵĺ���
 - ����ģ�飺STC51���ڲ���
 - �������ԣ��ⲿ��ʹ�û�ʹ��
 - ����˵����mydata:Ҫ���͵�һ���ֽ�
 - ����˵������
 - ע������һ���ֽڣ��Ǵ��ڷ��͵Ļ�������
 **************************************************************************/

void UART_Send_Byte(unsigned char mydata)	
{
 ES=0;
 TI=0;
 SBUF=mydata;
 while(!TI);
 TI=0;
 ES=1;
}

/**************************************************************************
 - ����������51��Ƭ���Ĵ��ڷ���0d 0a �����س�����
 - ����ģ�飺STC51���ڲ���
 - �������ԣ��ⲿ��ʹ�û�ʹ��
 - ����˵������
 - ����˵������
 - ע���˺������Ƿ���0d 0a�������ֽڣ��ڡ������նˡ��ϻ��лس����е�Ч��
 **************************************************************************/

void UART_Send_Enter()
{
 UART_Send_Byte(0x0d);
 UART_Send_Byte(0x0a);
}

/**************************************************************************
 - ����������51��Ƭ���Ĵ��ڷ����ַ���
 - ����ģ�飺STC51���ڲ���
 - �������ԣ��ⲿ��ʹ�û�ʹ��
 - ����˵����s:ָ���ַ�����ָ��
 - ����˵������
 - ע��������ַ�������'\n'����ᷢ��һ���س�����
 **************************************************************************/

void UART_Send_Str(char *s)
{
 int len=strlen(s)-1;
 int i;
 for(i=0;i<len;i++)
 UART_Send_Byte(s[i]);
 if(s[i]=='\n') 
 {
  UART_Send_Enter();
 }
 else
 {
  UART_Send_Byte(s[i]);
 }
}

/**************************************************************************
 - ����������51��Ƭ���Ĵ��ڷ�����ֵ
 - ����ģ�飺STC51���ڲ���
 - �������ԣ��ⲿ��ʹ�û�ʹ��
 - ����˵����dat:Ҫ���͵���ֵ
 - ����˵������
 - ע�������лὫ��ֵתΪ��Ӧ���ַ��������ͳ�ȥ������ 4567 תΪ "4567" 
 **************************************************************************/

/*void UART_Put_Num(unsigned long dat)
{
 char temp[20];
 u32tostr(dat,temp);
 UART_Send_Str(temp);
} */

/**************************************************************************
 - ����������51��Ƭ���Ĵ��ڷ��͵�����Ϣ
 - ����ģ�飺STC51���ڲ���
 - �������ԣ��ⲿ��ʹ�û�ʹ��
 - ����˵����inf:ָ����ʾ��Ϣ�ַ�����ָ��
             dat:һ����ֵ��ǰ�����ʾ��Ϣ������˵�������ֵ������
 - ����˵������
 - ���������
 **************************************************************************/

/*void UART_Put_Inf(char *inf,unsigned long dat)
{
 UART_Send_Str(inf);
 UART_Put_Num(dat);
 UART_Send_Str("\n");  
}

/*
void binary(unsigned char dat)
{
 unsigned char i;
 unsigned char a[17];
 for(i=0;i<8;i++)
 {
  a[i]=((dat<<i)&0x80)?'o':' ';
 }
 a[i]=0;
 for(i=0;i<strlen(a);i++)
 {
  UART_Send_Byte(a[i]);
  UART_Send_Byte(' ');
 }
}
*/