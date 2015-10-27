/*
 * FileName:       
 * Author:         wangqifeng  Version: 1.0  Date: 2008-7-15
 * Description:    
 * Version:        
 * Function List:  
 *                 1.
 * History:        
 *     <author>   <time>    <version >   <desc>
 */
#ifndef __THREAD_RWLOCK_H
#define __THREAD_RWLOCK_H
#include <pthread.h>
namespace MYFRAMECOMMON{
/**
* dec:
* ClassName:CThreadRwLock
* author: 
* version:
*CreateTime: 2008.7.15
*/
class CThreadRwLock
{
public:
    CThreadRwLock();
    ~CThreadRwLock();
	int GetReadLock();
	int GetWriteLock();
	int ReleaseLock();
private:
	pthread_rwlock_t m_rwlock;
};
}
#endif

