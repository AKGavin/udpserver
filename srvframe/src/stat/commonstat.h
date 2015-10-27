#ifndef __COMMON_STAT_H
#define __COMMON_STAT_H
#include <string>
#include "framecommon/framecommon.h"
using namespace MYFRAMECOMMON;
using namespace std;

const int STATITEMNUM=3;


const int LASTTENMINUTE=0;
const int LASTONEHOUR=1;
const int LASTONEDAY=2;

const int PROCESSTIMENUM=3; //0~10000, 10000~50000, 50000~
const int FIRSTPROCESSTIME = 10000;
const int SECONDPROCESSTIME =50000;


const int MAXCOMMANDSTATNUM=100;


enum NORMALTYPE
{
        SOCKETERRORTYPE=0
};

struct TCommandStat
{
        int nType;
        char sCmdName[20];
        int nTotalOk;
        int nTotalFail;
        ULONG64 lTotalProcessTime;
        ULONG64 lTotalNetProcessTime;
        int nOk[PROCESSTIMENUM];
        int nFail[PROCESSTIMENUM];
        ULONG64 lProcessTime[PROCESSTIMENUM];
        int nUsed;
        TCommandStat()
        {
                nType = 0;
                memset(sCmdName, 0, 20);
                nTotalOk = 0;
                nTotalFail = 0;
                lTotalProcessTime = 0;
                lTotalNetProcessTime  = 0;      
                memset(nOk, 0, 4 * PROCESSTIMENUM);
                memset(nFail, 0, 4 *PROCESSTIMENUM);
                memset(lProcessTime, 0, 8*PROCESSTIMENUM);
                nUsed = 0;
        }
};

struct TNormalStat
{
        int nType;        
        int nNum;
        char sStatInfo[20];
        int nUsed;
        TNormalStat()
        {
                nType = 0;
                nNum = 0;
                memset(sStatInfo, 0, 20);
                nUsed = 0;
        }
};

struct TCommandStr
{
        int nType;
        char sCmdName[20];
};

//打印统计区间 
const int PROCESSTIME[]=
{
        0,
        FIRSTPROCESSTIME,
        SECONDPROCESSTIME
};


class CStatItem
{
public:
        CStatItem();
        ~CStatItem();
        void AddCommandStat(int nType, int nUsec, int nNetUsec, int nOk);
        void AddNormalStat(int nType);
        int Init(string &statInfo, int nIntervalTime);
        void PrintStat();
        void Update();

        void AddCommandStatItem(TCommandStr &commandStr);
        void AddNormalStatItem(TCommandStr &normalStat);
private:
        string m_sStatInfo;
        int m_nIntervalTime;
        int m_nPreUpdateTime;
        
        TCommandStat m_pCommandStat[MAXCOMMANDSTATNUM];
        TNormalStat    m_pNormalStat[MAXCOMMANDSTATNUM];
        CThreadMutex m_mutex;
        //char m_statStr[5*10240];
        stringstream m_statStr;
};

class CCommonStat
{
public:
        CCommonStat();
        ~CCommonStat();
        void AddCommandStat(int nType, int nUsec, int nNetUsec, int nOk);
        void AddNormalStat(int nType);
        void Update();
        void PrintStat();

        
        void AddCommandStatItem(TCommandStr &commandStr);
        void AddNormalStatItem(TCommandStr &normalStat);
private:
        CStatItem m_statList[STATITEMNUM];                	
};
#endif

