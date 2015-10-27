/*
 * FileName:       
 * Author:         wangqifeng  Version: 1.0  Date: 2006-6-17
 * Description:    
 * Version:        
 * Function List:  
 *                 1.
 * History:        
 *     <author>   <time>    <version >   <desc>
 */
#ifndef __MAINTAIN_TASK_H
#define __MAINTAIN_TASK_H
#include "task.h"
class CMaintainTask:public CTask
{
public:
        CMaintainTask(ULONG64   taskId,  CWorkThread *pThread);
     
        
        ~CMaintainTask()
        {                
        }
        int Process(CMsg *pMsg);

        int OnTimer(TTimeEvent &timeEvent);

protected:
//        virtual int OtherTimerProcess(TTimeEvent &timeEvent);
        int DestroySelf();
};
#endif

