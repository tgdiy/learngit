/******************************************************************************/

/******************************************************************************/
//define main board header file
#ifndef __HEADERFILE_H__
#define __HEADERFILE_H__
 #define _C51 1		   //STM32，C51，LGT，STM8单片机选择
 #define n_DEBUG 1	  //调试状态1
 #define _RFD 1		   //FFD主节点		RFD分节点
/******************************************************************************/
 #ifdef _C51	 //Keil-C51-V4.23.00.0
#include "AT89X52.H"
			  
typedef unsigned char U8;
typedef unsigned int U16;
typedef unsigned long U32;

typedef signed char S8;
typedef signed int S16;
typedef signed long S32;

/*********************************///1602硬件连接
#define LCM_Data P2
sbit LCM_RS = P3^4;
sbit LCM_RW = P3^5;  //1602定义引脚
sbit LCM_E  = P3^6;                                                                                           
sbit LCM_B  = P2^7; 
//sbit Busy = 0xD1;   //用于检测LCM状态字中的Busy标识#define Busy 1 //

sbit KeyB = P3^7; //key与led4复用

sbit DATATX_LED = P1^7; 
sbit TX_LED = P3^2;
sbit RX_LED = P3^3;
sbit LED4 = P3^7;//key与led4复用
 /*******************************///4432硬件连接
sbit si4432_SDN =P1^7;      //20120612改插座//sbit SDN =P1^0;0开启4432
sbit si4432_NIRQ =P1^6;     //sbit si4432_NIRQ =P1^1;中断信号0
sbit si4432_NSEL = P1^5; //sbit si4432_NSEL = P1^2; 读写模式控制/串行片选
sbit si4432_SCK  = P1^4;    //sbit si4432_SCK  = P1^3;时钟
sbit si4432_MOSI = P1^3;    //sbit MOSI = P1^4;写数据
sbit si4432_MISO = P1^2;    //sbit MISO = P1^5;读数据 
//GIPO0
//GND
/*********************************///1602硬件电平
#define ON  0
#define OFF 1
#define LCM_RS(a)	if (a)	\
					LCM_RS = 1;\
					else		\
					LCM_RS = 0
#define LCM_RW(a)	if (a)	\
					LCM_RW = 1;\
					else		\
					LCM_RW = 0
#define LCM_E(a)	if (a)	\
					LCM_E = 1;\
					else		\
					LCM_E = 0
/*********************************///4432硬件电平
#define si4432_SDN(a)	if (a)	\
					si4432_SDN = 1;\
					else		\
					si4432_SDN = 0
#define si4432_NIRQ(a)	if (a)	\
					si4432_NIRQ = 1;\
					else		\
					si4432_NIRQ = 0
#define si4432_NSEL(a)	if (a)	\
					si4432_NSEL = 1;\
					else		\
					si4432_NSEL = 0
#define si4432_SCK(a)	if (a)	\
					si4432_SCK = 1;\
					else		\
					si4432_SCK = 0
#define si4432_MOSI(a)	if (a)	\
					si4432_MOSI = 1;\
					else		\
					si4432_MOSI = 0
#define si4432_MISO(a)	if (a)	\
					si4432_MISO = 1;\
					else		\
					si4432_MISO = 0
/*********************************/

#endif
/******************************************************************************/
 #ifdef _STM32
#include "stm32f10x.h"
#define ON  0
#define OFF 1

#define LED1(a)	if (a)	\
					GPIO_SetBits(GPIOC,GPIO_Pin_3);\
					else		\
					GPIO_ResetBits(GPIOC,GPIO_Pin_3)

#define LED2(a)	if (a)	\
					GPIO_SetBits(GPIOC,GPIO_Pin_4);\
					else		\
					GPIO_ResetBits(GPIOC,GPIO_Pin_4)

#define LED3(a)	if (a)	\
					GPIO_SetBits(GPIOC,GPIO_Pin_5);\
					else		\
					GPIO_ResetBits(GPIOC,GPIO_Pin_5)
#endif
/******************************************************************************/
 #ifdef _LGT
#include <io.h>                  // GCC的标准io.h
#include <iolgt8f0xa.h>          // LGT的头文件
//#define 
#endif
/******************************************************************************/	
#endif
/***************************************The End*************************************************/