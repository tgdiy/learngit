#include "delay.h"
#include<intrins.h> //ʹ��_nop_()�ؼ�

//ʱ��Ƶ�ʣ���Ҫ����  //12T-c51-12mhz��Ƭ�� 

void delay_us(unsigned int us)	  //5US!12T-c51-12mhz��Ƭ��; 
{
  while (--us);
 
}
/***********************************************************
void delay_us(unsigned int us)	  //1US!1T-c51-12mhz 
{
  do
  {
    _nop_();
    _nop_();

  } while (--us);
}
/***********************************************************/
void delay_ms(unsigned int ms)
{
  do
  {
    delay_us(100);	  //delay_us(250);
    delay_us(100);
    delay_us(100);
    delay_us(100);
  } while (--ms);
}
/**************************************************
//5us��ʱ����
void delay_5us(void)   //��� 0us
{
    _nop_();  //if Keil,require use intrins.h

}


//N5us��ʱ����
void delay_n5us(unsigned char n)     //���1280us  
   {
    unsigned int i=0;
    for(i=0;i<n;i++)
       delay_5us();
   }
  
//1ms��ʱ����
void delay_1ms(void)   //��� 0us
{
    unsigned char a,b;
    for(b=199;b>0;b--)
        for(a=1;a>0;a--);
}

  
//N ms��ʱ����
void delay_ms(unsigned char n)  //���255ms     
   {
    unsigned int i=0;
    for(i=0;i<n;i++)
       delay_1ms();
   }

 
/*******************************************************
 void delay_us(uint16_t us)
{
  do
  {
    _nop_();
  } while (--us);
}

void delay_ms(uint16_t ms)
{
  do
  {
    delay_us(250);
    delay_us(250);
    delay_us(250);
    delay_us(250);
  } while (--ms);
}




/*********************************************************************************************
unsigned char _DELAY_I=0,_DELAY_J=0,_DELAY_K=0;		//.h�ļ���ֻ�����������ܸ�ֵ
/*********************************************************************************************/