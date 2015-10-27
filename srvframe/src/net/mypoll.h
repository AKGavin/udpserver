#ifndef __MY_POLL_H
#define __MY_POLL_H
#include <sys/poll.h>
#include <errno.h>
#include <stdio.h>
#include "compoll.h"
#include <string>
#include <list>
using namespace std;
//
//const int MAXPOLLSIZE = 2000;
//const int POLLWAITTIME=10;

class CMyPoll:public CEpoll
{
public:
        CMyPoll();
        ~CMyPoll();
        int Init(int epollSize=EPOLLSIZE, int epollWaitTime=EPOLLWAITTIME);     
        
        int AddToEpoll(CSocketTask *pTask, int event);

        int DelFromEpoll(CSocketTask *pTask);    

        int ModifyFromEpoll(CSocketTask *pTask, int event);    

        int EpollWait();   

        static string GetPollVersion()
        {
                string sVersion;
                #ifdef  __USE_EPOLL
                sVersion = "The poll is epoll";
                #else
                sVersion = "The poll is poll";
                #endif
                return sVersion;
        }
protected:        
        pollfd *m_pollfd;
        CSocketTask** m_taskList;
		list<int> m_freeListIndex;
        int m_nFdNum;
};
#endif
