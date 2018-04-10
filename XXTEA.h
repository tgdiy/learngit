#ifndef __XXTEA_H__
#define __XXTEA_H__
//#include "AT89X52.H"



void XXTEA_Encrypt( unsigned char* buf, unsigned char* key );
void XXTEA_Decrpyt( unsigned char* buf, unsigned char* key );
void XXTEA_JiaMi(void);
void XXTEA_JieMi(void);

#define MX 			(((z>>5^y<<2)+(y>>3^z<<4))^((sum^y)+(k[p&3^e]^z)))  //(z>>5^y<<2)+(y>>3^z<<4)^(sum^y)+(k[p&3^e]^z)
#define DELTA 		0x9e3779b9
#define S_LOOPTIME  16    //5 //s_looptime其实控制的是加密的轮数
#define BLOCK_SIZE  32   //PAGE_SIZE,根据你所要加密的数据包长度修改此参数(单位:字节)
extern unsigned char  payloadRx[ ];
extern unsigned char XXTEA_key[ ];
#endif