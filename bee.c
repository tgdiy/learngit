#include "bee.h"
#include "delay.h"
 //#include "AT89X52.H"  
//#include <string.h>
/****************************************
 #ifndef _AT89X52_H 
����#define _AT89X52_H
	     #ifndef _bee_H
����#define _bee_H
����#endif



//void Delay100us(void);
void  BeesInOutC( unsigned char *p); 
unsigned long  BeesOut(unsigned long BeesOut, unsigned char *p);
unsigned long  BeesIn(unsigned long BeesIn, unsigned char *p);
void bee_test(void);

//unsigned char JiShuCaiJi[2]={0};
//unsigned long BeeOut,BeeIn;

#define  BeeP0  P0 
#endif
**********************************************/


unsigned char JiShuCaiJi[2]={0};
unsigned long BeeOut,BeeIn;

void bee_test(void)
{


 BeesInOutC( &JiShuCaiJi[0] );
 BeeOut=BeesOut( BeeOut, &JiShuCaiJi[0]);
 BeeIn=BeesIn( BeeIn, &JiShuCaiJi[0]);
  delay_us(100); 

}
/***************************************************/
unsigned long  BeesOut(unsigned long BeeOut, unsigned char *p)
 {  
unsigned char Out,Out1;
Out=p[0];
Out=(Out&0xf0)>>4;
Out1=Out&0x01;
BeeOut=BeeOut+Out1;
Out1=(Out>>1)&0x01;
BeeOut=BeeOut+Out1;

Out1=(Out>>2)&0x01;
BeeOut=BeeOut+Out1;

Out1=(Out>>3)&0x01;
BeeOut=BeeOut+Out1;
return (BeeOut);
 }
/***************************************************/
/***************************************************/
unsigned long  BeesIn(unsigned long BeeIn, unsigned char *p)
 {
unsigned char In,In1;
In=p[0];
In=(In&0x0f);
In1=In&0x01;
BeeIn=BeeIn+In1;

In1=(In>>1)&0x01;
BeeIn=BeeIn+In1;

In1=(In>>2)&0x01;
BeeIn=BeeIn+In1;

In1=(In>>3)&0x01;
BeeIn=BeeIn+In1;
return ( BeeIn);
 }
/***************************************************
void   Beesjia(unsigned char *p1, unsigned char *p) //����
 { 
 unsigned char a1,b1,c1,d1;
a1=p[0];
b1=p1[0];
a1=(a1&0xf0)>>4;
p1[0]=b1+a1;

c1=p[0];
d1=p1[1];
c1=(c1&0x0f);
p1[1]=d1+c1;
 }
*****************************/

void  BeesInOutC( unsigned char *p)  
 {
 unsigned char a,b,c;


 BeeP0= 0xff;    //��������򿪷���ܣ��ɼ����ص����Խ�ʡ����
  delay_us(100);       //��ʱ,50us�����ȶ�ʱ��
a=p[1];
b=BeeP0; 
p[1]=b;  //���βɼ�ֵCaiJi

       
b=a^b;             //�����������䣬��ʱ����b

a=c=p[1];
a=((a<<4)|(c>>4)); //a=(((a&0x0f)<<4)|((c&0xf0)>>4));           //���ֽڽ���
a=a&p[1];               //����������,��ʱ����a
p[0]=a&b;               //���ζ����Ƽ���ֵ//���ֽ�=������+������ͨ��˵�����۷����,
		             //���ֽ�=������+������ͨ��˵�����۷��.


 }
/********************
/*******************************************************************
 unsigned char  BeesInOutC( unsigned char JiShu, unsigned char CaiJi)  
{
 unsigned char a,b,c;


 P0= 0xff;    //��������򿪷���ܣ��ɼ����ص����Խ�ʡ����
 //Delay100us();      //��ʱ,50us�����ȶ�ʱ��
a=CaiJi;
b=P0; 

//return (CaiJi);       //���βɼ�ֵCaiJi

b=a^CaiJi;             //�����������䣬��ʱ����b

a=c=CaiJi;
a=((a<<4)|(c>>4)); //a=(((a&0x0f)<<4)|((c&0xf0)>>4));           //���ֽڽ���
a=a&CaiJi;               //����������,��ʱ����a
JiShu=a&b;               //���ζ����Ƽ���ֵR7//���ֽ�=������+������ͨ��˵�����۷����,
		             //���ֽ�=������+������ͨ��˵�����۷��.

  return (JiShu);
}
/********************
P0= 0xff;    //��������򿪷���ܣ��ɼ����ص����Խ�ʡ����
 //Delay100us();      //��ʱ,50us�����ȶ�ʱ��
 a=caiji;
 c=jcaiji=P0;         //���βɼ�ֵcaiji

 a=b=a^c;      //�����������䣬��ʱ����b
 a=c=caiji;
a=(a&0x0f)<<4;   //���ֽڽ���
c=(c&0xf0)>>4;
a=a|c;
 a=a&caiji;       //����������,��ʱ����a
 jishu=a&b;         //���ζ����Ƽ���ֵR7//���ֽ�=������+������ͨ��˵�����۷����,
		             //���ֽ�=������+������ͨ��˵�����۷��.

/***************************************************
; COMPILER INVOKED BY:
;        C:\KeilC51\C51\BIN\C51.EXE convert.c BROWSE DEBUG OBJECTEXTEND SRC(.\convert.SRC)

NAME	CONVERT

?PR?_BeesInOut?CONVERT SEGMENT CODE 
?DT?_BeesInOut?CONVERT SEGMENT DATA OVERLAYABLE 
	PUBLIC	_BeesInOut

	RSEG  ?DT?_BeesInOut?CONVERT
?_BeesInOut?BYTE:
        val?040:   DS   1
; 
; unsigned U16 BeesInOut( U16 val) //
; // ��ຯ������,20120312,���������İ취���ݲ���,
;//����U16(�ϴ�p0�����˿ڲɼ�ֵR6+�ϴ�����ֵR7)(���ر���p0�����ɼ�ֵR6+��������ֵR7)
;0xB4��10110100����ͼ����˵���� 
;Big Endian
 ;  msb=R6                           lsb=R7
;   ---------------------------------------------->
;   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
;   | 1 | 0 | 1 | 1   |   0 | 1 | 0 | 0 |
;   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
;***************************************************************************

	RSEG  ?PR?_BeesInOut?CONVERT
_BeesInOut:
			; SOURCE LINE # 2
	MOV  	val?040,R7
; {
			; SOURCE LINE # 3
; 
; }			; SOURCE LINE # 5
/********************************************************
   ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

   ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;	
;***********
; �ڴ���ӻ����룬�����Ҫʵ�ֵ�ת�����ܡextern unsigned char BeesInOut( unsigned char val);  
;//������ main�����ⲿ��ຯ������


 BeesInOut:  ;U16(�ϴζ˿ڲɼ�ֵR6+�ϴζ����Ƽ���ֵR7)(���ر��βɼ�ֵR6+���ζ����Ƽ���ֵR7)
     
     
loop:   mov	p0,#0ffh    ;//��������򿪷���ܣ��ɼ����ص����Խ�ʡ����
    call    delay100us	;//��ʱ,50us�����ȶ�ʱ��
      mov  a,r6
		mov  r6,p0     ;//���βɼ�ֵR6
		xrl  a,r6   
      mov  R5,a    ;//�����������䣬��ʱ����R5

      mov a,r6
		swap a
		anl a,r6     ;//����������,��ʱ����a

      anl a,r5     
		mov r7,a     ;//���ζ����Ƽ���ֵR7//���ֽ�=������+������ͨ��˵�����۷����,
		             ;//���ֽ�=������+������ͨ��˵�����۷��.
      ret          ;//��������򿪷���ܣ��ɼ����ص�






delay100us:   ;// 100us ��ʱ:   ;��� 0us
		MOV		R0, #100 // 1
		DJNZ	R0, $ 		// 46X2=98����������
		NOP			 		// 1

    RET
	
; END OF _BeesInOut

END
****************************************************

char code SST516[3] _at_ 0x003b;   //c51������ר��
#define  BeeData   P0       //���ݿ�

void Delay100us(void);
void BeesInOutC( unsigned char *jishucaiji); 
unsigned char jishucaiji[2]={0};

void main(void)
{

loop:
 BeesInOutC(jishucaiji[0]);

goto loop;
}


void BeesInOutC( unsigned char *jishucaiji)  
{
 unsigned char a,b,c;
 BeeData= 0xff;    //��������򿪷���ܣ��ɼ����ص����Խ�ʡ����
 //Delay100us();      //��ʱ,50us�����ȶ�ʱ��
 a=jishucaiji[1];
 c=jishucaiji[1]=BeeData;         //���βɼ�ֵcaiji
 a=b=a^c;      //�����������䣬��ʱ����b
 a=jishucaiji[1];
a=(a&0x0f)<<4;   //���ֽڽ���
c=(a&0xf0)>>4;
a=a|c;
 a=a&jishucaiji[1];       //����������,��ʱ����a
 jishucaiji[0]=c&b;         //���ζ����Ƽ���ֵR7//���ֽ�=������+������ͨ��˵�����۷����,
		             //���ֽ�=������+������ͨ��˵�����۷��.

}

*********************************
void Delay100us(void)
 {
	unsigned char a,b;
	for(b=171;b>0;b--)
		for(a=2;a>0;a--);
 }  
/*********************************/
