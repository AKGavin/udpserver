#ifndef __COM_POLL_H
#define __COM_POLL_H

#ifdef  __USE_EPOLL
#include <sys/epoll.h>
#endif
#include <sys/poll.h>

const int EPOLLWAITTIME=10;
const int EPOLLSIZE=1000;

#ifdef  __USE_EPOLL
#define MYPOLLERR   EPOLLERR
#define MYPOLLHUP   EPOLLHUP
#define MYPOLLIN    EPOLLIN
#define MYPOLLOUT  EPOLLOUT
#define MYPOLLET  EPOLLET
#define MYPOLLRDNORM 0
#else
#define MYPOLLERR   POLLERR
#define MYPOLLHUP  POLLHUP
#define MYPOLLIN     POLLIN
#define MYPOLLOUT  POLLOUT
#define MYPOLLET  0
#define MYPOLLRDNORM POLLRDNORM
#endif

class CSocketTask;
class CEpoll
{
public:
        CEpoll()
        {
        }
        
        virtual ~CEpoll()
        {
        }
        virtual int Init(int epollSize=EPOLLSIZE, int epollWaitTime=EPOLLWAITTIME) = 0;     
        virtual int AddToEpoll(CSocketTask *pTask, int event) = 0;

        virtual int DelFromEpoll(CSocketTask *pTask) = 0;    

        virtual int ModifyFromEpoll(CSocketTask *pTask, int event) = 0;    

        virtual int EpollWait() = 0;
};
#endif
