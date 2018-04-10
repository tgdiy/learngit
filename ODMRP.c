#include <stdlib.h>
#include <stdio.h>
#include "AT89X52.h"	
#include "ODMRP.h"
#include "uart.h"
//自组织网络adhoc网格多跳组播ODMRP路由
//2013061318kc51v4.23十节点二十队列无串口XDATA编译通过Program Size: data=9.0 xdata=2342 code=4970
//2013061318kc51v4.23一节点一队列无串口PDATA编译通过Program Size: data=9.0 xdata=223 code=3381


/**************************************************************************
 - 功能描述： ODMRP_main()主函数
 - 隶属模块：
 - 函数属性：
 - 参数说明：
 - 返回说明：
 - 更改记录：
 **************************************************************************/
U16  ODMRP_main()
{	


	initODMRP();		  //初始化整个网络中的基本参数

	setupRouter();		 //初始化网络中的路由信息，建立路由过程

	sendData();			 //发送数据

	//fclose(fp);	//关闭文件
    return 0;
}

/**************************************************************************
 - 功能描述：void setupRouter(void); //初始化网络中的路由信息，建立路由过程
 - 隶属模块：
 - 函数属性：
 - 参数说明：
 - 返回说明：
 - 更改记录：
 **************************************************************************/
void setupRouter()				 //初始化网络中的路由信息，建立路由过程
{
	//UART_Send_Str("Begin setupRouter()\n");	//	fprintf(fp,"Begin setupRouter()\n");

	processJoinQuery();			   //处理Join_Query链接询问信息

	processJoinReply();			   //处理join_Reply链接应答信息

	//UART_Send_Str("End setupRouter()\n");	//fprintf(fp,"End setupRouter()\n");				
}

/**************************************************************************
 - 功能描述：U16 checkMsgCache(U16 addr,int8 type);//U16 type检查是否是冗余信息，type=1是query，2是Reply，3是data
 - 隶属模块：
 - 函数属性：
 - 参数说明：
 - 返回说明：
 - 更改记录：
 **************************************************************************/
//检查是否是冗余信息，如果信息已经存在在msgCache中则返回0，否则返回1
U16 checkMsgCache(U16 addr,U8 type)	 //U16 type检查是否是冗余信息，type=1是query，2是Reply，3是data
{
	U8 i;//U16 i;
	U16 tempAddr;
	struct join_Query tQuery;
	struct join_Reply tReply;

	switch(type)
	{
		case 1: 
				tempAddr=queryQueueAddr;
				tQuery=tempQuery;
				for (i=0;i<nodeNum;i++)
				{
					if ( (msgCache[addr][i].srcAddr==tQuery.srcAddr)&&
						 (msgCache[addr][i].seqNum==tQuery.seqNum)&&
						 (msgCache[addr][i].MID==tQuery.MID)&&
						 (msgCache[addr][i].type==tQuery.type) )
						return 0;
					else continue;
				}
				return 1;
				break;
		case 2: 
				tempAddr=replyQueueAddr;
				tReply=tempReply; 
				for (i=0;i<nodeNum;i++)
				{
					if ( (msgCache[addr][i].srcAddr==tReply.srcAddr)&&
						 (msgCache[addr][i].seqNum==tReply.seqNum)&&
						 (msgCache[addr][i].MID==tReply.MID)&&
						 (msgCache[addr][i].type==tReply.type) )
						return 0;
					else continue;
				}
				return 1;
			    break;
		case 3: 
				//tempAddr=msgQueueAddr;
				for (i=0;i<nodeNum;i++)
				{
					if ( msgCache[addr][i].seqNum==-2)
						return 0;
					else continue;
				}
				return 1;
				break;
		default:return 0;
	}
	
}

/**************************************************************************
 - 功能描述： U32 time(U8 tmp0);//获取秒数
 - 隶属模块：
 - 函数属性：
 - 参数说明：
 - 返回说明：
 - 更改记录：
 **************************************************************************/
U32 time(U8 tmp0)
{
U8 i;
U32 j;
 i=tmp0;
 j=22222222;
 return j;
}


/***********************************************ODMRP_init.c****************************************/
/**************************************************************************
 - 功能描述：void initODMRP(void);	//初始化整个网络中的基本参数
 - 隶属模块：
 - 函数属性：
 - 参数说明：
 - 返回说明：
 - 更改记录：
 **************************************************************************/
//初始化整个网络中的基本参数
void initODMRP()
{
	U8 i,j;//int i,j;				//循环变量

	///if((fp=fopen("result123.txt","w"))==NULL)
///	{
///		puts("Cannot openfile\n");
///		exit(1);
///	}

    //UART_Send_Str("Begin odmrp_init file\n"); //	fprintf(fp,"Begin odmrp_init file\n");

	mID='A';                //U18 设置多播组的标识
	src=0;					//U16 设置源节点标识 
	seqNumber=0;			//U16 设置序列号的初始值
	msgCacheNum=0;			//U16 设置每个节点信息缓冲器的初始值，最多nodeNum个信息；
							// >nodeNum,减为0
	ttl=5;					//U16 设置time to live值为5，则信息最多可以跳5跳

	//初始化消息存储表msgCache[nodeNum][nodeNum]
	for (i=0;i<nodeNum;i++)
		for (j=0;j<nodeNum;j++)
		{
			msgCache[i][j].srcAddr=0;
			msgCache[i][j].seqNum=-1;
			//msgCache[i][j].timer=0;
			msgCache[i][j].MID=' ';
			msgCache[i][j].type=0;
		}

	//初始化msgIsFull[nodeNum]//声明信息缓存器当前存储了多少条信息
	for (i=0;i<nodeNum;i++)
	{
		msgIsFull[i]=0;
	}

	//初始化forwardTable[nodeNum]// 转发组表
	for (i=0;i<nodeNum;i++)
	{
		forwardTable[i].FW_FLAG=0;
		forwardTable[i].MID=' ';
		forwardTable[i].srcAddr=0;
		//forwardTable[i].timer=0;
	}

	//初始化routingTable[nodeNum] //路由表
	for (i=0;i<nodeNum;i++)
	{
		routingTable[i].MID=' ';
		routingTable[i].preAddr=0;
		routingTable[i].srcAddr=0;
		//routingTable[i].timer=0;
	}

	
	for (i=0;i<lineNum;i++)		
	{
		queryQueue[i].nodeAddr=0;	  //初始化请求队列queryQueue
		replyQueue[i].nodeAddr=0;	  //初始化回复队列replyQueue
		//queryQueue[i].query=0;
		//replyQueue[i].reply=0;
	}

	//各个标志位
	queryQueueTop=0;   //高位
	replyQueueTop=0;
	replyQueueAddr=0;	//地址
	queryQueueAddr=0;
	queryQueueCount=0;	//计数
	replyQueueCount=0;
	queryQueueHead=0;	//首位
	replyQueueHead=0;
	msgQueueTop=0;		//初始化数据队列高位
	msgQueueAddr=0;
	msgQueueCount=0;
	msgQueueHead=0;
	
	for (i=0;i<lineNum;i++)		 //初始化数据队列
	{
		msgQueue[i]=0;
	}
    #ifdef _DEBUG	 
    UART_Send_Str("End odmrp_init file\n");  
    #endif ///	fprintf(fp,"End odmrp_init file\n");
	
}

/*********************************************processJoinQuery.c************************************/
/**************************************************************************
 - 功能描述：void processJoinQuery(void);//处理Join_Query信息
 - 隶属模块：
 - 函数属性：
 - 参数说明：
 - 返回说明：
 - 更改记录：
 **************************************************************************/
//初始化网络中的路由信息，建立路由过程
void processJoinQuery()
{
	struct join_Query query,newQuery;		//新建2结构体query,newQuery;
	U16 i,j;
	U16 tempTTL;
	U16 tempHopCount;
	
	//UART_Send_Str("Begin processJoinQuery()\n");	//fprintf(fp,"Begin processJoinQuery()\n");

	//结构体query列表，生成链接询问消息
	query.type=0;			  	//信息类型
	query.TTL=ttl;				//time to live值5，每经过一跳减一
	query.hopCount=0;			//经过的跳数，从源到接受节点，每经过一跳加一
	query.MID=mID;				//多播组标识
	query.srcAddr=src;			//源节点标识
	query.preAddr=src;			//最后处理此信息的节点标识
	query.reserved=0;			//保留信息，设为0
	query.seqNum=seqNumber;		//信息的序列号

	//将生成的join_Query源节点标识+链接询问消息压入链接询问队列，链接询问队列高位++，链接询问队列计数++
	pushQueryQueue(query.srcAddr,query);
	while(!isEmptyQueryQueue())	   //判断链接询问队列计数是否为空，不空=0向下执行
	{
		popQueryQueue();//取出放在queryQueue[].nodeAddr和tempQuery中,queryQueueCount--;queryQueueHead++;
				
		for (i=0;i<nodeNum;i++)
		{
			//if (i==4)	fprU16f(fp,"The node 4: checkMsgCache=%d\n",checkMsgCache(i,1));
			//i不是节点本身，且是节点的邻居，且在邻居节点中，没有此消息存在(非冗余=1)
			if ( (i!=src)&&(i!=queryQueueAddr) && (node[queryQueueAddr][i]==1) && (checkMsgCache(i,1)) )
			{
				//setMsgCache(i);
				/*if (msgCacheNum>nodeNum)
					msgCacheNum=msgCacheNum-nodeNum;
				msgCache[i][msgCacheNum].srcAddr=tempQuery.srcAddr;
				msgCache[i][msgCacheNum].seqNum=tempQuery.seqNum;
				msgCache[i][msgCacheNum].MID=tempQuery.MID;
				msgCache[i][msgCacheNum].timer=time(NULL);
				msgCache[i][msgCacheNum].type=tempQuery.type;
				msgCacheNum++;*/
				if (msgIsFull[i]>nodeNum)	//保存消息
					msgIsFull[i]=msgIsFull[i]-nodeNum;
				msgCache[i][msgIsFull[i]].srcAddr=tempQuery.srcAddr;
				msgCache[i][msgIsFull[i]].seqNum=tempQuery.seqNum;
				msgCache[i][msgIsFull[i]].MID=tempQuery.MID;
				msgCache[i][msgIsFull[i]].timer=time(NULL);
				msgCache[i][msgIsFull[i]].type=tempQuery.type;
				msgIsFull[i]++;
				

				//setRouteTable(i);	保存路由表
				routingTable[i].srcAddr=tempQuery.srcAddr;
				routingTable[i].preAddr=queryQueueAddr;
				routingTable[i].MID=tempQuery.MID;
				routingTable[i].timer=time(NULL);

				tempHopCount=tempQuery.hopCount;
				tempTTL=tempQuery.TTL;
				tempHopCount++;
				tempTTL--;
				if (tempTTL>0)
				{
					//建立此邻居节点的query消息
					newQuery.hopCount=tempHopCount;
					newQuery.TTL=tempTTL;
					newQuery.preAddr=i;

					newQuery.MID=tempQuery.MID;
					newQuery.reserved=0;
					newQuery.seqNum=tempQuery.seqNum;
					newQuery.srcAddr=tempQuery.srcAddr;					
					newQuery.type=tempQuery.type;
					
					pushQueryQueue(newQuery.preAddr,newQuery);//压栈，放入query信息
					
					//判断是否是接收节点
					for (j=0;j<5;j++)
					{
						if (receiver[j]==i)
						{
							//setJoinReply(i)
							tempReply.count=newQuery.hopCount;
							tempReply.F=0;
							tempReply.MID=newQuery.MID;
							tempReply.nextAddr=routingTable[i].preAddr;
							tempReply.preAddr=i;
							tempReply.R=0;
							tempReply.reserved=0;
							tempReply.seqNum=newQuery.seqNum;
							tempReply.srcAddr=newQuery.srcAddr;
							tempReply.type=2;

							pushReplyQueue(tempReply.preAddr,tempReply);
						}//end if (receiver[j]==i)
					}//end for
				}//end if (tempTTL>0)
			}//end if (i!=queryQueueAddr)
		}//end for (i=0;i<nodeNum;i++)
	}//end while(!isEmptyQueryQueue())

#ifdef _DEBUG	
UART_Send_Str("End processJoinQuery()\n");  
#endif	//fprintf(fp,"End processJoinQuery()\n");
}

/**************************************************************************
 - 功能描述：void pushQueryQueue(U16 addr,struct join_Query q);//
 - 隶属模块：
 - 函数属性：将生成的join_Query源节点标识+链接询问消息压入链接询问队列，链接询问队列高位++，链接询问队列计数++
 - 参数说明：
 - 返回说明：
 - 更改记录：
 **************************************************************************/
//把query链接询问消息放入链接询问队列中，目的是为了能够以广度优先转发join_Query链接询问消息

//队列没有考虑循环，设置了200个信息？，用完为止
void pushQueryQueue(U16 addr,struct join_Query q)	
{
	queryQueue[queryQueueTop].nodeAddr=addr;	 //	U16 nodeAddr;
	queryQueue[queryQueueTop].query=q;			 //struct join_Query query;

	queryQueueTop++;							//链接询问队列高位
	queryQueueCount++;							//链接询问队列计数

	//UART_Send_Str("\t pushQueryQueue(): node=%d\t sum=%d\t top=%d\n");	//fprintf(fp,"\t pushQueryQueue(): node=%d\t sum=%d\t top=%d\n",addr,queryQueueCount,queryQueueTop);

}

/**************************************************************************
 - 功能描述：U16 isEmptyQueryQueue(void);//判断链接询问队列计数是否为空
 - 隶属模块：
 - 函数属性：
 - 参数说明：
 - 返回说明：
 - 更改记录：
 **************************************************************************/
//判断是否请求队列为空
U16 isEmptyQueryQueue()
{
	if (queryQueueCount==0)				   //链接询问队列计数
		return 1;
	else return 0;
}

/**************************************************************************
 - 功能描述：void popQueryQueue(void);
 - 隶属模块：
 - 函数属性：//取出放在queryQueue[].nodeAddr和tempQuery中,queryQueueCount--;queryQueueHead++;
 - 参数说明：
 - 返回说明：
 - 更改记录：
 **************************************************************************/
//取出的信息放在tempQuery和queryQueueAddr中
//修改queryQueueTop值?
void popQueryQueue()
{
	if (queryQueueCount==0)					   //链接询问队列计数
	{
		//UART_Send_Str("The queryQueue is empty, cannot pop query\n");//fprintf(fp,"The queryQueue is empty, cannot pop query\n");
		//return 0;
	}

	queryQueueAddr=queryQueue[queryQueueHead].nodeAddr;
	tempQuery=queryQueue[queryQueueHead].query;
	queryQueueCount--;
	queryQueueHead++;

	//UART_Send_Str("\t popQueryQueue():  node=%d\t sum=%d\t top=%d\n");	//fprintf(fp,"\t popQueryQueue():  node=%d\t sum=%d\t top=%d\n",queryQueueAddr,queryQueueCount,queryQueueTop);
}

/*********************************************processJoinReply.c************************************/			  																										  					 
/**************************************************************************
 - 功能描述：void pushReplyQueue(U16 addr,struct join_Reply r);//压栈，放入query信息
 - 隶属模块：
 - 函数属性：
 - 参数说明：
 - 返回说明：
 - 更改记录：
 **************************************************************************/
//把reply消息放入队列中，目的是为了能够以广度优先转发join_Reply消息
//队列没有考虑循环，设置了200个信息，用完为止
void pushReplyQueue(U16 addr, struct join_Reply r)
{
	replyQueue[replyQueueTop].nodeAddr=addr;
	replyQueue[replyQueueTop].reply=r;

	replyQueueTop++;
	replyQueueCount++;

	//UART_Send_Str("\t pushReplyQueue(): node=%d\t sum=%d\t top=%d\t nextHop=%d\n");//fprintf(fp,"\t pushReplyQueue(): node=%d\t sum=%d\t top=%d\t nextHop=%d\n",addr,replyQueueCount,replyQueueTop,r.nextAddr);
}	

/**************************************************************************
 - 功能描述：void processJoinReply(void);//处理join_Reply信息
 - 隶属模块：
 - 函数属性：
 - 参数说明：
 - 返回说明：
 - 更改记录：
 **************************************************************************/
void processJoinReply()
{
	U16 i;
	struct join_Reply newReply;

	//UART_Send_Str("Begin processJoinReply()\n");	 //	fprintf(fp,"Begin processJoinReply()\n");

	while(!isEmptyReplyQueue())
	{
		popReplyQueue();

		for (i=0;i<nodeNum;i++)
		{
			//if (i==2)	fprU16f(fp,"The checkMsgCache(i,2)=%d\n",checkMsgCache(i,2));
			if ( (i!=replyQueueAddr) && (node[replyQueueAddr][i]==1) && (checkMsgCache(i,2)) )
			{
				//把Reply消息放入msgCache中
				/*if (msgCacheNum>nodeNum)
					msgCacheNum=msgCacheNum-nodeNum;
				msgCache[i][msgCacheNum].srcAddr=tempReply.srcAddr;
				msgCache[i][msgCacheNum].seqNum=tempReply.seqNum;
				msgCache[i][msgCacheNum].MID=tempReply.MID;
				msgCache[i][msgCacheNum].timer=time(NULL);
				msgCache[i][msgCacheNum].type=tempReply.type;
				msgCacheNum++;*/
				if (msgIsFull[i]>nodeNum)
					msgIsFull[i]=msgIsFull[i]-nodeNum;
				msgCache[i][msgIsFull[i]].srcAddr=tempReply.srcAddr;
				msgCache[i][msgIsFull[i]].seqNum=tempReply.seqNum;
				msgCache[i][msgIsFull[i]].MID=tempReply.MID;
				msgCache[i][msgIsFull[i]].timer=time(NULL);
				msgCache[i][msgIsFull[i]].type=tempReply.type;
				msgIsFull[i]++;

				//检查此节点是否是转发组成员
				if( (i==tempReply.nextAddr)&&(i!=src) )
				{
					if (forwardTable[i].FW_FLAG!=1)
					{
						//是转发组成员，设置forwardTable
						forwardTable[i].srcAddr=tempReply.srcAddr;
						forwardTable[i].FW_FLAG=1;
						forwardTable[i].MID=tempReply.MID;
						forwardTable[i].timer=time(NULL);

						//UART_Send_Str("\t The node %d is a forward member\n");//fprintf(fp,"\t The node %d is a forward member\n",i);
					}

					//转发join_Reply
					newReply.nextAddr=routingTable[i].preAddr;
					newReply.preAddr=i;

					newReply.F=tempReply.F;
					newReply.count=tempReply.count;
					newReply.MID=tempReply.MID;
					newReply.R=tempReply.R;
					newReply.reserved=tempReply.reserved;
					newReply.seqNum=tempReply.seqNum;
					newReply.srcAddr=tempReply.srcAddr;
					newReply.type=tempReply.type;

					pushReplyQueue(newReply.preAddr,newReply);
				}
				else if (i==src)
				{
					//是源节点，接收reply，一条路由建立完毕
					//UART_Send_Str("The sourcer receive reply\n");	// fprintf(fp,"The sourcer receive reply\n");
				}
				else
				{
					//不是转发组节点不用转发reply消息
					//什么也不做
				}//endif
			}
			else
			{
				//是冗余信息
				//或者该节点就是自己本身
				//或者不是邻居节点
				//什么也不做
			}//endif
		}//endfor
	}//endwhile

#ifdef _DEBUG	
UART_Send_Str("End processJoinReply()\n");  
#endif //fprintf(fp,"End processJoinReply()\n");

}

/**************************************************************************
 - 功能描述：U16 isEmptyReplyQueue(void);//判断是否请求队列为空
 - 隶属模块：
 - 函数属性：
 - 参数说明：
 - 返回说明：
 - 更改记录：
 **************************************************************************/
//判断是否请求队列为空
U16 isEmptyReplyQueue()
{
	if ( replyQueueCount==0 )
		return 1;
	else return 0;
}

/**************************************************************************
 - 功能描述： //取出的信息放在tempReply和replyQueueAddr中//修改replyQueueTop值
 - 隶属模块：
 - 函数属性：
 - 参数说明：
 - 返回说明：
 - 更改记录：
 **************************************************************************/
void popReplyQueue()
{
	if (replyQueueCount==0)
	{
		//UART_Send_Str("The replyQueue is empty, cannot pop reply\n");	// fprintf(fp,"The replyQueue is empty, cannot pop reply\n");
		//return 0;
	}

	replyQueueAddr=replyQueue[replyQueueHead].nodeAddr;
	tempReply=replyQueue[replyQueueHead].reply;
	replyQueueCount--;
	replyQueueHead++;

	//UART_Send_Str("\t popReplyQueue():  node=%d\t sum=%d\t top=%d\n");// fprintf(fp,"\t popReplyQueue():  node=%d\t sum=%d\t top=%d\n",replyQueueAddr,replyQueueCount,replyQueueTop);
}
	

/**********************************************sendData.c*******************************************/

 //Join_Data arrived
 //search in Msg_Cache检查冗余信息，不是冗余则看是否是转发组节点，满足条件则转发
 //if (SrcAddr, SeqNum, MID) exists in the Msg_Cache
 //     then discard this Join_Data;  //冗余数据
 //     else if ((this node.FL_FLAG==1)&&(Time<TIME))
 //              then Broadcast this Join_Data packet;
 //              else node is not forwarding node or expired;
/**************************************************************************
 - 功能描述：void sendData(void);//发送数据
 - 隶属模块：
 - 函数属性：
 - 参数说明：  无
 - 返回说明：  无
 - 更改记录：
 **************************************************************************/
void sendData()		   //发送数据
{
	U16 i,j;
	//UART_Send_Str("\nBegin sendData()\n"); //	fprintf(fp,"\nBegin sendData()\n");
	
	//U8 msg[lineNum]="This is MSG.";//数据
	msgQueue[0]=0;//0号源节点放入数据队列等待发送数据
	pushMsgQueue(0);
	
	while(!isEmptyMsgQueue())
	{
		popMsgQueue();

		for (i=0;i<nodeNum;i++)
		{
			//if (i==3)
			//	fprU16f(fp,"  node 3:checkMsgCache(3,3)=%d\n",checkMsgCache(3,3));

			if ( (i!=msgQueueAddr)&&(i!=0)&&(checkMsgCache(i,3))&&(node[msgQueueAddr][i]==1) )
			{
				/*if (msgCacheNum>nodeNum)
					msgCacheNum=msgCacheNum-nodeNum;
				msgCache[i][msgCacheNum].seqNum=-2;
				msgCacheNum++;*/
				if (msgIsFull[i]>nodeNum)
					msgIsFull[i]=msgIsFull[i]-nodeNum;
				msgCache[i][msgIsFull[i]].seqNum=-2;
				msgIsFull[i]++;

				//UART_Send_Str("  The node %d receive msg\n");	//fprintf(fp,"  The node %d receive msg\n",i);

				if (forwardTable[i].FW_FLAG==1)
				{
					//转发组节点，转发数据
					pushMsgQueue(i);
				}
				
				for (j=0;j<5;j++)
				{
					if (receiver[j]==i)
					{
						//UART_Send_Str("\t\tThe receiver node %d receive msg\n"); //fprintf(fp,"\t\tThe receiver node %d receive msg\n",i);
					}
				}

			}
		}
	}
#ifdef _DEBUG	
UART_Send_Str("End sendData()\n");  
#endif // fprintf(fp,"End sendData()\n");

}

/**************************************************************************
 - 功能描述：U16 isEmptyMsgQueue(void);//判断是否数据队列为空
 - 隶属模块：
 - 函数属性：
 - 参数说明：  无
 - 返回说明：
 - 更改记录：
 **************************************************************************/
//判断是否数据队列为空
U16 isEmptyMsgQueue()
{
	if (msgQueueCount==0)
		return 1;
	else return 0;
}

/**************************************************************************
 - 功能描述：
 - 隶属模块：
 - 函数属性：
 - 参数说明：
 - 返回说明：
 - 更改记录：
 **************************************************************************/
void pushMsgQueue(U16 addr)
{
	msgQueue[msgQueueTop]=addr;
	
	msgQueueTop++;
	msgQueueCount++;

	//UART_Send_Str("\t pushMsgQueue():   node=%d\t\n");//	 fprintf(fp,"\t pushMsgQueue():   node=%d\t\n",addr);
}

/**************************************************************************
 - 功能描述：
 - 隶属模块：
 - 函数属性：
 - 参数说明：
 - 返回说明：
 - 更改记录：
 **************************************************************************/
void popMsgQueue()
{
	if (msgQueueCount==0)
	{
		//UART_Send_Str("The msgQueue is empty, cannot pop msg\n");	 //	 fprintf(fp,"The msgQueue is empty, cannot pop msg\n");
		//return 0;
	}

	msgQueueAddr=msgQueue[msgQueueHead];
	msgQueueCount--;
	msgQueueHead++;

	//UART_Send_Str("\t popMsgQueue():    node=%d\t\n");//fprintf(fp,"\t popMsgQueue():    node=%d\t\n",msgQueueAddr);
}

/**************************************************************************************************/
