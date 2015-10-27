#ifndef __TIME_STAT_H
#define __TIME_STAT_H
#include <sys/time.h>

class CTimeStat
{
public:
        CTimeStat();
        ~CTimeStat();

        void Begin();
        void End();
        void NetEnd();
        int GetTime();
        int GetNetTime();

        int Clone(CTimeStat &timeStat);                

        int Init(timeval &beginTime, timeval &netEndTime,   timeval &endTime);
private:
        timeval  m_beginTime;
        timeval  m_netEndTime;
        timeval  m_endTime;
};
#endif
