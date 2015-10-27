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
#ifndef  __TASK_H
#define __TASK_H
#include "timer.h"
#include "commmsg.h"
#include "framecommon/framecommon.h"
using namespace MYFRAMECOMMON;


const int INITSTATUS=0;  //初始化状态
const int ENDSTATUS=1000;//结束状态


class CWorkThread;
/**
* dec:
* ClassName:CTask
* author: 
* version:
*CreateTime: 2005.8.22
*/
class CTask
{
public:
        /**
         *任务ID,  这个任务所在的线程
         *  \param   taskId
         *  \param   *pThread
         *  \return   
         */
        CTask(ULONG64   taskId,  CWorkThread *pThread);
        
        /**
         *
         *  \return   virtual
         */
        virtual  ~CTask();
        
        /**
         *  处理消息方法
         *  \param   *msg
         *  \return   int
         */
        virtual int Process(CMsg *pMsg)=0;
        

        /**
         * 超时任务处理
         *  \param   &timeEvent
         *  \return   virtual int
         */
        virtual int OnTimer(TTimeEvent &timeEvent);

        virtual int CheckTimeOut();
                
        /**
         * 删除所有定时器
         *  \return   int
         */
        int DeleteAllTimer();
        
        /**
         *增加一个定时器
         *  \return   int
         */
        int AddOneTimer(int event, int timeLength);
        
        /**
         *删除一个定时器
         *  \return   int
         */
        int DeleteOneTimer(ULONG64 iTimerId);        

        /**
         *直接删除一个定时器, 不通知定时线程
         *  \return   int
         */
        int DirectDeleteOneTimer(ULONG64 iTimerId);        
        

        /**
         * 获取任务ID
         *  \return   int
         */
        int GetTaskId()
        {
                return m_iTaskId;
        }

        int GetTaskType()
        {
                return m_iTaskType;
        }

        void SetTaskType(int iTaskType)
        {
                m_iTaskType = iTaskType;
        }


        static ULONG64  GentTaskId();

        static int GetTaskNum()
        {
                return m_nSTaskNum;
        }
        //static int DeleteTask(ULONG64 lTaskId, CWorkThread *pThread);

protected:        
        int SendTimerOutMsg(int msgType,  TTimeEvent &timerStr);

        /**
         *删除这个任务本身
         *  \return    int
         */
        virtual int DestroySelf()=0;

        void UpdateTaskNum(int nNum);
        
        ULONG64        m_iTaskId; //任务ID                
        //时间队列表
        map<ULONG64, TTimeEvent>  m_timerList;
        //任务的状态
        int m_status;

        int m_iTaskType;
        CWorkThread *m_pThread; //线程对象

        int  m_nTaskTimeOut;
        int  m_nStartTime;

        static ULONG64     m_lCurTaskId;
        static CThreadMutex m_taskIdMutex;
        
        
        static int m_nSTaskNum;
        static CThreadMutex m_nTaskNumMutex;
};

#endif

