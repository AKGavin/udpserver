/*
 * FileName:       
 * Author:         wangqifeng  Version: 1.0  Date: 2005-4-7
 * Description:    
 * Version:        
 * Function List:  
 *                 1.
 * History:        
 *     <author>   <time>    <version >   <desc>
 */
#ifndef  __THREAD_MUTEX_H
#define __THREAD_MUTEX_H
#include <pthread.h>
namespace MYFRAMECOMMON{

/**
* dec:
* ClassName:CThreadMutex
* author: 
* version:
*CreateTime: 2005.4.7
*/
class CThreadMutex
{
public:
         CThreadMutex();
         ~CThreadMutex();
         void Lock();
         void UnLock();
private:
        pthread_mutex_t  m_threadMutex;
};

class CThreadLock
{
public:
        CThreadLock(CThreadMutex *pMutex)
        {
            m_pMutex = pMutex;
            m_pMutex->Lock();
        }
        ~CThreadLock()
        {
            m_pMutex->UnLock();
        }
private:
        CThreadMutex *m_pMutex;
};
}
#endif

