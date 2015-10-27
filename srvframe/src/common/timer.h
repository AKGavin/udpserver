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
#ifndef __TIMER_H
#define __TIMER_H
#include <map>
#include <list>
#include <vector>
#include "framecommon/framecommon.h"
using namespace MYFRAMECOMMON;
const int MAXTIMERID=1000000000;

using namespace std;
class CTask;
const int TIMERCYCLE=3600;

struct TTimeEvent
{
        ULONG64  m_iTimerId;
        int  m_nTimerPoint;
        int  m_nEvent;
};

struct TTimerStr
{
        ULONG64        iTaskId;
        int iEntityType;
        int iEntityId;
        TTimeEvent event;        
};

/**
* dec:
* ClassName:CTimer
* author: 
* version:
*CreateTime: 2005.3.28
*/
class CTimer
{
public:
        CTimer();
        ~CTimer();
        /**
         * 增加一个定时事件
         *  \return   int
         */
        int AddOneTimer(TTimerStr &timer);            
        
        /**
         * 删除一个定时事件
         *  \return   int
         */
        int DeleteOneTimer(int iTimePoint,  ULONG64 iTimerId);
        
        /**
         *  定时事件检查
         *  \return   void
         */
        int TimerCheck();

        
        /**
         *  设置当前定时时间点
         *  \param   timePoint
         *  \return   void
         */
        void SetCurTimePoint(int timeSec)
        {
                m_nCurTimePoint = timeSec%TIMERCYCLE;
        }

        int GetCurTimePoint()
        {
                return m_nCurTimePoint;
        }

        int SendTimerOutMsg(TTimerStr &timerStr);

        void PrintTimer();

        static  ULONG64 GentTimerId();
private:
        int   m_nCurTimePoint; //当前循环的时间点
        int   m_nPreTimePoint; //上个循环的时间点
        vector <map<ULONG64, TTimerStr> >  m_timerList;       
        static ULONG64 m_iTimerId;
        static CThreadMutex m_sTimerIdMutex;
};

#endif
