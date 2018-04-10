#ifndef __KEY_H__
#define	__KEY_H__

#include "headerfile.h"	  //
//#include "string.h"


 /*sbit Key0 = P0^0; 
 sbit Key1 = P0^1;
 sbit Key2 = P0^2; 
 sbit Key3 = P0^3; 
 sbit Key4 = P0^4; 
 sbit Key5 = P0^5; 
 sbit Key6 = P0^6; 
 sbit Key7 = P0^7; ///未用
 sbit KeyB = P3^7; //key与led4复用

 /*******
 *按键按下标置
 KEY_ON 0
 KEY_OFF 1
 ********/
#define KEY_ON	0
#define KEY_OFF	1

void DelayX(U8);
U8 Key_Scan(register);

#endif /* __LED_H */
