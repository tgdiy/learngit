#ifndef __BEE_H__
#define __BEE_H__
#include "AT89X52.H"
//#include <string.h>


void bee_test(void);

//void Delay100us(void);
void  BeesInOutC( unsigned char *p); 
unsigned long  BeesOut(unsigned long BeesOut, unsigned char *p);
unsigned long  BeesIn(unsigned long BeesIn, unsigned char *p);

//unsigned char JiShuCaiJi[2]={0};
//unsigned long BeeOut,BeeIn;
 
#define  BeeP0  P0 
#endif