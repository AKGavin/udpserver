/*
 * FileName:       
 * Author:         wangqifeng  Version: 1.0  Date: 2005-12-8
 * Description:    
 * Version:        
 * Function List:  
 *                 1.
 * History:        
 *     <author>   <time>    <version >   <desc>
 */
#ifndef __RECV_TASK_H
#define __RECV_TASK_H
#include "commserver.h"
#include "sockettask.h"
#include "commmsg.h"



/**
* dec:
* ClassName:CRecvTask
* author: 
* version:
*CreateTime: 2005.12.8
*/
class CRecvTask:public CSocketTask
{
public:
        CRecvTask(CTcpSocket *pSocket, CNetProcessThread *pWorkThread, int nListType=0);
        
         virtual ~CRecvTask();
        virtual int Init();
         /**
         *  处理消息方法
         *  \param   *msg
         *  \return   int
         */
        int Process(CMsg *pMsg);
        
        /**
         * 句柄处理
         *  \return   virtual int
         */
        int HandleProcess(int events);

        /**
         * 超时任务处理
         *  \param   &timeEvent
         *  \return   virtual int
         */
//        virtual int OnTimer(TTimeEvent &timeEvent);
        virtual int DestroySelf()=0;
protected:        
	int PutMsgToSendList(const char *pBuf, int nLen);	
        int  AddToEpoll();
        virtual int  RecvProcess()=0;
        int RecvBody();
        int RecvHttpRequest();
        int RecvGetHttpRequest();
        
        char m_recvBuf[MAXRECVLEN];        
        unsigned int m_nRecvLen;
        unsigned int m_iHasRecvLen;
        string m_sRecvStr;                 
        string m_sRecvHead;
};
#endif

