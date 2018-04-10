#include "XXTEA.h"

#include <string.h>






/******************************************************************************************************************
  TEA���ܽ����㷨
  TEA(Tiny Encryption Algorithm)��һ�ּ򵥸�Ч�ļ����㷨���Լ��ܽ����ٶȿ죬ʵ�ּ����ơ�
  �㷨�ܼ򵥣�TEA�㷨ÿһ�ο��Բ���64-bit(8-byte)������128-bit(16-byte)��Ϊkey���㷨���õ�������ʽ��
  �Ƽ��ĵ���������64�֣�����32�֡�
************************************************************************************************
void XXTEA_Encrypt( unsigned char* buf, unsigned char* key );
void XXTEA_Decrpyt( unsigned char* buf, unsigned char* key );
void XXTEA_JiaMi(void);
void XXTEA_JieMi(void);


#define MX 			(((z>>5^y<<2)+(y>>3^z<<4))^((sum^y)+(k[p&3^e]^z)))
#define DELTA 		0x9e3779b9
#define S_LOOPTIME  16    //s_looptime��ʵ���Ƶ��Ǽ��ܵ�����
#define BLOCK_SIZE  32   //PAGE_SIZE,��������Ҫ���ܵ����ݰ������޸Ĵ˲���(��λ:�ֽ�)(1�ֽ�=2byte)
extern unsigned char  payloadRx[ ];
extern unsigned char XXTEA_key[ ];
/******************/


void XXTEA_Encrypt( unsigned char* buf, unsigned char* key )
{
	unsigned char  n=BLOCK_SIZE/4;
	unsigned long *v=(unsigned long *)buf;
	unsigned long *k=(unsigned long *)key;
	unsigned long z = v[n - 1],y = v[0],sum = 0,e ;
	unsigned char p,q ;
	// Coding Part 
	
	q =S_LOOPTIME + 52 / n ;   //�ε���
	while ( q-- > 0 )
	{
		sum += DELTA ;
		e = (sum >> 2) & 3;
		for ( p = 0 ; p < n - 1 ; p++ )	y = v[p + 1],	z = v[p] += MX;
   	y = v[0] ;
		z = v[n - 1] += MX;
	}
}

void XXTEA_Decrpyt( unsigned char* buf, unsigned char* key )
{
	unsigned char n=BLOCK_SIZE/4;
	unsigned long *v=(unsigned long *)buf;
	unsigned long *k=(unsigned long *)key;
	unsigned long z = v[n - 1],y = v[0],sum = 0,e ;
	unsigned char p,q ;
	
	//Decoding Part...
	q =S_LOOPTIME + 52 / n ;   //�ε���
	sum = q * DELTA ;
	while ( sum != 0 )
	{
		e = (sum >> 2) & 3;
		for ( p = n - 1 ; p > 0 ; p-- )	z = v[p - 1], y = v[p] -= MX;
		z = v[n - 1] ;
		y = v[0] -= MX;
		sum -= DELTA ;
	}
}
/************************/


void XXTEA_JiaMi(void)
{ 
 //	unsigned char dat[16]="0123456789ABCDEF";
//	unsigned char XXTEA_key[16]="0123456789ABCDEF";
	//	memcpy(XXTEA_key,"0123456789ABCDEF";,16);//������֮ǰ��Ҫ���ú���Կ������ֻ��������Կ��DEMO��

	XXTEA_Encrypt(payloadRx,XXTEA_key);      //TEA���ܣ�����payload����������ݾ��Ǽ��ܺ�����ݡ�


}
/*****************************/



void XXTEA_JieMi(void)
{  
  //unsigned char dat1[16]="0123456789ABCDEF";  // unsigned char payload1[16]="0123456789ABCDEF";  //
//	unsigned char XXTEA_key[16]="0123456789ABCDEF";
//	memcpy(XXTEA_key,"0123456789abcdef",16);//������֮ǰ��Ҫ���ú���Կ������ֻ��������Կ��DEMO��

	XXTEA_Decrpyt(payloadRx,XXTEA_key);      //�������ݴ����payload���棬��TEA���ܾ��ܵõ�֮ǰ�����ġ�

}
/*****************************/

