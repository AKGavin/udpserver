/*
 * FileName:       
 * Author:         wangqifeng  Version: 1.0  Date: 2005-8-22
 * Description:    
 * Version:        
 * Function List:  
 *                 1.
 * History:        
 *     <author>   <time>    <version >   <desc>
 */
#ifndef __WORK_THREAD_h
#define __WORK_THREAD_h
#include "commonthread.h"
#include "commmsg.h"
#include <sys/time.h>
#include "task.h"
#include "framecommon/framecommon.h"
using namespace MYFRAMECOMMON;

const int THREADBUFLEN=200* 1024; //线程缓冲
/**
* dec:
* ClassName:CWorkThread
* author: 
* version:
*CreateTime: 2005.8.22
*/
class CWorkThread:public CCommonThread
{
public:
        CWorkThread(int iEntityType,  int iEntityId)
        {
                m_nEntityId = iEntityId;
                m_nEntityType = iEntityType;
                char *pTmpBuf = new char[THREADBUFLEN];
                m_threadBuf.AttachBuf(pTmpBuf, THREADBUFLEN);
        }
        
        virtual ~CWorkThread()
        {
        }

        
        /**
         *这个是工作线程运行的主函数, 主循环在这个函数里
         *  \return   int
         */
        virtual int Run()=0;
        virtual int PutMsgToInputList(CMsg *pMsg)=0;


        /**
         *工作线程初始化函数
         *  \return   int
         */
        virtual int Init();
   
  
        /**
         *
         *  \param   *pMsg
         *  \return   int
         */

        /**
         * 寻找任务
         *  \param   iTaskId
         *  \return   CTask *
         */
        CTask * FindTask(ULONG64        iTaskId);

        /**
         *获取实体id
         *  \return   int
         */
        int GetEntityId()
        {
                return m_nEntityId;
        }

        int GetEntityType()
        {
                return m_nEntityType;
        }

    
        /**
         *
         *  \param   *pTask
         *  \return   int
         */
        int AddTask(CTask *pTask);

        /**
         *
         *  \param   taskId
         *  \return   int
         */
        int DelTask(ULONG64         taskId);


        CBufMgr &GetThreadBuf()
        {
                return m_threadBuf;
        }

        void SendMsg(int destEntityType, int destEntityId, ULONG64 lTaskId,  int msgType,  CMsgBody *pMsgBody, ULONG64 lSrcTaskId=0);
protected:

        int TimerMsgProcess(CMsg *pMsg);

        /**
         *  消息处理函数是一个虚函数， 待各个实例实现
         *  \param   *msg
         *  \return   int
         */
        virtual int   Process(CMsg *pMsg) = 0; 

        //需要持续等待的任务， 
        //如果一步执行完毕的任务直接就处理完毕        
        map<ULONG64       ,  CTask*> m_taskList;
        
        int m_nEntityId;  //实体ＩＤ
        int m_nEntityType; //实体类型

        //每个线程用一些缓冲
        CBufMgr m_threadBuf;        
};
#endif
