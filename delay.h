
#ifndef _DELAY_H_
#define _DELAY_H_

void delay_us(unsigned int us);

/* Function for looping "delay_us"
 * 
 */
void delay_ms(unsigned int ms);

#define delay_s(x)  delay_ms((unsigned int)1000 * (unsigned int)x)

/*********************************************************************************************
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _F_CPU_
#define _F_CPU_
//#define F_CPU_1000000
//#define F_CPU_1843200
//#define F_CPU_2000000
//#define F_CPU_3686400
//#define F_CPU_4000000
//#define F_CPU_7372800
//#define F_CPU_8000000
//#define F_CPU_11059200
#define F_CPU_12000000
//#define F_CPU_14745600
//#define F_CPU_16000000
//#define F_CPU_18432000
//#define F_CPU_20000000
#endif
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//XT精确延时函数
#ifndef _DELAY_XT_H_
#define _DELAY_XT_H_
#include <intrins.h>
//延时专用全局变量
extern unsigned char _DELAY_I,_DELAY_J,_DELAY_K; //.h文件中只能声明，不能赋值,//多次使用占用代码空间
//精确延时1T秒
#define _delay_1T()            {_nop_();}
//精确延时2xT秒，x范围1-255
#define _delay_2xT(x)         {for(_DELAY_I=x;_DELAY_I>0;_DELAY_I--);}
//精确延时10xT秒，xx范围1-255
#define _delay_10xT(x)        {for(_DELAY_I=x;_DELAY_I>0;_DELAY_I--)\
                                    {for(_DELAY_J=3;_DELAY_J>0;_DELAY_J--);};}
//精确延时100xT秒，xx范围1-255
#define _delay_100xT(x)       {for(_DELAY_I=x;_DELAY_I>0;_DELAY_I--)\
                                    {for(_DELAY_J=48;_DELAY_J>0;_DELAY_J--);};}
//精确延时xKT秒，x范围1-255
#define _delay_xKT(x)            {for(_DELAY_I=x;_DELAY_I>0;_DELAY_I--)\
                                    {for(_DELAY_J=2;_DELAY_J>0;_DELAY_J--)\
                                        {for(_DELAY_K=248;_DELAY_K>0;_DELAY_K--);};};}
//精确延时10xKT秒，xx范围1-255
#define _delay_10xKT(x)        {for(_DELAY_I=x;_DELAY_I>0;_DELAY_I--)\
                                    {for(_DELAY_J=20;_DELAY_J>0;_DELAY_J--)\
                                        {for(_DELAY_K=248;_DELAY_K>0;_DELAY_K--);};};}
//精确延时100xKT秒，xx范围1-255
#define _delay_100xKT(x)       {for(_DELAY_I=x;_DELAY_I>0;_DELAY_I--)\
                                    {for(_DELAY_J=200;_DELAY_J>0;_DELAY_J--)\
                                        {for(_DELAY_K=248;_DELAY_K>0;_DELAY_K--);};};}
#endif
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//uS,mS延时函数自动设置
#ifdef F_CPU_11059200  //F=11.0592MHz,T=1.08507uS
#define _delay_1us() _delay_1T()
#define _delay_10us() {_delay_2xT(4);_delay_1T();}
#define _delay_100us() _delay_2xT(46)
#define _delay_1ms() _delay_10xT(92)
#endif

#ifdef F_CPU_12000000  //F=12.0000MHz,T=1.00000uS
#define _delay_1us() _delay_1T()
#define _delay_10us() _delay_2xT(5)
#define _delay_100us() _delay_2xT(50)
#define _delay_1ms() _delay_xKT(1)
#define _delay_5ms() _delay_xKT(5)
#define _delay_10ms() _delay_xKT(10)
#define _delay_20ms() _delay_xKT(20)
#define _delay_50ms() _delay_xKT(50)
#endif

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////





////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*********************************************************************************************/
#endif
