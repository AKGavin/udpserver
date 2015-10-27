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
#ifndef __SEND_TASK_H
#define __SEND_TASK_H
#include "commserver.h"




/**
* dec: 这个类会不停的向客户端push数据, 直到发送队列中有一个中止消息, 中止消息的消息体为空, 长度为0
* ClassName:CSendTask
* author: 
* version:
*CreateTime: 2005.12.8
*/
class CSendTask:public CSocketTask
{
public:
        CSendTask(CTcpSocket *pSocket, CNetProcessThread *pWorkThread, int nListType=0);
       
        virtual ~CSendTask();

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
//        int OnTimer(TTimeEvent &timeEvent);

        int DestroySelf(); 

        
        void PutSendMsg(CSendMsg *pSendMsg)
        {
                m_pSendMsgList.push_back(pSendMsg);
        }        
        void PutEndMsg();
        
        virtual void Routine(); //例行处理
protected:   
        CSendMsg* GetSendMsg();
        
        
        int AddToEpoll();         
        list<CSendMsg*>  m_pSendMsgList;
        CSendMsg *m_pSendMsg;
        int m_iCurSendLen;
        int SendData();
};


#endif

