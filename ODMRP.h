#ifndef __ODMRP_H__
#define __ODMRP_H__
///#include <time.h>
#include <string.h>
#include "headerfile.h"	

#define nodeNum 5              //���ýڵ���10	������һλ��ram��271
#define lineNum 10              //���ö�����20  ÿ����һλ��ram��40
//����֯����adhoc��������鲥ODMRP·��
//2013061318kc51v4.23ʮ�ڵ��ʮ�����޴���XDATA����ͨ��Program Size: data=9.0 xdata=2342 code=4970
//2013061319kc51v4.23��ڵ�ʮ�����޴���XDATA����ͨ��Program Size: data=9.0 xdata=947 code=4735
//2013061323kc51v4.23��ڵ�ʮ����XDATA����ͨ��Program Size: data=9.0 xdata=985 const=0 code=8619
//·�ɱ�ڵ�ÿ���ڵ㰴�轨����ά��·�ɱ�
//һ���ڵ���յ�һ�����ظ���Join_Reply�������·�ɱ��н�����Ӧ��Ŀ��
//���߸���·�ɱ�����Ӧ����Ŀ��ʱ��ֵ���ڸ���Ŀ�м�¼Դ�ڵ�͵���Դ�ڵ����һ���ڵ㡣
//һ��ʱ��ֵ�����趨��TIME����������ಥ���Դ�ڵ㲻�ٷ������ݻ��߿�����Ϣ����ɾ������Ŀ��
struct routing_Table   //	·�ɱ�//�ṹ���ڲ��Ǳ���
{
	U16 srcAddr;			//Դ�ڵ��ʶ
	U16 preAddr;			//��������Ϣ�Ľڵ��ʶ
	U8 MID;				//�ಥ���ʶ
	U32 timer;			//�洢������߸���ʱ�䣬��time()����ȡ�ô�1970.1.1��0��ʱ���������(long��)
};

//һ���ڵ��Ƕಥ���һ��ת����ڵ��ʱ����ת�������ά��ת������Ϣ��
//��¼�ಥ��ʶ����ID�ͱ��ڵ������ˢ�µ�ʱ�䡣
//����ڵ��յ�Join_Reply�����Ǹ���Ϣ�е�Next Hop Address�����������ת�����Ա��
struct forward_Table	   // ת�����
{
	U16 srcAddr;			//Դ�ڵ��ʶ
	U8 FW_FLAG; //U16 FW_FLAG;			//1��ʾ��ת�����Ա��0��ʾ����ת�����Ա
	U8 MID;				//�ಥ���ʶ
	U32 timer;			//�洢������߸���ʱ�䣬��time()����ȡ�ô�1970.1.1��0��ʱ���������(long��)
};

//ÿ���ڵ�ά��һ����Ϣ�洢��Ŀ����Ϊ�˼���ظ����顣
//һ���ڵ���յ�һ���µ�Join_Query����������ݷ����
//���÷����Դ�ڵ�ʶ����ID�����кż�¼���Լ�����Ϣ�洢���У�
//����ʹ��LRU/FIFO�㷨������õ���Ŀ,���������FIFO�㷨
struct msg_Cache			//��Ϣ�洢��
{
	U16 srcAddr;			//Դ�ڵ��ʶ
	U16 seqNum;				//��Ϣ�����к�
	U8 MID;				//�ಥ���ʶ
	U32 timer;			//�洢������߸���ʱ�䣬��time()����ȡ�ô�1970.1.1��0��ʱ���������(long��)
	U8 type;				//U16 type��Ϣ���ͣ�1-join_Query;2-join_Reply;3-data
};

//join_Query����ѯ����Ϣ��Դ�ڵ㷢����;�����ڵ�ת������Ϣ��Ŀ�����ڽ���·��
struct join_Query			 //����ѯ��
{
	U16 type;				//��Ϣ����
	U16 reserved;			//������Ϣ����Ϊ0
	U16 TTL;				//time to liveֵ��ÿ����һ����һ
	U16 hopCount;			//��������������Դ�����ܽڵ㣬ÿ����һ����һ
	U16 seqNum;				//��Ϣ�����к�
	U16 srcAddr;			//Դ�ڵ��ʶ
	U16 preAddr;			//��������Ϣ�Ľڵ��ʶ
	U8 MID;					//�ಥ���ʶ
};

//join_Reply����Ӧ����Ϣ�ɽ��ܽڵ㷢����һ�����ܽڵ��յ�һ���µ�join_Query��Ϣ��Ҫ����Join_Reply��Ӧ
//;�����ڵ���routing_Table��ȷ���Լ��Ƿ���ת����ڵ�
struct join_Reply			 //����Ӧ��
{
	U16 type;				//��Ϣ����
	U16 count;				//��������������Դ�����ܽڵ�
	U16 reserved;			//������Ϣ����Ϊ0
	U16 R;					//Acknowledgment request flag. 
							//This flag is set when active acknowledgment packet is requested
	U16 F;					//ת�����ʶ����Ϣ����ת�����Ա����������Ϊ1
	U16 seqNum;				//��Ϣ�����к�
	U16 srcAddr;			//Դ�ڵ��ʶ
	U16 preAddr;			//��������Ϣ�Ľڵ��ʶ
	U16 nextAddr;			//join_Reply����һ���ڵ��ʶ
	U8 MID;				//�ಥ���ʶ
};

struct routing_Table routingTable[nodeNum];	//·�ɱ�//����routing_Tableһ���Ľṹ��routingTable[nodeNum]
struct forward_Table forwardTable[nodeNum];	//��ת�����Ա������ת�����
struct msg_Cache msgCache[nodeNum][nodeNum];//��Ϣ�洢��

//����Դ�ڵ��ʶ�����кŵĳ�ʼֵ��ÿ���ڵ���Ϣ�������ĳ�ʼֵ��time to liveֵ
U16 src,seqNumber,msgCacheNum,ttl;
U8 mID;		//�����ಥ��ı�ʶ

//�����ڵ�֮�����ӵĹ�ϵ��1��ʾ�����ӣ�0��ʾ������
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

//�������ܽڵ�ı�ʶ
//U16 receiver[5]={3,5,6,8,9};
U16 receiver[5]={2,5,4,8,9};

//������Ϣ��������ǰ�洢�˶�������Ϣ
U16 msgIsFull[nodeNum]={0};	

//����ѯ�ʶ��У����ڽ���·��ʱ��join_Query���������ת��
struct query_Queue
{
	U16 nodeAddr;
	struct join_Query query;
}queryQueue[lineNum];
//����ѯ�ʶ��и�λ������ѯ�ʶ��е�ַ������ѯ�ʶ��м���������ѯ�ʶ�����λ��
U16 queryQueueTop,queryQueueAddr,queryQueueCount,queryQueueHead;

//����Ӧ����У����ڽ���·��ʱ��join_Reply����Ӧ�𰴹������ת��
struct reply_Queue
{
	U16 nodeAddr;
	struct join_Reply reply;
}replyQueue[lineNum];
//����Ӧ����и�λ������Ӧ����е�ַ������Ӧ����м���������Ӧ�������λ
U16 replyQueueTop,replyQueueAddr,replyQueueCount,replyQueueHead;

struct join_Query tempQuery;//���ڴ洢��ʱ������ѯ����Ϣ
struct join_Reply tempReply;//���ڴ洢��ʱ������Ӧ����Ϣ

///_FILE_ *fp;	  ///�����ļ�ָ��

U16 TIME;//���ڴ����Ϣ���ڵ�ʱ�䣬Ӧ��<300��
/*struct join_Data 
{
	U8 message[lineNum];
	
}*/
/*struct msg_Queue
{
	U16 nodeAddr;
	U8 msg[30];
}msgQueue[lineNum];*/
U16 msgQueue[lineNum];	 //���ݶ���
//���ݶ��и�λ�����ݶ��е�ַ�����ݶ��м��������ݶ�����λ
U16 msgQueueTop,msgQueueAddr,msgQueueCount,msgQueueHead;

void initODMRP(void);	//��ʼ�����������еĻ�������
void setupRouter(void); //��ʼ�������е�·����Ϣ������·�ɹ���
void processJoinQuery(void);//����Join_Query����ѯ����Ϣ
void pushQueryQueue(U16 addr,struct join_Query q);//�����ɵ�join_QueryԴ�ڵ��ʶ+����ѯ����Ϣѹ������ѯ�ʶ��У�����ѯ�ʶ��и�λ++������ѯ�ʶ��м���++
void popQueryQueue(void);//ȡ������queryQueue[].nodeAddr��tempQuery��,queryQueueCount--;queryQueueHead++;
U16 isEmptyQueryQueue(void);//�ж�����ѯ�ʶ��м����Ƿ�Ϊ��

void processJoinReply(void);//����join_Reply����Ӧ����Ϣ
void pushReplyQueue(U16 addr,struct join_Reply r);//ѹջ������Reply����Ӧ����Ϣ
void popReplyQueue(void);//ȡ��������Ӧ����Ϣ����tempReply��replyQueueAddr��//�޸�replyQueueTopֵ
U16 isEmptyReplyQueue(void);//�ж�����Ӧ������Ƿ�Ϊ��

U16 checkMsgCache(U16 addr,U8 type);//U16 type����Ƿ���������Ϣ��type=1��query��2��Reply��3��data
void sendData(void);//��������
U16 isEmptyMsgQueue(void);//�ж����ݶ����Ƿ�Ϊ��
void pushMsgQueue(U16 addr); //ѹջ������
void popMsgQueue(void);
U16 ODMRP_main(void);
U32 time(U8 tmp0);//��ȡ����

#endif