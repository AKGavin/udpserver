/*
 * FileName:       
 * Author:         wangqifeng  Version: 1.0  Date: 2006-12-30
 * Description:    
 * Version:        
 * Function List:  
 *                 1.
 * History:        
 *     <author>   <time>    <version >   <desc>
 */
#include "iparea.h"
#include "cmysql.h"
#include "toolkit.h"
#include "myfile.h"
#include "writeRunInfo.h"
#include <stdlib.h>
namespace MYFRAMECOMMON{
CIpAreaInfo::CIpAreaInfo()
{
        m_defautArea.nArea = 0;
        m_defautArea.nBeginIp = 0;
        m_defautArea.nEndIp = 0;
}

CIpAreaInfo::~CIpAreaInfo()
{
}

int CIpAreaInfo::LoadIPArea(string sHost, string sUser, string sPasswd, string sDbName)
{
        CMysql dbInfo;
        MYSQL_ROW row;
        char sSQLBuf[1024] = {0};
        try
        {
                dbInfo.Connect(sHost.c_str(), sUser.c_str(), sPasswd.c_str());
                //sprintf(sSQLBuf, "use %s", sDbName.c_str());
				snprintf(sSQLBuf, sizeof(sSQLBuf), "use %s", sDbName.c_str());
                dbInfo.Query(sSQLBuf);

                //sprintf(sSQLBuf, "select beginip, endip, areano from ipareainfo");
				snprintf(sSQLBuf, sizeof(sSQLBuf), "select beginip, endip, areano from ipareainfo");
                dbInfo.Query(sSQLBuf);
                while ((row=dbInfo.FetchRow()))
                {
                        TIPArea ipArea;
                        int i=0;
                        ipArea.nBeginIp = strtoul(row[i++], 0, 10);
                        ipArea.nEndIp= strtoul(row[i++], 0, 10);
                        ipArea.nArea = CToolKit::Atoi(row[i++]);
                        m_ipAreaList.insert(make_pair(ipArea.nEndIp, ipArea));
                }                        
        }
        catch (CMysqlException &e)
        {
                WriteRunInfo::WriteLog("Db error, sql %s, error %s", sSQLBuf, e.GetErrMsg());
                return -1;
        }
        return 0;
}


int CIpAreaInfo::LoadIPAreaFromFile(string sFileName)
{
        try
        {
                CMyFile myFile(sFileName);
                myFile.FOpen("r");
                char sBuf[1024] ={0};
                vector<string> strList;
                string sDelim = " ";
                int i=0;
                while (fgets(sBuf, 1024, myFile.GetFp()))
                {
                        strList.clear();
                        CToolKit::SplitString(sBuf, sDelim, strList);
                        if (strList.size() != 3)
                        {
                                continue;
                        }
                        TIPArea ipArea;
                        ipArea.nBeginIp = strtoul(strList[0].c_str(), 0, 10);
                        ipArea.nEndIp = strtoul(strList[1].c_str(), 0, 10);
                        ipArea.nArea = CToolKit::Atoi(strList[2].c_str());          
                        m_ipAreaList.insert(make_pair(ipArea.nEndIp, ipArea));
                        i++;
                }
                WriteRunInfo::WriteLog("Load ipareainfo ok, i is %d,  size is %d",i,  m_ipAreaList.size());
        }
        catch(CMyException &e)
        {
                WriteRunInfo::WriteLog("File op error, error %s",  e.GetErrMsg().c_str());
        }
        return 0;
}

TIPArea &CIpAreaInfo::GetIpArea(unsigned int nIp)
{
        map<unsigned int, TIPArea>::iterator mapItr = m_ipAreaList.lower_bound(nIp); 
        if (mapItr != m_ipAreaList.end())
        {
                return mapItr->second;
        }
        else
        {
                return m_defautArea;
        }
}
}
