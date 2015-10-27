#include "mypoll.h"
#include "sockettask.h"
CMyPoll::CMyPoll()
{
    m_nFdNum = 0;
    m_pollfd = NULL;
    m_taskList = NULL;
}

CMyPoll::~CMyPoll()
{
    if (m_pollfd != NULL)
    {
        delete [] m_pollfd;
    }

    if (m_taskList != NULL)
    {
        delete[] m_taskList;
    }
}

int CMyPoll::Init(int epollSize, int epollWaitTime)
{
    m_pollfd = new pollfd[EPOLLSIZE];
    m_taskList = new CSocketTask*[EPOLLSIZE];
    for (int i=0; i < EPOLLSIZE; i++)
    {
        m_pollfd[i].fd = -1;
        m_pollfd[i].events = 0;
        m_pollfd[i].revents = 0;
        m_taskList[i] = NULL;
		//提高效率为了减少句柄加入fd的时间
		m_freeListIndex.push_back(i);
	}
    return 0;
}


int CMyPoll::AddToEpoll(CSocketTask *pTask, int event)
{     
    int i = 0;
	if (m_freeListIndex.begin() == m_freeListIndex.end())
		return -1;
	//释放下标
	list<int>::iterator listItr = m_freeListIndex.begin();
	i=*listItr;
	m_freeListIndex.erase(listItr);
	
	if (m_pollfd[i].fd == -1)
    {
        m_pollfd[i].fd = pTask->GetSocket();
        m_pollfd[i].events = event;
        m_pollfd[i].revents = 0;
        m_taskList[i] = pTask;
        m_nFdNum ++;
		pTask->SetPollPos(i);        
    }
	return 0;
	/*
    for (i=0; i < EPOLLSIZE; i++)
    {
        if (m_pollfd[i].fd == -1)
        {
            m_pollfd[i].fd = pTask->GetSocket();
            m_pollfd[i].events = event;
            m_pollfd[i].revents = 0;
            m_taskList[i] = pTask;
            m_nFdNum ++;
			pTask->SetPollPos(i);
            break;
        }
    }
    if (i >= EPOLLSIZE)
    {
        return -1;
    }
    */
    return 0;
}

int CMyPoll::ModifyFromEpoll(CSocketTask *pTask, int event)
{
	int ret=-1;
	int nPos=pTask->GetPollPos();
	if (nPos >=0 && nPos < EPOLLSIZE)
	{
		if (m_pollfd[nPos].fd == pTask->GetSocket())
		{
			m_pollfd[nPos].events = event;
            m_pollfd[nPos].revents = 0;
			ret=0;
		}
	}
    return -1;
}

int CMyPoll::DelFromEpoll(CSocketTask *pTask)
{	
	int nPos = pTask->GetPollPos();
	int ret=-1;
	if (nPos >=0 && nPos < EPOLLSIZE)
	{
		if (m_pollfd[nPos].fd == pTask->GetSocket())
	    {
	        m_pollfd[nPos].fd = -1;
	        m_taskList[nPos] = NULL;
	        m_pollfd[nPos].events = 0;
	        m_pollfd[nPos].revents = 0;
	        m_nFdNum --;
			//将空闲的下标加入
			m_freeListIndex.push_back(nPos);
			ret=0;
	    }
	}	
	return ret;
}

int CMyPoll::EpollWait()
{
    int  nfds =  poll(m_pollfd, EPOLLSIZE, EPOLLWAITTIME);        
    if (nfds <= 0)
    {
        if (nfds < 0)
            WriteRunInfo::WriteLog("poll nfds%d error(%d) is %s",nfds, errno, strerror(errno));      
        return 0;
    }
    int tmpProcessNum=0;
    for (int i =0; i < EPOLLSIZE; i++)
    {
        if ((m_pollfd[i].fd != -1) && (m_pollfd[i].events != 0) && (m_pollfd[i].revents != 0))
        {
            m_taskList[i]->HandleProcess(m_pollfd[i].revents);
            tmpProcessNum++;
            if (m_taskList[i] != NULL) //如果句柄还存活的话把回调事件清零
            {
                m_pollfd[i].revents = 0;
            }
        }
        if (tmpProcessNum == nfds)
        {
            break;
        }
    }
    return nfds;
}

