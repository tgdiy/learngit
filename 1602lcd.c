#include "1602lcd.h"
#include "delay.h"

//写数据
void WriteDataLCM(unsigned char WDLCM)
{
		ReadStatusLCM(); //检测忙
		LCM_Data = WDLCM;
		LCM_RS (OFF);   //LCM_RS = 1;
		LCM_RW (ON);	//LCM_RW = 0;
		LCM_E (ON); //若晶振速度太高可以在这后加小的延时
	   delay_ms(5); //延时
		LCM_E (OFF);
		LCM_E (OFF); 
		LCM_E (OFF); 
       delay_ms(5);  
}

//写命令
void WriteCommandLCM(unsigned char WCLCM,BuysC) //BuysC为0时忽略忙检测
{
		if (BuysC) 
		ReadStatusLCM(); //根据需要检测忙
		LCM_Data = WCLCM;
		delay_ms(5);  
		LCM_RS (ON);
      delay_ms(5);  
		LCM_RW (ON);
		delay_ms(5);  
 	   LCM_E (ON);
      delay_ms(5);   
		LCM_E (OFF); 
		LCM_E (OFF);
		LCM_E (OFF); 
      delay_ms(5);  
}

//读指令判忙
unsigned char ReadStatusLCM(void)
{
		//Busy=1;
		LCM_RW (OFF);
		LCM_RS (ON);
		LCM_E (OFF);
	while (LCM_B == OFF );//	while (LCM_B & Busy); //检测忙信号
		return(LCM_B);	  //此处需要调试
		LCM_E (ON);
        LCM_RW (ON);
}


//初始化
void LCD_Init(void) 
{
		//WriteCommandLCM(0x3C,1);
		delay_ms(5);  
	    //WriteCommandLCM(0x3C,1); 
		delay_ms(5);   
		WriteCommandLCM(0x3C,1);
		delay_ms(5);  
		WriteCommandLCM(0x01,1);
		delay_ms(5);  
		WriteCommandLCM(0x06,1);
		delay_ms(5);   
		WriteCommandLCM(0x0F,1); //C
		delay_ms(5);   
}
//清屏
void LCD_cls(void)
{
		WriteCommandLCM(0x01,0);
		delay_ms(5);  
}
//任意地址写
void DisplayOneChar(unsigned char X, unsigned char Y, unsigned char DData)
{
		//Y &= 0x3;
		//X &= 0xF; //限制X不能大于16，Y不能大于1  //3
   

		 if(Y==0)
		 {
		 X =X+0x80; //算出指令码
		WriteCommandLCM(X, 0); //这里不检测忙信号，发送地址码
		WriteDataLCM(DData);	 
		 }
		 else 
		 if(Y==1) 
		 {
		 X =X+0xC0; //算出指令码
		WriteCommandLCM(X, 0); //这里不检测忙信号，发送地址码
		WriteDataLCM(DData);		 
		 }
		 else 
		 if(Y==2) 
		 {
		 X =X+0x94; //算出指令码
		WriteCommandLCM(X, 0); //这里不检测忙信号，发送地址码
		WriteDataLCM(DData);
		 }
		 else
		 if(Y==3) 
		 {
		 X =X+0xD4; //算出指令码
		WriteCommandLCM(X, 0); //这里不检测忙信号，发送地址码
		WriteDataLCM(DData);
		 }

}

//写字符串
//使用方法   DisplayListChar(0,0,"No GPS connect..");
void DisplayListChar(unsigned char X, unsigned char Y, unsigned char code *DData)
{                                                           //显示ROM区字符unsigned char code *DData)
  	unsigned char j=0;   //ListLength,
    //	LCD_cls();
  	//ListLength = strlen(DData);	   //strlen是测字符串的长度的说
  	//Y &= 0x1;
  	//X &= 0xF; //限制X不能大于15，Y不能大于1
    //if (X <= 0xF) //X坐标应小于0xF
      //{ 
		for( j=0;j<16;j++ ) 
    	 if (NULL!=DData[j]) 
            {
             DisplayOneChar(X, Y, DData[j]); //显示单个字符
             X++; } 
	 else {break; } //到字符串结尾，退出
				  
				 
           
       //}
}
/*******************************
//延时
void Delay5Ms(void)  //误差 0us   
{
    unsigned char a,b;
    for(b=185;b>0;b--)
        for(a=12;a>0;a--);
}
/*******************************/
/*******************************
//延时
void Delay400Ms(void)
{
    unsigned char a,b,c;
    for(c=51;c>0;c--)
        for(b=224;b>0;b--)
            for(a=16;a>0;a--);
}
/******************************/

//变数 待改进
void bianshu(unsigned char q, unsigned char w, unsigned char g)
   {
	switch(g)
	{
		case 0   :DisplayListChar(q,w,"0");break;	
		case 1   :DisplayListChar(q,w,"1");break;	
		case 2   :DisplayListChar(q,w,"2");break;	
		case 3   :DisplayListChar(q,w,"3");break;	
		case 4   :DisplayListChar(q,w,"4");break;	
		case 5   :DisplayListChar(q,w,"5");break;	
		case 6   :DisplayListChar(q,w,"6");break;	
		case 7   :DisplayListChar(q,w,"7");break;	
		case 8   :DisplayListChar(q,w,"8");break;	
		case 9   :DisplayListChar(q,w,"9");break;
		case 0x0a   :DisplayListChar(q,w,"A");break;	
		case 0x0b   :DisplayListChar(q,w,"B");break;	
		case 0x0c   :DisplayListChar(q,w,"C");break;	
		case 0x0d   :DisplayListChar(q,w,"D");break;	
		case 0x0e   :DisplayListChar(q,w,"E");break;			
		case 0x0f   :DisplayListChar(q,w,"F");break;		    
	}
   }
/*********************************************************************************/
void jc(q,w,d)
{

  unsigned int a;//高4位变为十位数
  a = d;
  a = a & 0xf0;
  a = a >> 4;
  bianshu(q,w,a);

  a = d;
  a = a & 0x0f;
  bianshu(q+1,w,a);//低4位变为个位数

}/*********************************************************************************/
//写RAM字符串
//使用方法   DisplayDataListChar(0,0,&hhhhhhh[]);
void DisplayDataListChar(unsigned char X, unsigned char Y,  unsigned char data *(PData))
{                                                           //显示RAM区字符//
  	unsigned char j=0;   //ListLength,
    //	LCD_cls();
  	//ListLength = strlen(DData);	   //strlen是测字符串的长度的说
  	//Y &= 0x1;
  	//X &= 0xF; //限制X不能大于15，Y不能大于1
    //if (X <= 0xF) //X坐标应小于0xF
      //{ 
		for( j=0;j<16;j++ ) 
    	 if (NULL!=PData[j]) 
            {
             DisplayOneChar(X, Y, PData[j]); //显示单个字符
             X++; } 
	 else {break; } //到字符串结尾，退出
				  
				 
           
       //}
}
/*********************************************************************************/


