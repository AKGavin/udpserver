#ifndef __LISTEN_TASK_H
#define __LISTEN_TASK_H

#include "sockettask.h"
#include "framecommon/framecommon.h"
using namespace std;

struct TListenBlock
{
    int nSock;
    CThreadMutex mutex;
    TListenBlock()
    {
        nSock = -1;
    }
};

/**
* dec:
* ClassName:CListenTask
* author: 
* version:
*CreateTime: 2005.8.24
*/
class CListenTask:public CSocketTask
{
public:
        /**
         *
         *  \param   taskId
         *  \param   iSocket
         *  \return   
         */
        CListenTask(CTcpSocket *pSocket, 
                CNetProcessThread*pWorkThread);
         
        ~CListenTask();
        virtual int Init();
        /**
         *  消息处理
         *  \param   *pMsg
         *  \return   int
         */
        int Process(CMsg *pMsg);        
        
        /**
         * 句柄被触发处理
         *  \return   int
         */
        int HandleProcess(int event);

protected:
        int DestroySelf();
        int PutMsgToSendList(const char *pBuf, int nLen);		
        int AddToEpoll();
        static CThreadMutex m_listenMutex;
};
#endif

