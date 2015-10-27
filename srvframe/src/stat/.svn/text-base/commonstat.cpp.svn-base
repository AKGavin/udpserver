#include "commonstat.h"
#include "myserver.h"

CStatItem::CStatItem()
{
        m_nPreUpdateTime = CToolKit::GetCurZeroTime(time(NULL));
}

CStatItem::~CStatItem()
{
}

int CStatItem::Init(string &statInfo, int nIntervalTime)
{
        m_sStatInfo = statInfo;
        m_nIntervalTime = nIntervalTime;
        return 0;
}

void CStatItem::AddCommandStat(int nType, int nUsec, int nNetUsec,  int nOk)
{
       m_mutex.Lock();       
       if (nType >= 0 && nType < MAXCOMMANDSTATNUM && nUsec > 0)
       {
                if (m_pCommandStat[nType].nUsed == 0)
                {
                        m_mutex.UnLock();
                        return;
                }
                
                
                if (nOk == 0)
                {
                     m_pCommandStat[nType].nTotalOk ++;                     
                }
                else
                {
                     m_pCommandStat[nType].nTotalFail ++;
                }
                if (nUsec > 0)  
                        m_pCommandStat[nType].lTotalProcessTime += nUsec;
                if (nNetUsec > 0)
                        m_pCommandStat[nType].lTotalNetProcessTime += nNetUsec;
                for (int i=0; i < PROCESSTIMENUM; i++)
                {
                        if (i < PROCESSTIMENUM-1)
                        {
                                if (nUsec > PROCESSTIME[i] && nUsec <= PROCESSTIME[i+1])
                                {
                                        if (nOk == 0)
                                        {
                                                m_pCommandStat[nType].nOk[i] ++;
                                        }
                                        else
                                        {
                                                m_pCommandStat[nType].nFail[i] ++;
                                        }
                                        m_pCommandStat[nType].lProcessTime[i] += nUsec;
                                        break;
                                }
                        }
                        else
                        {
                                if (nOk == 0)
                                {
                                        m_pCommandStat[nType].nOk[i] ++;
                                }
                                else
                                {
                                        m_pCommandStat[nType].nFail[i] ++;
                                }
                                m_pCommandStat[nType].lProcessTime[i] += nUsec;
                        }
                }                
       }
       m_mutex.UnLock();
}

void CStatItem::AddNormalStat(int nType)
{
        m_mutex.Lock();     
        if (nType >=0 && nType < MAXCOMMANDSTATNUM)
        {
                if (m_pNormalStat[nType].nUsed == 0)
                {
                        m_mutex.UnLock();
                        return;
                }
                m_pNormalStat[nType].nNum++;
        }
        m_mutex.UnLock();     
}


void CStatItem::Update()
{      
        int nowTime = CMyserver::GetInstance()->GetTimeVal().tv_sec;
        if (nowTime - m_nPreUpdateTime < m_nIntervalTime)
        {
                return;
        }
        PrintStat();
        m_nPreUpdateTime = nowTime;                
        int i=0;
        for (i=0; i < MAXCOMMANDSTATNUM; i++)
        {
                m_pCommandStat[i].nTotalOk = 0;
                m_pCommandStat[i].nTotalFail = 0;
                m_pCommandStat[i].lTotalProcessTime= 0;
                m_pCommandStat[i].lTotalNetProcessTime = 0;
                for (int j=0; j < PROCESSTIMENUM; j++)
                {
                        m_pCommandStat[i].nOk[j] = 0;
                        m_pCommandStat[i].nFail[j] = 0;
                        m_pCommandStat[i].lProcessTime[j] = 0;
                }
        }
        
        for (i=0; i < MAXCOMMANDSTATNUM; i++)
        {
                m_pNormalStat[i].nType = 0;
                m_pNormalStat[i].nNum= 0;
        }

}

void CStatItem::PrintStat()
{        
        //char *pEnd = m_statStr + snprintf(m_statStr, sizeof(m_statStr), "%s\n", m_sStatInfo.c_str());       
        m_statStr.str("");
        m_statStr << m_sStatInfo;
        int i=0;                
        for (i=0; i < MAXCOMMANDSTATNUM; i++)
        {                
                if (m_pCommandStat[i].nUsed == 0)
                {
                        continue;
                }
                
                int nAvgTime = 0;
                int nAvgNetTime = 0;
                int nTotalNum = m_pCommandStat[i].nTotalOk + m_pCommandStat[i].nTotalFail;
                if (nTotalNum > 0)
                {
                        nAvgTime = m_pCommandStat[i].lTotalProcessTime/nTotalNum;                        
                        nAvgNetTime  = m_pCommandStat[i].lTotalNetProcessTime/nTotalNum;                        
                }
//               pEnd += snprintf(pEnd, sizeof(m_statStr) - (pEnd-m_statStr), "[%s, %d, %d, %d, %d]", m_pCommandStat[i].sCmdName, m_pCommandStat[i].nTotalOk,
//                        m_pCommandStat[i].nTotalFail, nAvgTime, nAvgNetTime);
                m_statStr << "[" << m_pCommandStat[i].sCmdName << "," << m_pCommandStat[i].nTotalOk << "," 
                    << m_pCommandStat[i].nTotalFail << "," << nAvgTime << "," << nAvgNetTime << "]";
        }

        for (i=0; i < MAXCOMMANDSTATNUM; i++)
        {
                if (m_pNormalStat[i].nUsed == 0)
                {
                        continue;
                }
                //pEnd += snprintf(pEnd, sizeof(m_statStr) - (pEnd-m_statStr), "[%s:%d]", m_pNormalStat[i].sStatInfo, m_pNormalStat[i].nNum);
                m_statStr << "[" << m_pNormalStat[i].sStatInfo << ":" << m_pNormalStat[i].nNum << "]";
        }
        WriteRunInfo::WriteStat("%s", m_statStr.str().c_str());
}


void CStatItem::AddCommandStatItem(TCommandStr &commandStr)
{
        if (commandStr.nType < 0 || commandStr.nType >= MAXCOMMANDSTATNUM)
        {
                return;
        }        
        //strcpy(m_pCommandStat[commandStr.nType].sCmdName, commandStr.sCmdName);
		strncpy(m_pCommandStat[commandStr.nType].sCmdName, commandStr.sCmdName, sizeof(m_pCommandStat[commandStr.nType].sCmdName));
        m_pCommandStat[commandStr.nType].nType = commandStr.nType;
        m_pCommandStat[commandStr.nType].nUsed = 1;
}

void CStatItem::AddNormalStatItem(TCommandStr &normalStat)
{
        if (normalStat.nType < 0 || normalStat.nType >= MAXCOMMANDSTATNUM)
        {
                return;
        }
        m_pNormalStat[normalStat.nType].nType = normalStat.nType;
        //strcpy(m_pNormalStat[normalStat.nType].sStatInfo, normalStat.sCmdName);
		strncpy(m_pNormalStat[normalStat.nType].sStatInfo, normalStat.sCmdName, sizeof(m_pNormalStat[normalStat.nType].sStatInfo));
        m_pNormalStat[normalStat.nType].nUsed = 1;
}



CCommonStat::CCommonStat()
{
        string tmpStr = "Last ten minutes stat info:";        
        m_statList[LASTTENMINUTE].Init(tmpStr, TENMINUTE);


        tmpStr = string("Last one hour stat info:");
        m_statList[LASTONEHOUR].Init(tmpStr, ONEHOUR);

        tmpStr = string("Last one day stat info:");
        m_statList[LASTONEDAY].Init(tmpStr, ONEDAY);

        //默认增加socketerror的统计类型
        TCommandStr cmdStr;
        cmdStr.nType = SOCKETERRORTYPE;
        //strcpy(cmdStr.sCmdName, "SocketError");
		strncpy(cmdStr.sCmdName, "SocketError", sizeof(cmdStr.sCmdName));
        AddNormalStatItem(cmdStr);
}

CCommonStat::~CCommonStat()
{
}
        

void CCommonStat::AddCommandStat(int nType, int nUsec, int nNetUsec,  int nOk)
{
        for (int i=0; i < STATITEMNUM; i++)
        {
                m_statList[i].AddCommandStat(nType, nUsec,nNetUsec,  nOk);
        }
}

void CCommonStat::Update()
{
        for (int i=0; i < STATITEMNUM; i++)
        {
                m_statList[i].Update();
        }
}

void CCommonStat::PrintStat()
{
        for (int i=0; i < STATITEMNUM; i++)
        {
                m_statList[i].PrintStat();
        }
}

void CCommonStat::AddNormalStat(int nType)
{
        for (int i=0; i < STATITEMNUM; i++)
        {
                m_statList[i].AddNormalStat(nType);
        }
}

void CCommonStat::AddCommandStatItem(TCommandStr &commandStr)
{
        for (int i=0; i < STATITEMNUM; i++)
        {
                m_statList[i].AddCommandStatItem(commandStr);
        }

}

void CCommonStat::AddNormalStatItem(TCommandStr &normalStat)
{
        for (int i=0; i < STATITEMNUM; i++)
        {
                m_statList[i].AddNormalStatItem(normalStat);
        }
}


