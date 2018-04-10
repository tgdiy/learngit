#include "bee.h"
#include "delay.h"
 //#include "AT89X52.H"  
//#include <string.h>
/****************************************
 #ifndef _AT89X52_H 
　　#define _AT89X52_H
	     #ifndef _bee_H
　　#define _bee_H
　　#endif



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
void   Beesjia(unsigned char *p1, unsigned char *p) //繁琐
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


 BeeP0= 0xff;    //可以另外打开发光管，采集完后关掉，以节省电能
  delay_us(100);       //延时,50us以上稳定时间
a=p[1];
b=BeeP0; 
p[1]=b;  //本次采集值CaiJi

       
b=a^b;             //进出正负跳变，临时变量b

a=c=p[1];
a=((a<<4)|(c>>4)); //a=(((a&0x0f)<<4)|((c&0xf0)>>4));           //半字节交换
a=a&p[1];               //进出都触发,临时变量a
p[0]=a&b;               //本次二进制计数值//低字节=出跳变+进出都通，说明是蜜蜂进入,
		             //高字节=进跳变+进出都通，说明是蜜蜂出.


 }
/********************
/*******************************************************************
 unsigned char  BeesInOutC( unsigned char JiShu, unsigned char CaiJi)  
{
 unsigned char a,b,c;


 P0= 0xff;    //可以另外打开发光管，采集完后关掉，以节省电能
 //Delay100us();      //延时,50us以上稳定时间
a=CaiJi;
b=P0; 

//return (CaiJi);       //本次采集值CaiJi

b=a^CaiJi;             //进出正负跳变，临时变量b

a=c=CaiJi;
a=((a<<4)|(c>>4)); //a=(((a&0x0f)<<4)|((c&0xf0)>>4));           //半字节交换
a=a&CaiJi;               //进出都触发,临时变量a
JiShu=a&b;               //本次二进制计数值R7//低字节=出跳变+进出都通，说明是蜜蜂进入,
		             //高字节=进跳变+进出都通，说明是蜜蜂出.

  return (JiShu);
}
/********************
P0= 0xff;    //可以另外打开发光管，采集完后关掉，以节省电能
 //Delay100us();      //延时,50us以上稳定时间
 a=caiji;
 c=jcaiji=P0;         //本次采集值caiji

 a=b=a^c;      //进出正负跳变，临时变量b
 a=c=caiji;
a=(a&0x0f)<<4;   //半字节交换
c=(c&0xf0)>>4;
a=a|c;
 a=a&caiji;       //进出都触发,临时变量a
 jishu=a&b;         //本次二进制计数值R7//低字节=出跳变+进出都通，说明是蜜蜂进入,
		             //高字节=进跳变+进出都通，说明是蜜蜂出.

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
; // 汇编函数声明,20120312,用内联汇编的办法传递参数,
;//输入U16(上次p0上拉端口采集值R6+上次跳变值R7)(返回本次p0上拉采集值R6+本次跳变值R7)
;0xB4（10110100）用图加以说明。 
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
; 在此添加汇编代码，完成所要实现的转换功能xtern unsigned char BeesInOut( unsigned char val);  
;//必须在 main加入外部汇编函数声明


 BeesInOut:  ;U16(上次端口采集值R6+上次二进制计数值R7)(返回本次采集值R6+本次二进制计数值R7)
     
     
loop:   mov	p0,#0ffh    ;//可以另外打开发光管，采集完后关掉，以节省电能
    call    delay100us	;//延时,50us以上稳定时间
      mov  a,r6
		mov  r6,p0     ;//本次采集值R6
		xrl  a,r6   
      mov  R5,a    ;//进出正负跳变，临时变量R5

      mov a,r6
		swap a
		anl a,r6     ;//进出都触发,临时变量a

      anl a,r5     
		mov r7,a     ;//本次二进制计数值R7//低字节=出跳变+进出都通，说明是蜜蜂进入,
		             ;//高字节=进跳变+进出都通，说明是蜜蜂出.
      ret          ;//可以另外打开发光管，采集完后关掉






delay100us:   ;// 100us 延时:   ;误差 0us
		MOV		R0, #100 // 1
		DJNZ	R0, $ 		// 46X2=98个机器周期
		NOP			 		// 1

    RET
	
; END OF _BeesInOut

END
****************************************************

char code SST516[3] _at_ 0x003b;   //c51仿真器专用
#define  BeeData   P0       //数据口

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
 BeeData= 0xff;    //可以另外打开发光管，采集完后关掉，以节省电能
 //Delay100us();      //延时,50us以上稳定时间
 a=jishucaiji[1];
 c=jishucaiji[1]=BeeData;         //本次采集值caiji
 a=b=a^c;      //进出正负跳变，临时变量b
 a=jishucaiji[1];
a=(a&0x0f)<<4;   //半字节交换
c=(a&0xf0)>>4;
a=a|c;
 a=a&jishucaiji[1];       //进出都触发,临时变量a
 jishucaiji[0]=c&b;         //本次二进制计数值R7//低字节=出跳变+进出都通，说明是蜜蜂进入,
		             //高字节=进跳变+进出都通，说明是蜜蜂出.

}

*********************************
void Delay100us(void)
 {
	unsigned char a,b;
	for(b=171;b>0;b--)
		for(a=2;a>0;a--);
 }  
/*********************************/
