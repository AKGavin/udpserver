/*
 * FileName:       
 * Author:         wangqifeng  Version: 1.0  Date: 2005-9-8
 * Description:    
 * Version:        
 * Function List:  
 *                 1.
 * History:        
 *     <author>   <time>    <version >   <desc>
 */
#include "sockettask.h"
#include "workthread.h"
#include "netprocessthread.h"

CThreadMutex CSocketTask::m_sMutex;
ULONG64 CSocketTask::m_sCurTaskId = 0;

ULONG64 CSocketTask::GenTaskId()
{
	ULONG64 tmpTaskId;
	m_sMutex.Lock();
	tmpTaskId = m_sCurTaskId++;
	m_sMutex.UnLock();
	return tmpTaskId;
}


CSocketTask::CSocketTask(CTcpSocket *pTcpSocket, CNetProcessThread* pWorkThread,int nListType)
{                
        m_nTaskTimeOut = CMyserver::GetInstance()->GetBaseConf().GetConfigItem().nSocketTimeout;
        m_pTcpSocket = pTcpSocket;   
        if (m_pTcpSocket != NULL)
            m_pTcpSocket->SetNonBlocking();
        
        m_nStartTime=CMyserver::GetInstance()->GetTimeVal().tv_sec;
        //加入链表
        m_lTaskId = GenTaskId();

	//by:fwx
	m_nListType = nListType;		
        m_status=0;
        m_pThread = pWorkThread;
	m_pollPos=-1;
        m_pPos = pWorkThread->AddSocketToList(this);       
}

CSocketTask::~CSocketTask()
{            
        //删除所有的epoll事件
        DelFromEpoll();
        if (m_pTcpSocket != NULL)
                delete m_pTcpSocket;
        m_pThread->DelSocketTask(this);

		if (m_pPos != NULL)
			delete m_pPos;
};
	
int CSocketTask::CheckTimeOut()
{        
        if (m_nTaskTimeOut == -1)
        {
                return 0;
        }
        
        int ret = 0;
        if ((CMyserver::GetInstance()->GetTimeVal().tv_sec - m_nStartTime) > m_nTaskTimeOut)
        {
                CMyserver::GetInstance()->GetCommonStat().AddNormalStat(SOCKETTIMEOUTSTAT);
                DestroySelf();
                ret = 1;
        }
        return ret;
}


int CSocketTask::DelFromEpoll()
{
        if (m_pTcpSocket == NULL)
        {
                return 0;
        }
        
        CNetProcessThread *pNetThread = (CNetProcessThread*)m_pThread;
        CEpoll *pEpoll  = pNetThread->GetEpoll();
        pEpoll->DelFromEpoll(this);
        return 0;
}

void CSocketTask::SetLinger()
{
//    struct linger {
//        u_short l_onoff;
//        u_short l_linger;
//    };
    struct  linger sLinger;
    sLinger.l_onoff = 1;
    sLinger.l_linger=0;
    m_pTcpSocket->SetSockOpt(SOL_SOCKET,SO_LINGER, (const char*)&sLinger, sizeof(linger));
}

CSocketPos* CSocketTask::GetPos()
{
    return m_pPos;
}

int CSocketTask::GetPollPos()
{
	return m_pollPos;
}

void CSocketTask::SetPollPos(int pos)
{	
	m_pollPos = pos;
}

