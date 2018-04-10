#ifndef __1602lcd_H__
#define __1602lcd_H__

#include "headerfile.h"	  //
#include <string.h>
/***************************************
#define LCM_Data P2
sbit LCM_RS = P3^4;
sbit LCM_RW = P3^5;  //1602定义引脚
sbit LCM_E  = P3^6; 
                                                                                           
sbit LCM_B  = P2^7; 
sbit Busy = 0xD1;   //用于检测LCM状态字中的Busy标识
/***************************************/

void WriteDataLCM(unsigned char WDLCM);
void WriteCommandLCM(unsigned char WCLCM,BuysC);
unsigned char ReadDataLCM(void);
unsigned char ReadStatusLCM(void);
void LCD_Init(void);
void LCD_cls(void);
void DisplayOneChar(unsigned char X, unsigned char Y, unsigned char DData);
void DisplayListChar(unsigned char X, unsigned char Y, unsigned char code *DData);
void bianshu(unsigned char q, unsigned char w, unsigned char g);
//void Delay5Ms(void);
//void Delay400Ms(void);
void DisplayDataListChar(unsigned char X, unsigned char Y, unsigned char data *(PData));

void jc(q,w,d);
/****************************
void delay_us(unsigned char us);
void delay_ms(unsigned char ms);
************************/
#endif /* __1602lcd_H */
