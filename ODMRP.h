#ifndef __ODMRP_H__
#define __ODMRP_H__
///#include <time.h>
#include <string.h>
#include "headerfile.h"	

#define nodeNum 5              //设置节点数10	再增加一位，ram多271
#define lineNum 10              //设置队列数20  每增加一位，ram多40
//自组织网络adhoc网格多跳组播ODMRP路由
//2013061318kc51v4.23十节点二十队列无串口XDATA编译通过Program Size: data=9.0 xdata=2342 code=4970
//2013061319kc51v4.23五节点十队列无串口XDATA编译通过Program Size: data=9.0 xdata=947 code=4735
//2013061323kc51v4.23五节点十队列XDATA编译通过Program Size: data=9.0 xdata=985 const=0 code=8619
//路由表节点每个节点按需建立和维护路由表。
//一个节点接收到一个非重复的Join_Reply分组后，在路由表中建立相应条目，
//或者更新路由表中相应的条目的时间值，在该条目中记录源节点和到达源节点的下一跳节点。
//一旦时间值超过设定的TIME，表明这个多播组的源节点不再发送数据或者控制信息，则删除此条目。
struct routing_Table   //	路由表//结构体内部是变量
{
	U16 srcAddr;			//源节点标识
	U16 preAddr;			//最后处理此信息的节点标识
	U8 MID;				//多播组标识
	U32 timer;			//存储插入或者更新时间，以time()方法取得从1970.1.1：0点时至今的秒数(long型)
};

//一个节点是多播组的一个转发组节点的时候，在转发组表中维护转发组信息。
//记录多播组识别码ID和本节点最近被刷新的时间。
//如果节点收到Join_Reply并且是该信息中的Next Hop Address，则表明它是转发组成员。
struct forward_Table	   // 转发组表
{
	U16 srcAddr;			//源节点标识
	U8 FW_FLAG; //U16 FW_FLAG;			//1表示是转发组成员，0表示不是转发组成员
	U8 MID;				//多播组标识
	U32 timer;			//存储插入或者更新时间，以time()方法取得从1970.1.1：0点时至今的秒数(long型)
};

//每个节点维护一个消息存储表，目的是为了检测重复分组。
//一个节点接收到一个新的Join_Query分组或者数据分组后，
//将该分组的源节点识别码ID和序列号记录在自己的消息存储表中，
//可以使用LRU/FIFO算法清除不用的条目,本程序采用FIFO算法
struct msg_Cache			//消息存储表
{
	U16 srcAddr;			//源节点标识
	U16 seqNum;				//信息的序列号
	U8 MID;				//多播组标识
	U32 timer;			//存储插入或者更新时间，以time()方法取得从1970.1.1：0点时至今的秒数(long型)
	U8 type;				//U16 type信息类型，1-join_Query;2-join_Reply;3-data
};

//join_Query链接询问消息由源节点发出，途径各节点转发此信息，目的在于建立路由
struct join_Query			 //链接询问
{
	U16 type;				//信息类型
	U16 reserved;			//保留信息，设为0
	U16 TTL;				//time to live值，每经过一跳减一
	U16 hopCount;			//经过的跳数，从源到接受节点，每经过一跳加一
	U16 seqNum;				//信息的序列号
	U16 srcAddr;			//源节点标识
	U16 preAddr;			//最后处理此信息的节点标识
	U8 MID;					//多播组标识
};

//join_Reply链接应答消息由接受节点发出，一旦接受节点收到一个新的join_Query消息则要返回Join_Reply响应
//途径各节点检查routing_Table以确认自己是否是转发组节点
struct join_Reply			 //链接应答
{
	U16 type;				//信息类型
	U16 count;				//经过的跳数，从源到接受节点
	U16 reserved;			//保留信息，设为0
	U16 R;					//Acknowledgment request flag. 
							//This flag is set when active acknowledgment packet is requested
	U16 F;					//转发组标识，信息经过转发组成员处理则设置为1
	U16 seqNum;				//信息的序列号
	U16 srcAddr;			//源节点标识
	U16 preAddr;			//最后处理此信息的节点标识
	U16 nextAddr;			//join_Reply的下一跳节点标识
	U8 MID;				//多播组标识
};

struct routing_Table routingTable[nodeNum];	//路由表//定义routing_Table一样的结构体routingTable[nodeNum]
struct forward_Table forwardTable[nodeNum];	//是转发组成员，设置转发组表
struct msg_Cache msgCache[nodeNum][nodeNum];//消息存储表

//声明源节点标识，序列号的初始值，每个节点信息缓冲器的初始值，time to live值
U16 src,seqNumber,msgCacheNum,ttl;
U8 mID;		//声明多播组的标识

//声明节点之间链接的关系，1标示有链接，0标示无连接
U16 node[nodeNum][nodeNum]={0};	//**/
/*U16 node[nodeNum][nodeNum]={1,1,0,0,0,0,0,0,0,0,
							1,1,1,0,1,0,0,0,0,0,
							0,1,1,1,1,1,0,0,0,0,
							0,0,1,1,0,0,0,0,0,0,
							0,1,0,0,1,0,1,1,0,0,
							0,0,1,0,0,1,0,0,0,0,
							0,0,0,0,1,0,1,0,0,0,
							0,0,0,0,1,0,0,1,1,1,
							0,0,0,0,0,0,0,1,1,0,
							0,0,0,0,0,0,0,1,0,1
							}; 
							//0,1,2,3,4,5,6,7,8,9
/*U16 node[nodeNum][nodeNum]={0,1,0,0,0,0,1,0,0,0,
							1,0,1,0,0,1,0,1,0,0,
							0,1,0,1,0,0,0,0,0,0,
							0,0,1,0,1,1,0,0,0,0,
							0,0,0,1,0,1,0,0,1,0,
							0,1,0,1,1,0,0,1,1,0,
							1,0,0,0,0,0,0,1,0,0,
							0,1,0,0,0,1,1,0,0,1,
							0,0,0,0,1,1,0,0,0,1,
							0,0,0,0,0,0,0,1,1,0
							};*/

//声明接受节点的标识
//U16 receiver[5]={3,5,6,8,9};
U16 receiver[5]={2,5,4,8,9};

//声明信息缓存器当前存储了多少条信息
U16 msgIsFull[nodeNum]={0};	

//链接询问队列，用于建立路由时的join_Query按广度优先转发
struct query_Queue
{
	U16 nodeAddr;
	struct join_Query query;
}queryQueue[lineNum];
//链接询问队列高位，链接询问队列地址，链接询问队列计数，链接询问队列首位。
U16 queryQueueTop,queryQueueAddr,queryQueueCount,queryQueueHead;

//链接应答队列，用于建立路由时的join_Reply链接应答按广度优先转发
struct reply_Queue
{
	U16 nodeAddr;
	struct join_Reply reply;
}replyQueue[lineNum];
//链接应答队列高位，链接应答队列地址，链接应答队列计数，链接应答队列首位
U16 replyQueueTop,replyQueueAddr,replyQueueCount,replyQueueHead;

struct join_Query tempQuery;//用于存储临时的链接询问信息
struct join_Reply tempReply;//用于存储临时的链接应答信息

///_FILE_ *fp;	  ///声明文件指针

U16 TIME;//用于存放信息存在的时间，应该<300秒
/*struct join_Data 
{
	U8 message[lineNum];
	
}*/
/*struct msg_Queue
{
	U16 nodeAddr;
	U8 msg[30];
}msgQueue[lineNum];*/
U16 msgQueue[lineNum];	 //数据队列
//数据队列高位，数据队列地址，数据队列计数，数据队列首位
U16 msgQueueTop,msgQueueAddr,msgQueueCount,msgQueueHead;

void initODMRP(void);	//初始化整个网络中的基本参数
void setupRouter(void); //初始化网络中的路由信息，建立路由过程
void processJoinQuery(void);//处理Join_Query链接询问信息
void pushQueryQueue(U16 addr,struct join_Query q);//将生成的join_Query源节点标识+链接询问消息压入链接询问队列，链接询问队列高位++，链接询问队列计数++
void popQueryQueue(void);//取出放在queryQueue[].nodeAddr和tempQuery中,queryQueueCount--;queryQueueHead++;
U16 isEmptyQueryQueue(void);//判断链接询问队列计数是否为空

void processJoinReply(void);//处理join_Reply链接应答信息
void pushReplyQueue(U16 addr,struct join_Reply r);//压栈，放入Reply链接应答信息
void popReplyQueue(void);//取出的链接应答信息放在tempReply和replyQueueAddr中//修改replyQueueTop值
U16 isEmptyReplyQueue(void);//判断链接应答队列是否为空

U16 checkMsgCache(U16 addr,U8 type);//U16 type检查是否是冗余信息，type=1是query，2是Reply，3是data
void sendData(void);//发送数据
U16 isEmptyMsgQueue(void);//判断数据队列是否为空
void pushMsgQueue(U16 addr); //压栈，放入
void popMsgQueue(void);
U16 ODMRP_main(void);
U32 time(U8 tmp0);//获取秒数

#endif