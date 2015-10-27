/*
 * FileName:       
 * Author:         wangqifeng  Version: 1.0  Date: 2006-7-25
 * Description:    
 * Version:        
 * Function List:  
 *                 1.
 * History:        
 *     <author>   <time>    <version >   <desc>
 */
#ifndef __RECVANDSENDTASK_H
#define __RECVANDSENDTASK_H
#include "recvtask.h"
#include "framecommon/framecommon.h"
using namespace MYFRAMECOMMON;

class CRecvAndSendTask:public CRecvTask
{
public:
        CRecvAndSendTask(CTcpSocket *pSocket, CNetProcessThread *pWorkThread, int nListType=0);
        virtual ~CRecvAndSendTask();
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
        virtual int HandleProcess(int events);

        virtual int DestroySelf()=0;
protected:        
        virtual int RecvProcess()=0;
        virtual int SendProcess()=0;

        int SendData();

        
        int  AddToEpoll();         
        CSendMsg *GetMsgFromSendList();
        /**
         *
         *  \param   *pBuf
         *  \param   nLen
         *  \return   int
         */
        virtual int PutMsgToSendList(const char *pBuf, int nLen);

        /**
         *
         *  \param   *pBuf
         *  \param   nLen
         *  \return   int
         */
        int SendBufProcess(const char *pBuf, int nLen);
     
        CMyList<CSendMsg*> m_sendList;
        unsigned int m_iCurSendLen;
        CSendMsg* m_pCurSendMsg;
};

#endif
