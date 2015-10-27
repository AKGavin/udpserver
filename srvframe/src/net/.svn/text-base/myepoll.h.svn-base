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
#ifndef  __MYEPOLL_H__
#define __MYEPOLL_H__
#include "sys/epoll.h" 
#include "errno.h"
#include "sockettask.h"
#include "compoll.h"

/**
* dec:
* ClassName:CEpoll
* author: 
* version:
*CreateTime: 2005.4.18
*/
class CRealEpoll:public CEpoll
{
public:
     CRealEpoll();
     int Init(int epollSize=EPOLLSIZE, int epollWaitTime=EPOLLWAITTIME);
     
     ~CRealEpoll();
     
     int AddToEpoll(CSocketTask *pTask, int event);
     
     int DelFromEpoll(CSocketTask *pTask);    
     
     int ModifyFromEpoll(CSocketTask *pTask, int event);    
     
     int EpollWait();   
private:
     int  m_nEpollFd;
     int m_nEpollSize;
     int m_nEpollWaitTime;
     struct epoll_event *m_epollEvents;
};
#endif
#endif
