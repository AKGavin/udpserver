/*
 * FileName:       
 * Author:         wangqifeng  Version: 1.0  Date: 2006-1-10
 * Description:    
 * Version:        
 * Function List:  
 *                 1.
 * History:        
 *     <author>   <time>    <version >   <desc>
 */
#ifndef __SYN_WORKTHREAD_H
#define __SYN_WORKTHREAD_H
#include "workthread.h"
#include "synqueue.h"
/**
* dec:
* ClassName:CSynWorkThread
* author: 
* version:
*CreateTime: 2006.1.10
*/
class CSynWorkThread:public CWorkThread
{
public:
       CSynWorkThread(int iEntityType,  int iEntityId):CWorkThread(iEntityType, iEntityId)
       {
       }
       
    ~CSynWorkThread()
       {
       }
    
       int Run()
       {
             while (m_nRunFlag)
             {
                    CMsg *pMsg = m_inputList.get();
                    if (pMsg != NULL)
                    {
                            Process(pMsg);
                            delete pMsg;
                    }
             }
             return 0;
       }
       
       int Process(CMsg *pMsg) = 0; 
       int PutMsgToInputList(CMsg * pMsg)
       {
               m_inputList.put(pMsg);
               return 0;
       }
protected:
       CQueue  m_inputList;
};

#endif
