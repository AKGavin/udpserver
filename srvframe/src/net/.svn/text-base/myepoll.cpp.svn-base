/*
 * FileName:       
 * Author:         wangqifeng  Version: 1.0  Date: 2005-8-26
 * Description:    
 * Version:        
 * Function List:  
 *                 1.
 * History:        
 *     <author>   <time>    <version >   <desc>
 */
#ifdef  __USE_EPOLL

#include "myepoll.h"
#include "myserver.h"

CRealEpoll::CRealEpoll()
{
        m_nEpollFd = 0;
        m_nEpollSize = 0;
        m_epollEvents = NULL;
}

CRealEpoll::~CRealEpoll()
{
        if (m_epollEvents != NULL)
        {
                delete[] m_epollEvents;
        }
        close(m_nEpollFd);
}

int CRealEpoll::Init(int epollSize, int epollWaitTime)
{
        m_nEpollWaitTime = epollWaitTime;
        m_nEpollSize = epollSize;
        m_nEpollFd = epoll_create(m_nEpollSize);
        if (m_nEpollFd == -1)
        {       
                WriteRunInfo::WriteLog("Epoll init failed, errno is %d, and error is %s", errno, strerror(errno));
                return -1;
        }
        m_epollEvents = new epoll_event[m_nEpollSize];        
        return 0;
}

int CRealEpoll::AddToEpoll(CSocketTask *pTask, int event)
{
        int tmpOprate = EPOLL_CTL_ADD;
        struct epoll_event tmpEvent;
        memset(&tmpEvent, 0, sizeof(epoll_event));
        tmpEvent.events = event;
        tmpEvent.data.ptr = pTask;
        epoll_ctl(m_nEpollFd, tmpOprate, pTask->GetSocket(), &tmpEvent);
        CMyserver::GetInstance()->GetCommonStat().AddNormalStat(ADDEPOLL);
        return 0;
}

int CRealEpoll::DelFromEpoll(CSocketTask *pTask)
{
        int tmpOprate = EPOLL_CTL_DEL;
        struct epoll_event tmpEvent;
        memset(&tmpEvent, 0, sizeof(epoll_event));
        tmpEvent.data.ptr = pTask;
        epoll_ctl(m_nEpollFd, tmpOprate, pTask->GetSocket(), &tmpEvent);
        CMyserver::GetInstance()->GetCommonStat().AddNormalStat(DELEPOLL);
        return 0;
}

int CRealEpoll::ModifyFromEpoll(CSocketTask *pTask, int event)
{
        int tmpOprate = EPOLL_CTL_MOD;
        struct epoll_event tmpEvent;
        tmpEvent.events = event;  
        tmpEvent.data.ptr = pTask;
        epoll_ctl(m_nEpollFd, tmpOprate, pTask->GetSocket(), &tmpEvent);
         CMyserver::GetInstance()->GetCommonStat().AddNormalStat(MODEPOLL);
        return 0;
}

int CRealEpoll::EpollWait()
{
        int  nfds =  epoll_wait(m_nEpollFd, m_epollEvents, m_nEpollSize,  m_nEpollWaitTime);
        if (nfds == -1)
        {
                WriteRunInfo::WriteRunLog("epoll_wait error(%d) is %s", errno, strerror(errno));      
                return 0;
        }      
        int tmpProcessNum=0;
        for (int i =0; i < nfds; i++)
        {
                //epoll_event *tmpEvents = m_epoll.GetEpollEvents();
                CSocketTask * tmpTask = (CSocketTask*)(m_epollEvents[i].data.ptr);
                if (tmpTask != NULL)
                {
                        tmpTask->HandleProcess(m_epollEvents[i].events);
                }
        }
        tmpProcessNum = nfds;
        return nfds;
}
#endif


