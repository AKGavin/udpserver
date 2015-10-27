/*
 * FileName:       
 * Author:         wangqifeng  Version: 1.0  Date: 2005-4-8
 * Description:    
 * Version:        
 * Function List:  
 *                 1.
 * History:        
 *     <author>   <time>    <version >   <desc>
 */

#include "threadmutex.h"
#include "writeRunInfo.h"

namespace MYFRAMECOMMON{


CThreadMutex::CThreadMutex()
{        
        pthread_mutex_init(&m_threadMutex, NULL);
}

CThreadMutex::~CThreadMutex()
{  
        pthread_mutex_destroy(&m_threadMutex);
}

void CThreadMutex::Lock()
{
        pthread_mutex_lock(&m_threadMutex);
}

void CThreadMutex::UnLock()
{
        pthread_mutex_unlock(&m_threadMutex);
}
}

