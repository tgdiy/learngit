#ifndef __LED_H__
#define	__LED_H__

#include "headerfile.h"	  //
/***************************
  sbit DATATX_LED = P1^7; 
  sbit TX_LED = P3^2;
  sbit RX_LED = P3^3;
  sbit LED4 = P3^7;//key”Îled4∏¥”√
*************************/
void LED_ALLOFF(void);

/* the macro definition to trigger the led on or off 
 * 1 - off
 - 0 - on
 */
#define ON  0
#define OFF 1

#define DATATX_LED(a)	if (a)	\
					DATATX_LED = 1;\
					else		\
					DATATX_LED = 0

#define TX_LED(a)	if (a)	\
					TX_LED = 1;\
					else		\
					TX_LED = 0

#define RX_LED(a)	if (a)	\
					RX_LED = 1;\
					else		\
					RX_LED = 0

#define LED4(a)	if (a)	\
					LED4 = 1;\
					else		\
					LED4 = 0



#endif /* __LED_H */
