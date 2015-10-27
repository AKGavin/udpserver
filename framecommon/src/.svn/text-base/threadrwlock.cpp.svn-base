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
#include "threadrwlock.h"
namespace MYFRAMECOMMON{
CThreadRwLock::CThreadRwLock()
{
	pthread_rwlock_init(&m_rwlock, NULL);
}

CThreadRwLock::~CThreadRwLock()
{
	pthread_rwlock_destroy(&m_rwlock);
}

int CThreadRwLock::GetReadLock()
{
	pthread_rwlock_rdlock(&m_rwlock);
	return 0;
}

int CThreadRwLock::GetWriteLock()
{
	pthread_rwlock_wrlock(&m_rwlock);
	return 0;
}

int CThreadRwLock::ReleaseLock()
{
	pthread_rwlock_unlock(&m_rwlock);
	return 0;
}
}
