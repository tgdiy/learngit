/********************(C) SUN 2012 嵌入式开发工作室 ********************
 * 文件名  ：Key.c
 * 描述    ：Key 应用函数库
 *          
 * 实验平台：C51-600开发板
 * 硬件连接：-----------------        
 *          |   P0.0 - Key0     |
 *          |   P0.1 - Key1     |
 *          |   P0.2 - Key2     |
 *          |   P0.3 - Key3     |
 *          |   P0.4 - Key4     |
 *          |   P0.5 - Key5     |
 *          |   P0.6 - Key6     |
 *          |   P0.7 - Key7     |
 *          |   P3.7 - KeyB  ;//与Led4复用       |
 *           ----------------- 
 * 库版本  ：
 *
 * 作者    ：sun20120214																										  
 * 博客    ：
/**********************************************************************************/
//#include "AT89X52.H"
#include "key.h" 


 /***********************************************************************
 * 函数名： DelayX()
 * 描述  ：延时
 * 输入  ：延时值，
 * 输出  ：无
 *

 void DelayX(nCount)
{
  for(; nCount != 0; nCount--);
} 


 /************************************************************************
 * 函数名：Key_Scan
 * 描述  ：检测是否有按键按下
 * 输入  ：Px_Pin：待读取的端口位 .x 可以是 0，1，2，3或者 4	
 *		   
 * 输出  ：KEY_OFF(没按下按键)、KEY_ON（按下按键）
 ****




U8    Key_Scan(KeyX)
{			
		//*检测是否有按键按下 
  	if(KeyX == KEY_ON ) 
	  {	   
	 	 //*延时消抖
	   Delay5Ms();	//DelayX(255);		
	   		if(KeyX == KEY_ON )  
					{	 
						//*等待按键释放 
						while(KeyX == KEY_ON);   
			 			return 	KEY_ON;	 
					}
		   	else
					return KEY_OFF;
		}
	else
		return KEY_OFF;
}



/******************* (C) SUN 2012 嵌入式开发工作室 *****END OF FILE****/
