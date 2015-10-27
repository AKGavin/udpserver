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
#ifndef __NETPROCESS_THREAD_H
#define __NETPROCESS_THREAD_H
#include "commserver.h"
#include "asynqueue.h"
#include "sockettask.h"

const int CHECKTASKTIMEOUTINTERVAL=20;

/**
* dec:
* ClassName:CNetProcessThread
* author: 
* version:
*CreateTime: 2005.12.8
*/
class CNetProcessThread:public CWorkThread
{
public:
        CNetProcessThread(int iEntityType, int iEntityId):CWorkThread(iEntityType, iEntityId)
        {       
                m_nPreCheckTimeOut = time(NULL);
        }
        
        ~CNetProcessThread()
        {
        }

        int Run();
        
        int PutMsgToInputList(CMsg *pMsg);
        
        int Init();

        int HandleProcess();

        CEpoll *GetEpoll()
        {
                return m_epoll;
        }               

        CSocketPos* AddSocketToList(CSocketTask *pSockTask);
        int DelSocketTask(CSocketTask *pSockTask);
		CSocketTask *FindSocketTask(ULONG64 lTaskId);
protected:
        virtual int   OtherProcess();
        
        virtual int   Process(CMsg *pMsg);
        CAsynQueue  m_inputList;

        CEpoll *m_epoll;       

        int  CheckTaskTimeOut();
        int  m_nPreCheckTimeOut;
        list<CSocketTask*> m_pSocketList;
		map<ULONG64, CSocketTask*> m_pMapSocketList;
};
#endif
