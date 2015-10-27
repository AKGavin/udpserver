#ifndef __ASYN_QUEUE_H
#define __ASYN_QUEUE_H

extern "C" {
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
}

#include <deque>

#include "commmsg.h"
using namespace std;


class CAsynQueue
{
public:
    CAsynQueue()
    {
        pthread_mutex_init(&m_mutex_write, NULL);
    }
    ~CAsynQueue()
        {
                pthread_mutex_destroy(&m_mutex_write);
        }

    int  put(CMsg *pMsg)
     {       
       pthread_mutex_lock(&m_mutex_write);
       m_queue.push_back(pMsg);
       pthread_mutex_unlock(&m_mutex_write);
       return 0;
   }
    
    CMsg*    get()
    {   
            CMsg *pMsg = NULL;
            pthread_mutex_lock(&m_mutex_write);
            deque<CMsg*>::iterator tmpItr = m_queue.begin();
            if (tmpItr != m_queue.end())
            {
                  pMsg  = *tmpItr;
                  m_queue.pop_front();
            }  
            pthread_mutex_unlock(&m_mutex_write);     
            return pMsg;
   }

    size_t  size()
    {
        return m_queue.size();
    }

private:
    pthread_mutex_t m_mutex_write;
    deque<CMsg*> m_queue;               /* 保存数据的队列 */
};



#endif /* __CLIB_QUEUE__ */

