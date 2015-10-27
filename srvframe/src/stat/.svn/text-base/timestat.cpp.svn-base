#include "timestat.h"
#include <stdio.h>
#include <string.h>

CTimeStat::CTimeStat()
{

}

CTimeStat::~CTimeStat()
{

}


void CTimeStat::Begin()
{
        gettimeofday(&m_beginTime, NULL);
}


void CTimeStat::End()
{
        gettimeofday(&m_endTime, NULL);
}

int CTimeStat::Init(timeval &beginTime, timeval &netEndTime,  timeval &endTime)
{
        memcpy(&m_beginTime, &beginTime, sizeof(timeval));
        memcpy(&m_netEndTime, &netEndTime, sizeof(timeval));
        memcpy(&m_endTime, &endTime, sizeof(timeval));
        return 0;
}

int CTimeStat::GetTime()
{
        int nUsec = m_endTime.tv_usec - m_beginTime.tv_usec + 1000000*(m_endTime.tv_sec - m_beginTime.tv_sec);
        return nUsec;
}

int CTimeStat::Clone(CTimeStat &timeStat)
{
        timeStat.Init(m_beginTime, m_netEndTime, m_endTime);
        return 0;
}

void CTimeStat::NetEnd()
{
        gettimeofday(&m_netEndTime, NULL);
}

int CTimeStat::GetNetTime()
{
        int nUsec = m_netEndTime.tv_usec - m_beginTime.tv_usec + 1000000*(m_netEndTime.tv_sec - m_beginTime.tv_sec);
        return nUsec;
}
