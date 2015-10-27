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
#ifndef  __MAINTAIN_THREAD_H
#define  __MAINTAIN_THREAD_H
#include "workthread.h"
#include "asynqueue.h"
#include "maintaintask.h"

/**
* dec:
* ClassName:CMaintainThread
* author: 
* version:
*CreateTime: 2005.8.26
*/
class CMaintainThread:public CWorkThread
{
public:
        /**
         *
         *  \param   iEntityType
         *  \param   iEntityId
         */
        CMaintainThread(int iEntityType, int iEntityId):CWorkThread(iEntityType, iEntityId)
        {
//                m_pMaintainTask = NULL;
        }
        
        ~CMaintainThread()
        {
        }

        int Run();

        /**
         *
         *  \param   pMsg
         *  \return   int
         */
        int PutMsgToInputList(CMsg * pMsg);

        int Init();        
protected:
         /**
         *  消息处理函数是一个虚函数， 待各个实例实现
         *  \param   *msg
         *  \return   int
         */
        int   Process(CMsg *pMsg);         


//        CMaintainTask *m_pMaintainTask; // 维护任务         
        CAsynQueue m_inputList;     
};


#endif

