#include <stdlib.h>
#include <stdio.h>
#include "AT89X52.h"	
#include "ODMRP.h"
#include "uart.h"
//����֯����adhoc��������鲥ODMRP·��
//2013061318kc51v4.23ʮ�ڵ��ʮ�����޴���XDATA����ͨ��Program Size: data=9.0 xdata=2342 code=4970
//2013061318kc51v4.23һ�ڵ�һ�����޴���PDATA����ͨ��Program Size: data=9.0 xdata=223 code=3381


/**************************************************************************
 - ���������� ODMRP_main()������
 - ����ģ�飺
 - �������ԣ�
 - ����˵����
 - ����˵����
 - ���ļ�¼��
 **************************************************************************/
U16  ODMRP_main()
{	


	initODMRP();		  //��ʼ�����������еĻ�������

	setupRouter();		 //��ʼ�������е�·����Ϣ������·�ɹ���

	sendData();			 //��������

	//fclose(fp);	//�ر��ļ�
    return 0;
}

/**************************************************************************
 - ����������void setupRouter(void); //��ʼ�������е�·����Ϣ������·�ɹ���
 - ����ģ�飺
 - �������ԣ�
 - ����˵����
 - ����˵����
 - ���ļ�¼��
 **************************************************************************/
void setupRouter()				 //��ʼ�������е�·����Ϣ������·�ɹ���
{
	//UART_Send_Str("Begin setupRouter()\n");	//	fprintf(fp,"Begin setupRouter()\n");

	processJoinQuery();			   //����Join_Query����ѯ����Ϣ

	processJoinReply();			   //����join_Reply����Ӧ����Ϣ

	//UART_Send_Str("End setupRouter()\n");	//fprintf(fp,"End setupRouter()\n");				
}

/**************************************************************************
 - ����������U16 checkMsgCache(U16 addr,int8 type);//U16 type����Ƿ���������Ϣ��type=1��query��2��Reply��3��data
 - ����ģ�飺
 - �������ԣ�
 - ����˵����
 - ����˵����
 - ���ļ�¼��
 **************************************************************************/
//����Ƿ���������Ϣ�������Ϣ�Ѿ�������msgCache���򷵻�0�����򷵻�1
U16 checkMsgCache(U16 addr,U8 type)	 //U16 type����Ƿ���������Ϣ��type=1��query��2��Reply��3��data
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
 - ���������� U32 time(U8 tmp0);//��ȡ����
 - ����ģ�飺
 - �������ԣ�
 - ����˵����
 - ����˵����
 - ���ļ�¼��
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
 - ����������void initODMRP(void);	//��ʼ�����������еĻ�������
 - ����ģ�飺
 - �������ԣ�
 - ����˵����
 - ����˵����
 - ���ļ�¼��
 **************************************************************************/
//��ʼ�����������еĻ�������
void initODMRP()
{
	U8 i,j;//int i,j;				//ѭ������

	///if((fp=fopen("result123.txt","w"))==NULL)
///	{
///		puts("Cannot openfile\n");
///		exit(1);
///	}

    //UART_Send_Str("Begin odmrp_init file\n"); //	fprintf(fp,"Begin odmrp_init file\n");

	mID='A';                //U18 ���öಥ��ı�ʶ
	src=0;					//U16 ����Դ�ڵ��ʶ 
	seqNumber=0;			//U16 �������кŵĳ�ʼֵ
	msgCacheNum=0;			//U16 ����ÿ���ڵ���Ϣ�������ĳ�ʼֵ�����nodeNum����Ϣ��
							// >nodeNum,��Ϊ0
	ttl=5;					//U16 ����time to liveֵΪ5������Ϣ��������5��

	//��ʼ����Ϣ�洢��msgCache[nodeNum][nodeNum]
	for (i=0;i<nodeNum;i++)
		for (j=0;j<nodeNum;j++)
		{
			msgCache[i][j].srcAddr=0;
			msgCache[i][j].seqNum=-1;
			//msgCache[i][j].timer=0;
			msgCache[i][j].MID=' ';
			msgCache[i][j].type=0;
		}

	//��ʼ��msgIsFull[nodeNum]//������Ϣ��������ǰ�洢�˶�������Ϣ
	for (i=0;i<nodeNum;i++)
	{
		msgIsFull[i]=0;
	}

	//��ʼ��forwardTable[nodeNum]// ת�����
	for (i=0;i<nodeNum;i++)
	{
		forwardTable[i].FW_FLAG=0;
		forwardTable[i].MID=' ';
		forwardTable[i].srcAddr=0;
		//forwardTable[i].timer=0;
	}

	//��ʼ��routingTable[nodeNum] //·�ɱ�
	for (i=0;i<nodeNum;i++)
	{
		routingTable[i].MID=' ';
		routingTable[i].preAddr=0;
		routingTable[i].srcAddr=0;
		//routingTable[i].timer=0;
	}

	
	for (i=0;i<lineNum;i++)		
	{
		queryQueue[i].nodeAddr=0;	  //��ʼ���������queryQueue
		replyQueue[i].nodeAddr=0;	  //��ʼ���ظ�����replyQueue
		//queryQueue[i].query=0;
		//replyQueue[i].reply=0;
	}

	//������־λ
	queryQueueTop=0;   //��λ
	replyQueueTop=0;
	replyQueueAddr=0;	//��ַ
	queryQueueAddr=0;
	queryQueueCount=0;	//����
	replyQueueCount=0;
	queryQueueHead=0;	//��λ
	replyQueueHead=0;
	msgQueueTop=0;		//��ʼ�����ݶ��и�λ
	msgQueueAddr=0;
	msgQueueCount=0;
	msgQueueHead=0;
	
	for (i=0;i<lineNum;i++)		 //��ʼ�����ݶ���
	{
		msgQueue[i]=0;
	}
    #ifdef _DEBUG	 
    UART_Send_Str("End odmrp_init file\n");  
    #endif ///	fprintf(fp,"End odmrp_init file\n");
	
}

/*********************************************processJoinQuery.c************************************/
/**************************************************************************
 - ����������void processJoinQuery(void);//����Join_Query��Ϣ
 - ����ģ�飺
 - �������ԣ�
 - ����˵����
 - ����˵����
 - ���ļ�¼��
 **************************************************************************/
//��ʼ�������е�·����Ϣ������·�ɹ���
void processJoinQuery()
{
	struct join_Query query,newQuery;		//�½�2�ṹ��query,newQuery;
	U16 i,j;
	U16 tempTTL;
	U16 tempHopCount;
	
	//UART_Send_Str("Begin processJoinQuery()\n");	//fprintf(fp,"Begin processJoinQuery()\n");

	//�ṹ��query�б���������ѯ����Ϣ
	query.type=0;			  	//��Ϣ����
	query.TTL=ttl;				//time to liveֵ5��ÿ����һ����һ
	query.hopCount=0;			//��������������Դ�����ܽڵ㣬ÿ����һ����һ
	query.MID=mID;				//�ಥ���ʶ
	query.srcAddr=src;			//Դ�ڵ��ʶ
	query.preAddr=src;			//��������Ϣ�Ľڵ��ʶ
	query.reserved=0;			//������Ϣ����Ϊ0
	query.seqNum=seqNumber;		//��Ϣ�����к�

	//�����ɵ�join_QueryԴ�ڵ��ʶ+����ѯ����Ϣѹ������ѯ�ʶ��У�����ѯ�ʶ��и�λ++������ѯ�ʶ��м���++
	pushQueryQueue(query.srcAddr,query);
	while(!isEmptyQueryQueue())	   //�ж�����ѯ�ʶ��м����Ƿ�Ϊ�գ�����=0����ִ��
	{
		popQueryQueue();//ȡ������queryQueue[].nodeAddr��tempQuery��,queryQueueCount--;queryQueueHead++;
				
		for (i=0;i<nodeNum;i++)
		{
			//if (i==4)	fprU16f(fp,"The node 4: checkMsgCache=%d\n",checkMsgCache(i,1));
			//i���ǽڵ㱾�����ǽڵ���ھӣ������ھӽڵ��У�û�д���Ϣ����(������=1)
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
				if (msgIsFull[i]>nodeNum)	//������Ϣ
					msgIsFull[i]=msgIsFull[i]-nodeNum;
				msgCache[i][msgIsFull[i]].srcAddr=tempQuery.srcAddr;
				msgCache[i][msgIsFull[i]].seqNum=tempQuery.seqNum;
				msgCache[i][msgIsFull[i]].MID=tempQuery.MID;
				msgCache[i][msgIsFull[i]].timer=time(NULL);
				msgCache[i][msgIsFull[i]].type=tempQuery.type;
				msgIsFull[i]++;
				

				//setRouteTable(i);	����·�ɱ�
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
					//�������ھӽڵ��query��Ϣ
					newQuery.hopCount=tempHopCount;
					newQuery.TTL=tempTTL;
					newQuery.preAddr=i;

					newQuery.MID=tempQuery.MID;
					newQuery.reserved=0;
					newQuery.seqNum=tempQuery.seqNum;
					newQuery.srcAddr=tempQuery.srcAddr;					
					newQuery.type=tempQuery.type;
					
					pushQueryQueue(newQuery.preAddr,newQuery);//ѹջ������query��Ϣ
					
					//�ж��Ƿ��ǽ��սڵ�
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
 - ����������void pushQueryQueue(U16 addr,struct join_Query q);//
 - ����ģ�飺
 - �������ԣ������ɵ�join_QueryԴ�ڵ��ʶ+����ѯ����Ϣѹ������ѯ�ʶ��У�����ѯ�ʶ��и�λ++������ѯ�ʶ��м���++
 - ����˵����
 - ����˵����
 - ���ļ�¼��
 **************************************************************************/
//��query����ѯ����Ϣ��������ѯ�ʶ����У�Ŀ����Ϊ���ܹ��Թ������ת��join_Query����ѯ����Ϣ

//����û�п���ѭ����������200����Ϣ��������Ϊֹ
void pushQueryQueue(U16 addr,struct join_Query q)	
{
	queryQueue[queryQueueTop].nodeAddr=addr;	 //	U16 nodeAddr;
	queryQueue[queryQueueTop].query=q;			 //struct join_Query query;

	queryQueueTop++;							//����ѯ�ʶ��и�λ
	queryQueueCount++;							//����ѯ�ʶ��м���

	//UART_Send_Str("\t pushQueryQueue(): node=%d\t sum=%d\t top=%d\n");	//fprintf(fp,"\t pushQueryQueue(): node=%d\t sum=%d\t top=%d\n",addr,queryQueueCount,queryQueueTop);

}

/**************************************************************************
 - ����������U16 isEmptyQueryQueue(void);//�ж�����ѯ�ʶ��м����Ƿ�Ϊ��
 - ����ģ�飺
 - �������ԣ�
 - ����˵����
 - ����˵����
 - ���ļ�¼��
 **************************************************************************/
//�ж��Ƿ��������Ϊ��
U16 isEmptyQueryQueue()
{
	if (queryQueueCount==0)				   //����ѯ�ʶ��м���
		return 1;
	else return 0;
}

/**************************************************************************
 - ����������void popQueryQueue(void);
 - ����ģ�飺
 - �������ԣ�//ȡ������queryQueue[].nodeAddr��tempQuery��,queryQueueCount--;queryQueueHead++;
 - ����˵����
 - ����˵����
 - ���ļ�¼��
 **************************************************************************/
//ȡ������Ϣ����tempQuery��queryQueueAddr��
//�޸�queryQueueTopֵ?
void popQueryQueue()
{
	if (queryQueueCount==0)					   //����ѯ�ʶ��м���
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
 - ����������void pushReplyQueue(U16 addr,struct join_Reply r);//ѹջ������query��Ϣ
 - ����ģ�飺
 - �������ԣ�
 - ����˵����
 - ����˵����
 - ���ļ�¼��
 **************************************************************************/
//��reply��Ϣ��������У�Ŀ����Ϊ���ܹ��Թ������ת��join_Reply��Ϣ
//����û�п���ѭ����������200����Ϣ������Ϊֹ
void pushReplyQueue(U16 addr, struct join_Reply r)
{
	replyQueue[replyQueueTop].nodeAddr=addr;
	replyQueue[replyQueueTop].reply=r;

	replyQueueTop++;
	replyQueueCount++;

	//UART_Send_Str("\t pushReplyQueue(): node=%d\t sum=%d\t top=%d\t nextHop=%d\n");//fprintf(fp,"\t pushReplyQueue(): node=%d\t sum=%d\t top=%d\t nextHop=%d\n",addr,replyQueueCount,replyQueueTop,r.nextAddr);
}	

/**************************************************************************
 - ����������void processJoinReply(void);//����join_Reply��Ϣ
 - ����ģ�飺
 - �������ԣ�
 - ����˵����
 - ����˵����
 - ���ļ�¼��
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
				//��Reply��Ϣ����msgCache��
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

				//���˽ڵ��Ƿ���ת�����Ա
				if( (i==tempReply.nextAddr)&&(i!=src) )
				{
					if (forwardTable[i].FW_FLAG!=1)
					{
						//��ת�����Ա������forwardTable
						forwardTable[i].srcAddr=tempReply.srcAddr;
						forwardTable[i].FW_FLAG=1;
						forwardTable[i].MID=tempReply.MID;
						forwardTable[i].timer=time(NULL);

						//UART_Send_Str("\t The node %d is a forward member\n");//fprintf(fp,"\t The node %d is a forward member\n",i);
					}

					//ת��join_Reply
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
					//��Դ�ڵ㣬����reply��һ��·�ɽ������
					//UART_Send_Str("The sourcer receive reply\n");	// fprintf(fp,"The sourcer receive reply\n");
				}
				else
				{
					//����ת����ڵ㲻��ת��reply��Ϣ
					//ʲôҲ����
				}//endif
			}
			else
			{
				//��������Ϣ
				//���߸ýڵ�����Լ�����
				//���߲����ھӽڵ�
				//ʲôҲ����
			}//endif
		}//endfor
	}//endwhile

#ifdef _DEBUG	
UART_Send_Str("End processJoinReply()\n");  
#endif //fprintf(fp,"End processJoinReply()\n");

}

/**************************************************************************
 - ����������U16 isEmptyReplyQueue(void);//�ж��Ƿ��������Ϊ��
 - ����ģ�飺
 - �������ԣ�
 - ����˵����
 - ����˵����
 - ���ļ�¼��
 **************************************************************************/
//�ж��Ƿ��������Ϊ��
U16 isEmptyReplyQueue()
{
	if ( replyQueueCount==0 )
		return 1;
	else return 0;
}

/**************************************************************************
 - ���������� //ȡ������Ϣ����tempReply��replyQueueAddr��//�޸�replyQueueTopֵ
 - ����ģ�飺
 - �������ԣ�
 - ����˵����
 - ����˵����
 - ���ļ�¼��
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
 //search in Msg_Cache���������Ϣ�������������Ƿ���ת����ڵ㣬����������ת��
 //if (SrcAddr, SeqNum, MID) exists in the Msg_Cache
 //     then discard this Join_Data;  //��������
 //     else if ((this node.FL_FLAG==1)&&(Time<TIME))
 //              then Broadcast this Join_Data packet;
 //              else node is not forwarding node or expired;
/**************************************************************************
 - ����������void sendData(void);//��������
 - ����ģ�飺
 - �������ԣ�
 - ����˵����  ��
 - ����˵����  ��
 - ���ļ�¼��
 **************************************************************************/
void sendData()		   //��������
{
	U16 i,j;
	//UART_Send_Str("\nBegin sendData()\n"); //	fprintf(fp,"\nBegin sendData()\n");
	
	//U8 msg[lineNum]="This is MSG.";//����
	msgQueue[0]=0;//0��Դ�ڵ�������ݶ��еȴ���������
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
					//ת����ڵ㣬ת������
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
 - ����������U16 isEmptyMsgQueue(void);//�ж��Ƿ����ݶ���Ϊ��
 - ����ģ�飺
 - �������ԣ�
 - ����˵����  ��
 - ����˵����
 - ���ļ�¼��
 **************************************************************************/
//�ж��Ƿ����ݶ���Ϊ��
U16 isEmptyMsgQueue()
{
	if (msgQueueCount==0)
		return 1;
	else return 0;
}

/**************************************************************************
 - ����������
 - ����ģ�飺
 - �������ԣ�
 - ����˵����
 - ����˵����
 - ���ļ�¼��
 **************************************************************************/
void pushMsgQueue(U16 addr)
{
	msgQueue[msgQueueTop]=addr;
	
	msgQueueTop++;
	msgQueueCount++;

	//UART_Send_Str("\t pushMsgQueue():   node=%d\t\n");//	 fprintf(fp,"\t pushMsgQueue():   node=%d\t\n",addr);
}

/**************************************************************************
 - ����������
 - ����ģ�飺
 - �������ԣ�
 - ����˵����
 - ����˵����
 - ���ļ�¼��
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
