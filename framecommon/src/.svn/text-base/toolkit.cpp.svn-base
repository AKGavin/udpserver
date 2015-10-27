/*
 * FileName:       
 * Author:         wangqifeng  Version: 1.0  Date: 2005-11-1
 * Description:    
 * Version:        
 * Function List:  
 *                 1.
 * History:        
 *     <author>   <time>    <version >   <desc>
 */ 
#include <uuid/uuid.h>
#include <stdint.h>
#include "toolkit.h"
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <iconv.h>
#include <errno.h>
#include <sstream>
#include <sys/time.h>
#include <sys/times.h>
#include "csha1.h"
#include "myfile.h"
#include "myexception.h"
#include "writeRunInfo.h"
namespace MYFRAMECOMMON{


int CToolKit::m_isInitRand = 0;

string CToolKit::GenUuid()
{
        uuid_t out;
        uuid_generate(out);
        char sOut[40]={0};
        uuid_unparse(out, sOut);
        return string(sOut);
}


int CToolKit::GenRandomStr(string &sStr, int iLen)
{
        if (m_isInitRand == 0)
        {
                srandom(time(NULL));
                m_isInitRand = 1;
        }
        
        for (int i=0; i < iLen; i++) //数字加字母
        {
                long randint = random(); 
                int tmp = randint%36;
                char tmpChar;
                if (tmp < 26)
                {
                        tmpChar = 'a' + tmp;                        
                }
                else
                {
                        tmpChar = '0' + tmp -26;                        
                }
                sStr.append(1, tmpChar);
        }
        return 0;
}




int CToolKit::Atoi(const char *pBuf)
{
        if (pBuf == NULL)
        {
                return 0;
        }
        return atoi(pBuf);
}

float CToolKit::Atof(const char *pBuf)
{
        if (pBuf == NULL)
        {
                return 0.0;
        }
        return atof(pBuf);
}

string CToolKit::STRING(const char *pBuf)
{
        if (pBuf == NULL)
        {
                return string("");
        }
        return string(pBuf);
}

ULONG64 CToolKit::Atol64(const char *pBuf)
{
        if (pBuf == NULL)
        {
                return 0;
        }
        ULONG64 tmpLong;
        sscanf(pBuf, "%llu", &tmpLong);
        return tmpLong;
}

int CToolKit::CaptialToLow(string &sStr)
{
        int len = sStr.length();
        string tmpStr;
        for (int i=0; i < len; i++)
        {
                if (sStr[i] >= 'A' && sStr[i] <= 'Z')
                {
                        char ch = sStr[i] + 'a' - 'A';
                        tmpStr.append(1, ch);
                }
                else
                {
                        tmpStr.append(1,  sStr[i]);
                }
        }
        sStr = tmpStr;
        return 0;
}

int  CToolKit::GetCurZeroTime(time_t nTime)
{
        tm tmptm;
        localtime_r(&nTime, &tmptm);
        tmptm.tm_hour =0;
        tmptm.tm_min = 0;
        tmptm.tm_sec =0;
        int nZeroTime = mktime(&tmptm);
        return nZeroTime;
}


string CToolKit::Secd2Time(time_t lSecd)
{
        char psDateTime[50];
        int iYear, iMonth, iDay, iHour, iMin, iSec;
        struct tm tmstu;
        localtime_r(&lSecd, &tmstu);
                
        iYear = tmstu.tm_year+1900;
        iMonth = tmstu.tm_mon+1;
        iDay = tmstu.tm_mday;
        iHour = tmstu.tm_hour;
        iMin = tmstu.tm_min;
        iSec = tmstu.tm_sec;

        //sprintf(psDateTime, "%04d-%02d-%02d %02d:%02d:%02d", iYear, iMonth, iDay, iHour, iMin, iSec);
		snprintf(psDateTime, sizeof(psDateTime), "%04d-%02d-%02d %02d:%02d:%02d", iYear, iMonth, iDay, iHour, iMin, iSec);
        return string(psDateTime);
}

string CToolKit::Secd2Time1(time_t lSecd)
{
        char psDateTime[50];
        int iYear, iMonth, iDay, iHour, iMin, iSec;
        struct tm tmstu;
        localtime_r(&lSecd, &tmstu);
                
        iYear = tmstu.tm_year+1900;
        iMonth = tmstu.tm_mon+1;
        iDay = tmstu.tm_mday;
        iHour = tmstu.tm_hour;
        iMin = tmstu.tm_min;
        iSec = tmstu.tm_sec;

        //sprintf(psDateTime, "%04d%02d%02d%02d%02d%02d", iYear, iMonth, iDay, iHour, iMin, iSec);
		snprintf(psDateTime, sizeof(psDateTime), "%04d%02d%02d%02d%02d%02d", iYear, iMonth, iDay, iHour, iMin, iSec);
        return string(psDateTime);
}


string CToolKit::Secd2Date(time_t lSecd)
{
        char psDateTime[20] = {0};
        struct tm tmstu;
        localtime_r(&lSecd, &tmstu);
        //sprintf(psDateTime, "%04d%02d%02d", tmstu.tm_year+1900, tmstu.tm_mon + 1, tmstu.tm_mday);
		snprintf(psDateTime, sizeof(psDateTime), "%04d%02d%02d", tmstu.tm_year+1900, tmstu.tm_mon + 1, tmstu.tm_mday);
        return string(psDateTime);
}


time_t CToolKit::DateTimeToSec(const string &sDateTime) //YYYY-MM-DD HH:MM:SS
{
        vector<string> tmpList;
        string sDelim = " ";
        SplitString(sDateTime, sDelim, tmpList);
        if (tmpList[0].length() != 10)
        {
                return 0;
        }

        if (tmpList[1].length() != 8)
        {
                return 0;
        }
        string sYear = tmpList[0].substr(0,4);
        string sMon = tmpList[0].substr(5,2);
        string sDay = tmpList[0].substr(8,2);
        string sHour = tmpList[1].substr(0,2);
        string sMin = tmpList[1].substr(3,2);
        string sSec = tmpList[1].substr(6,2);
                
        tm tmpTm;
        tmpTm.tm_year = atoi(sYear.c_str()) - 1900;
        tmpTm.tm_mon = atoi(sMon.c_str()) - 1;
        tmpTm.tm_mday = atoi(sDay.c_str());
        tmpTm.tm_hour = atoi(sHour.c_str());
        tmpTm.tm_min = atoi(sMin.c_str());
        tmpTm.tm_sec = atoi(sSec.c_str());
        return mktime(&tmpTm);
}

string CToolKit::SecToDateTime(time_t nTime)
{
        char tmpBuf[30] = {0};
        tm tmpTm;
        localtime_r(&nTime, &tmpTm);
        //sprintf(tmpBuf, "%04d-%02d-%02d %02d:%02d:%02d", tmpTm.tm_year+1900, tmpTm.tm_mon + 1, tmpTm.tm_mday, 
        //       tmpTm.tm_hour, tmpTm.tm_min, tmpTm.tm_sec);
		snprintf(tmpBuf, sizeof(tmpBuf), "%04d-%02d-%02d %02d:%02d:%02d", tmpTm.tm_year+1900, tmpTm.tm_mon + 1, tmpTm.tm_mday, 
			tmpTm.tm_hour, tmpTm.tm_min, tmpTm.tm_sec);
        return string(tmpBuf);
}

string CToolKit::IntToStr(int nTmp)
{
        char tmpBuf[20] = {0};
        //sprintf(tmpBuf, "%d", nTmp);
        snprintf(tmpBuf, sizeof(tmpBuf), "%d", nTmp);
        return string(tmpBuf);
}

int CToolKit::IsHasChinese(char *pStr)
{
        if (pStr == NULL)
        {
                return -1;
        }
        int nIfChinese = -1;
        char *pChar = pStr;
        while (*pChar != 0)
        {
                if ((unsigned char)*pChar > 128)
                {
                        return 0;
                }
                pChar++;
        }        
        return nIfChinese;
}

int CToolKit::USLEEP(int nUsec)
{
        timeval timeout;
        int nSec = nUsec / 1000000;
        int nTmpUsec = nUsec % 1000000;
        timeout.tv_sec = nSec;
        timeout.tv_usec = nTmpUsec;
        select(0, NULL, NULL, NULL, &timeout);
        return 0;
}



void CToolKit::String_Utf16(char* dest, int nDestLen,   char* src, int nSrcLen)
{
        int i;
        memset (dest, 0, nDestLen);  /* initialise dest to 1000 times 0 */
        for (i=0; i<nSrcLen&&(i*2+1) < nDestLen; i++)
        {
                dest[i*2] = src[i];
                dest[i*2+1] = 0;
        }

        /* padd with zeros */
        dest[i*2] = 0;
        dest[i*2+1] = 0;
}


void CToolKit::StringTrim(string &sSrc)
{       
        int i = 0;
        while ((sSrc[i] == ' ' || sSrc[i] == '\t' || sSrc[i] == '\r' || sSrc[i] == '\n') && i < (int)sSrc.length())
        {
                i++;
        }
        int nBeginPos = i;
        i = sSrc.length() - 1;
        while ((sSrc[i] == ' ' || sSrc[i] == '\t' || sSrc[i] == '\r' || sSrc[i] == '\n') && i >=0)
        {
                i--;
        }
        int nEnd = i;
        sSrc = sSrc.substr(nBeginPos, nEnd - nBeginPos + 1);        
}

unsigned int CToolKit::shortELFhash(const char* sStr, unsigned int nLen)
{
        unsigned long h = 0;
		unsigned long g = 0;
        for (unsigned i = 0; i < nLen; ++i)
        {
                h = (h << 4) + *sStr++;
                g = h & 0xF0000000L;
                if (g)
                {
                        h ^= g >> 24;
                }
                h &= ~g;
        }
        return h;
}

int CToolKit::SplitString(const string &srcStr,const string &delim, vector<string>& strList)
{     
        char *sBuf = new char[srcStr.length() + 1];
        char *sTmpBuf = new char[srcStr.length() + 1];
        
        strncpy(sBuf, srcStr.c_str(), srcStr.length());
        sBuf[srcStr.length()] = 0;

        char *tmpSrc = sBuf;
        char *tt = sTmpBuf;
        
        char *sToken = strtok_r(tmpSrc, delim.c_str(), &tt);
        while (sToken)
        {
                strList.push_back(sToken);
                sToken = strtok_r(NULL, delim.c_str(), &tt);
        }
        delete [] sBuf;
        delete [] sTmpBuf;
        return 0;
}

int CToolKit::SplitString(const char *srcStr, string &delim, vector<string>& strList)
{
        char *sTmpBuf = new char[strlen(srcStr) + 1];
        char *tt = sTmpBuf;
        char *sToken = strtok_r((char*)srcStr, delim.c_str(), &tt);
        while (sToken)
        {
                strList.push_back(sToken);
                sToken = strtok_r(NULL, delim.c_str(),  &tt);
        }
        delete []sTmpBuf;
        return 0;
}


int  CToolKit::ReadDirfileList(const string &sPathdir,const string &sSuffix,  vector<string> &fileList)
{
        DIR *dirp;
        struct dirent *direntp;
        if( (dirp = opendir(sPathdir.c_str())) == NULL)  
        {                  
                return -1;  
        }
        
        char filename[1024];
        bzero(filename, 1024);
        while(( direntp = readdir( dirp)) != NULL)
        {
                if (sSuffix != "")
                {
                        if(strcmp(direntp->d_name + strlen(direntp->d_name) - sSuffix.length(), sSuffix.c_str()) == 0)
                        {
                                //sprintf(filename, "%s/%s", sPathdir.c_str(), direntp->d_name);
                                snprintf(filename, sizeof(filename), "%s/%s", sPathdir.c_str(), direntp->d_name);
                                fileList.push_back(filename);
                        }
                }
                else
                {
                        fileList.push_back(filename);
                }
        }
        closedir(dirp);        
        return 0;
}

int CToolKit::DateToSec(const string &sDate) //YYYYMMDD
{
        if (sDate.length() != 8)
        {
                return 0;
        }

        string sYear = sDate.substr(0, 4);
        string sMon = sDate.substr(4, 2);
        string sDay = sDate.substr(6, 2);

        tm tmpTm;
        memset(&tmpTm, 0, sizeof(tm));
        tmpTm.tm_year = atoi(sYear.c_str()) - 1900;
        tmpTm.tm_mon = atoi(sMon.c_str()) - 1;
        tmpTm.tm_mday = atoi(sDay.c_str());
        time_t tmpTime =  mktime(&tmpTm);
        return tmpTime;
}

int CToolKit::DetailTimeToSec(const string & sTimeStr) //YYYYMMDDHHMMSS
{
        if (sTimeStr.length() != 14)
        {
                return 0;
        }
        string sYear = sTimeStr.substr(0, 4);
        string sMon = sTimeStr.substr(4, 2);
        string sDay = sTimeStr.substr(6, 2);
        string sHour = sTimeStr.substr(8, 2);
        string sMin = sTimeStr.substr(10, 2);
        string sSec = sTimeStr.substr(12, 2);

        tm tmpTm;
        tmpTm.tm_year = atoi(sYear.c_str()) - 1900;
        tmpTm.tm_mon = atoi(sMon.c_str()) - 1;
        tmpTm.tm_mday = atoi(sDay.c_str());
        tmpTm.tm_hour = atoi(sHour.c_str());
        tmpTm.tm_min = atoi(sMin.c_str());
        tmpTm.tm_sec = atoi(sSec.c_str());
        time_t tmpTime =  mktime(&tmpTm);
        return tmpTime;
}

int CToolKit::GetUpdateNum(MYSQL *pMysql)
{
        int nMatch = 0;
        const char *pInfo = mysql_info(pMysql);
        if (pInfo != NULL)
        {                 
                char pTmp[] = "Rows matched:";
                char *pMatch = (char*)strstr(pInfo, pTmp);
                char *pChange = (char*)strstr(pInfo, "Changed");
                if (pMatch != NULL && pChange != NULL)
                {
                        unsigned int tmpLen  =  strlen(pTmp);
                        char *pBegin = pMatch + tmpLen;
                        int nLen = pChange - pMatch - tmpLen;
                        char tmpMatch[40] = {0};
                        if (nLen < 40)
                        memcpy(tmpMatch, pBegin, nLen);
                        nMatch = atoi(tmpMatch);
                }
        }
        return nMatch;
}

int CToolKit::ParseUrl(const string &sUrl, string &sIpAddr, int &nPort, string &sUrlFile, string &sHosts)
{       
        sUrlFile = "";
        nPort = -1;
        sIpAddr = "";
        
        string::size_type nPos0 = sUrl.find("://");
        if (nPos0 == string::npos)
        {
                return -1;
        }

        string::size_type nPos1 = sUrl.find("/", nPos0+3);
        if (nPos1 != string::npos)
        {
                sUrlFile = sUrl.substr(nPos1);
        }
        
        if (sUrlFile == "")
                sUrlFile = "/";

        sHosts = sUrl.substr(nPos0+3, nPos1 - nPos0 - 3);
        //将port分析出来
        nPos1 = sHosts.find(":");
        
        if (nPos1 != string::npos)
        {
                nPort = atoi(sHosts.substr(nPos1+1).c_str());
                sHosts = sHosts.substr(0, nPos1);
        }
        
        int ret = ParseDomain(sHosts, sIpAddr);
        return ret;
}


int CToolKit::ParseDomain(const string &sDomain, string &sIpAddr)
{
        //获取hosts的ip的地址
        struct hostent *hp;
        if (NULL == (hp = gethostbyname(sDomain.c_str())))
        {
                return -1;
        }             
        in_addr tmpAddr;
        memcpy(&tmpAddr.s_addr,  hp->h_addr, hp->h_length);
        sIpAddr = string(inet_ntoa(tmpAddr));        
        return 0;
}

int CToolKit::GetStringByLabel(const string &sSrc,const string &sLabel1,const string &sLabel2, string &sOut, unsigned int nBeginPos, int nIfRetPos)
{
        if (nBeginPos >= sSrc.length())
        {
            return -1;
        }
        char *pTmp = (char*)sSrc.c_str() + nBeginPos;
        char *pTmp0;
        char *pTmp1;
        if (sLabel1 == "")
        {
            pTmp0 = pTmp;
        }
        else
        {  
            pTmp0= strstr(pTmp, sLabel1.c_str());
            if (pTmp0 == NULL)
            {      
                return -1;
            }
        }
        int ret=pTmp0-sSrc.c_str()+sLabel1.length();
        if (sLabel2 != "")
        {   
            pTmp1 = strstr(pTmp0+sLabel1.length(), sLabel2.c_str());
            if (pTmp1 == NULL)
            {
                return -1;
            }
            ret=pTmp1+sLabel2.length()-sSrc.c_str();
            sOut=string(pTmp0+sLabel1.length(), pTmp1-pTmp0-sLabel1.length());
        }
        else
        {
            sOut = string(pTmp0+sLabel1.length());
            ret=sSrc.length();
        }
        if (nIfRetPos == 0)
        {
            ret = 0;
        }
        return ret;
}


int CToolKit::TimeToSec(const string &sDateStr) //Wed, 13 Dec 2006 03:10:20 GMT
{
        string sDelim = " ";
        vector<string> srcList;
        SplitString(sDateStr, sDelim, srcList);                
        if (srcList.size() != 6)
        {
                return 0;
        }
        sDelim = ":";
        vector<string> tmpList;
        SplitString(srcList[4], sDelim, tmpList);
        if (tmpList.size() != 3)
        {
                return 0;
        }
        
        tm tmpTm;
        tmpTm.tm_year = atoi(srcList[3].c_str()) - 1900;
        tmpTm.tm_mon = GetMonthNum(srcList[2]) - 1;
        tmpTm.tm_mday = atoi(srcList[1].c_str());
        
        tmpTm.tm_hour = atoi(tmpList[0].c_str());
        tmpTm.tm_min  = atoi(tmpList[1].c_str());
        tmpTm.tm_sec  = atoi(tmpList[2].c_str());
        time_t retTime =  mktime(&tmpTm);
        if (srcList[5] == "GMT")
        {
                struct timezone tz1;
		struct timeval tv1;
                gettimeofday(&tv1, &tz1);
                retTime = retTime - tz1.tz_minuteswest*60;
        }
        return retTime;
}

int CToolKit::GetMonthNum(const string &sMonth)
{               
        int retMonth = 1;
        for (int i=0; i < 12; i++)
        {
                if (strcmp(sMonth.c_str(), MONTHARRAY[i].sMonth) == 0)
                {
                        retMonth = MONTHARRAY[i].nMonth;
                }
        }
        return retMonth;
}

string CToolKit::GetMonStr(int nMonth)
{
        string sMonth;
        if (nMonth >=1 && nMonth <=12)
        {
                sMonth = string(MONTHARRAY[nMonth-1].sMonth);
        }
        return sMonth;
}

string CToolKit::GetWeekStr(int nWeek)
{
        string sWeek;
        if (nWeek >=0 && nWeek <=6)
        {
                sWeek = string(WEEKARRAY[nWeek].sWeek);
        }
        return sWeek;
}


string CToolKit::SecToHttpTime(time_t tmpTime)
{
        struct timeval tm1;
        struct timezone tz1;        
        gettimeofday(&tm1, &tz1);
        tmpTime = tmpTime + tz1.tz_minuteswest*60;
        tm tmpTm;
        localtime_r(&tmpTime, &tmpTm);
        char sTime[128] = {0};
        //sprintf(sTime, "%s, %d %s %d %02d:%02d:%02d GMT ", GetWeekStr(tmpTm.tm_wday).c_str(),  tmpTm.tm_mday, 
        //        GetMonStr(tmpTm.tm_mon + 1).c_str(), 
        //        tmpTm.tm_year+1900, tmpTm.tm_hour, tmpTm.tm_min,   tmpTm.tm_sec);
		snprintf(sTime, sizeof(sTime),"%s, %d %s %d %02d:%02d:%02d GMT ", GetWeekStr(tmpTm.tm_wday).c_str(),  tmpTm.tm_mday, 
			GetMonStr(tmpTm.tm_mon + 1).c_str(), 
			tmpTm.tm_year+1900, tmpTm.tm_hour, tmpTm.tm_min,   tmpTm.tm_sec);
        return string(sTime);
}

int  CToolKit::DecodeUrl(const string &sSrc, string &sDest, int nIfTransChinese, int nIfTransPlus)
{
        const char *sTmp = sSrc.c_str();
        int nLen = sSrc.length();
        int i=0;
        while (i < nLen)
        {
            if (nIfTransPlus == 1)
            {
                if (sTmp[i] == '+')
                {
                    sDest.append(1, ' ');
                    i++;
                    continue;
                }
            }
            
            if (sTmp[i] == '%' && i < nLen - 2)
            {      
                    string tmpStr = sSrc.substr(i+1, 2);                        
                    unsigned char tmpc = strtoul(tmpStr.c_str(), 0, 16);
                 
                    if (tmpc > 0x80  && nIfTransChinese == 0)
                    {                            
                        if ((tmpc & 0xf0) == 0xe0 )//it is a chinese char with 3 bytes
                        {
                            if (i < nLen - 8)
                            {
                                string ss = sSrc.substr(i, 9);
                                sDest.append(ss);
                                i += 9;                                    
                            }
                            else
                            {
                                break;
                            }
                        }
                        else //
                        {
                             if (i < nLen - 5)
                            {
                                string ss = sSrc.substr(i, 6);
                                sDest.append(ss);
                                i += 6;
                            }
                            else
                            {
                                break;
                            }
                        }
                    }
                    else
                    {
                        sDest.append(1, (char)tmpc);
                        i += 3;
                    }
            }
            else
            {
                    sDest.append(1, sTmp[i]);
                    i++;
            }
        }
        return 0;
}

int CToolKit::UrlEncode(const string &sSrc, string &sDest)
{
    int nLen = sSrc.length();
    int i=0;
    while (i < nLen)
    {
        unsigned char tmpc = (unsigned char)sSrc[i];
        if (URLESCAPE[tmpc] == 1)
        {
            char sBuf[10] = {0};
            //sprintf(sBuf, "%c%02X", '%', tmpc);
			snprintf(sBuf, sizeof(sBuf),"%c%02X", '%', tmpc);
            sDest.append(sBuf);
        }
        else
        {
            sDest.append(1, sSrc[i]);
        }
        i++;
    }
    return 0;
}




int CToolKit::EncodeSpace(string &sSrc)
{
        string sDest;
        int nLen = sSrc.length();
        for (int i=0; i < nLen; i++)
        {
                if (sSrc[i] == ' ')
                {
                        sDest.append("%20", 3);
                }
                else
                {
                        sDest.append(1, sSrc[i]);
                }
        }
        sSrc.assign(sDest);
        return 0;
}

int CToolKit::ChineseEncode(string &sSrc, string &sDest,  int ifToUtf8)
{
        int i=0;
        int sLen = sSrc.length();
        while (i < sLen)
        {
                unsigned char tmpc = (unsigned char)sSrc[i];
                if (tmpc > 0x80)
                {
                    if ((tmpc & 0xf0) == 0xe0 &&  i+2 < sLen) //三个字节
                    {
                        unsigned char cc1 = (unsigned char)sSrc[i+1];    
                        unsigned char cc2 = (unsigned char)sSrc[i+2];
                        char tmpBuf[10] = {0};
                        //sprintf(tmpBuf, "%c%02X%c%02X%c%02X",  '%' , (unsigned char)tmpc, '%', (unsigned char)cc1, '%', (unsigned char)cc2);
						snprintf(tmpBuf, sizeof(tmpBuf),"%c%02X%c%02X%c%02X",  '%' , (unsigned char)tmpc, '%', (unsigned char)cc1, '%', (unsigned char)cc2);
                        sDest.append(tmpBuf, 9);                        
                        i += 3;    
                    }
                    else
                    {
                        unsigned char cc1 = (unsigned char)sSrc[i+1]; 
                        char tmpBuf[10] = {0};
                        if ( ifToUtf8==0)
                        {                    
                            
                            //sprintf(tmpBuf, "%c%02X%c%02X",  '%' , (unsigned char)tmpc, '%', (unsigned char)cc1);
							snprintf(tmpBuf, sizeof(tmpBuf), "%c%02X%c%02X",  '%' , (unsigned char)tmpc, '%', (unsigned char)cc1);
                            sDest.append(tmpBuf, 6); 
                        }
                        else
                        {
                            string tmpstr, sOut;
                            tmpstr.append(1, (char)tmpc);
                            tmpstr.append(1, (char)cc1);                        
                            CToolKit::MyIconv("gbk", "utf-8", tmpstr, sOut);
                            if (sOut.length() == 3)
                            {
                                //sprintf(tmpBuf, "%c%02X%c%02X%c%02X", '%', (unsigned char)sOut[0], '%', (unsigned char)sOut[1], '%', (unsigned char)sOut[2]);
								snprintf(tmpBuf, sizeof(tmpBuf), "%c%02X%c%02X%c%02X", '%', (unsigned char)sOut[0], '%', (unsigned char)sOut[1], '%', (unsigned char)sOut[2]);
                                sDest.append(tmpBuf, 9);
                            }                        
                        }
                        i += 2;
                     }
                }
                else
                {             
                        sDest.append(1, sSrc[i]);
                        i++;
                }
        }
        return 0;
}


int CToolKit::MyIconv(const string &sFromEncode,const string &sToEncode, string &sInput, string &sOut)
{
        iconv_t convid =  iconv_open(sToEncode.c_str(), sFromEncode.c_str());
        if (convid == (iconv_t)-1)
        {                
                return -1;
        }
        
        int retValue = 0;
        char *sInputBuf = (char*)sInput.c_str();
        size_t nInputLen = sInput.length();
        size_t nOutputLen = nInputLen * 4;
        char *sOutBuf = new char[nOutputLen];
        char *ss = sOutBuf;
        size_t nssLen = nOutputLen;
        while (1)
        {
                size_t ret = iconv(convid, &sInputBuf, &nInputLen, &sOutBuf, &nOutputLen);
                if (ret != size_t(-1))
                {
                        sOut = string(ss, nssLen - nOutputLen);
                        break;
                }
                else
                {
                        if (errno == EILSEQ)
                        {
                            int nNum=3;
                            if (sFromEncode != "utf-8")
                            {
                                memcpy(sOutBuf, sInputBuf, nNum);
                                sOutBuf += nNum;
                                nOutputLen -= nNum;
                                nNum=2;
                            }
                            sInputBuf += nNum;
                            nInputLen -= nNum;                            
                            continue;
                        }
                        else
                        {
                                retValue = -1;
                                break;
                        }
                }
        }        
        iconv_close(convid);
        delete [] ss;        
        return retValue;
}


string  CToolKit::EscapeSqlStr(const char *sSrc, int nLen)
{
	char *pBuf = new char[2 * nLen + 1];
	unsigned int ret = mysql_escape_string(pBuf, sSrc, nLen);
	string retStr = string(pBuf, ret);
	delete [] pBuf;
	return retStr;
}

string  CToolKit::EscapeSqlStr(const string& sSrc)
{
        return EscapeSqlStr(sSrc.c_str(), sSrc.length());
}

string CToolKit::CutChinese(const string &sSrc)
{
    string sStr = sSrc;
    //将尾部半个汉字字符去掉
    int i=0;
    int len=sStr.length();
    while (i < len)
    {
        if (IN_RANGE(sStr[i], GBK_HIMIN, GBK_HIMAX))
        {
            if (i == (len-1))
            {
                sStr.erase(i, 1);
                break;
            }
            else
            {
                i+=2;
            }
        }
        else
        {
            i++;
        }
    }
    return sStr;
}

string CToolKit::EscapeJson(const string &sSrc)
{
    string sStr = CutChinese(sSrc);
    sStr = EscapeSqlStr(sStr);
    return sStr;    
}

string CToolKit::ToJsonStr(map<string, string> &mapStr, int ifEscape)
{
        stringstream str;
        map<string, string>::iterator tmpItr;
        str << "{";
        int i=0;
        for (tmpItr = mapStr.begin(); tmpItr != mapStr.end(); tmpItr++)
        {
                if (i > 0)
                        str << ",";
                str << "'"<< tmpItr->first << "':'";              
                if (ifEscape == 1)
                        str << EscapeSqlStr(tmpItr->second) << "'";
                else
                        str << tmpItr->second << "'";        
                i++;
        }
        str << "}";
        string tmpstr = str.str();
        return StrNewlinePro(tmpstr);
}

string CToolKit::StrNewlinePro(const string &src)
{
        string tmpstr;
        for (int i=0; i < (int)src.length(); i++)
        {
            if (src[i] != '\n' && src[i] != '\r')
                tmpstr.append(1, src[i]);
        }
        return tmpstr;
}

bool CToolKit::IsToday(const string &sTime)
{       
        if (sTime.length() < 10)
                return false;
        int nYear = atoi(sTime.substr(0, 4).c_str());
        int nMonth = atoi(sTime.substr(5, 2).c_str());
        int nDay = atoi(sTime.substr(8, 2).c_str());
        struct tm tmptm;
        time_t tmptime =   time(NULL);
        localtime_r(&tmptime, &tmptm);
        bool ret = false;
        if (tmptm.tm_year+1900 == nYear && tmptm.tm_mon+1 == nMonth && tmptm.tm_mday == nDay)
        {
                ret = true;
        }
        return ret;
}


string CToolKit::ReplaceStr(const string &sSrc)
{   
    string  tmpstr="";
    for (int i=0; i < (int)sSrc.length(); i++)
    {
        if (REPLACESET[(unsigned char)sSrc[i]] == 1)
        {
            char aa[10] = {0};
            //sprintf(aa, "%c%02X",'%', (unsigned char)sSrc[i]);
			snprintf(aa, sizeof(aa), "%c%02X",'%', (unsigned char)sSrc[i]);
            tmpstr.append((const char*)aa);
        }
        else
        {
            tmpstr.append(1, sSrc[i]);
        }
    }
    return tmpstr;
}

int CToolKit::Html_Escape_Init()
{
    m_specialCharList.clear();
    m_specialCharList['&']  =  "&amp;";
    m_specialCharList['"']  =  "&quot;";
    m_specialCharList['\'']  = "&#039;";
    m_specialCharList['<']  = "&lt;";
    m_specialCharList['>']  = "&gt;";
    m_specialCharList['\'']  = "&apos;";
    return 0;
}

map<unsigned char, string> CToolKit::m_specialCharList;
int  CToolKit::m_nHtmlSpecial = CToolKit::Html_Escape_Init();

string CToolKit::Html_Escape(const string &str)
{
    string tmpstr;
    for (int i=0;  i < (int)str.length(); i++)
    {
        map<unsigned char, string>::iterator tmpItr =  m_specialCharList.find((unsigned char)str[i]);
        if (tmpItr != m_specialCharList.end())
        {
            tmpstr.append(tmpItr->second);
        }
        else
        {
            tmpstr.append(1, str[i]);
        }
    }
    return tmpstr;
}

string CToolKit::LeiyanUnEscape(string &str)
{
    string tmpstr;
    for (int i=0;  i < (int)str.length(); i++)
    {
        if (str[i] == '%' && i+2 < (int)str.length())
        {
            char tmpc = strtoul(str.substr(i+1, 2).c_str(), 0, 16);
            tmpstr.append(1, tmpc);
            i += 2;
        }
        else
        {
            tmpstr.append(1, str[i]);
        }
    }
    return tmpstr;
}

string CToolKit::RegExpEscape(string &str)
{
    string tmpstr;
    int nLen = str.length();
    for (int i=0; i < nLen; i++)
    {
        unsigned char tmpc = (unsigned char)str[i];
        if ((tmpc & 0xf0) == 0xc0 && i+1 < nLen) //两个字节 汉字
        {
            tmpstr.append(1, str[i]);
            tmpstr.append(1, str[i+1]);
            i += 2;            
        }
        else if ((tmpc & 0xf0) == 0xe0 &&  i+2 < nLen) //三个字节
        {
            tmpstr.append(1, str[i]);
            tmpstr.append(1, str[i+1]);
            tmpstr.append(1, str[i+2]);
            i += 3;            
        }
        else
        {            
            if (REGEXPESCAPE[tmpc] == 1)
            {
                tmpstr.append(1, '\\');              
            }
            tmpstr.append(1, str[i]);
            i++;
        }                             
    }
    return tmpstr;
}


string CToolKit::GetFileCid(const string &sFileName)
{   
    string sCid;
    const int BLOCKLEN=20*1024;
    try
    {
        CMyFile myFile(sFileName);
        myFile.FOpen("r");
        struct stat *pStat =  myFile.GetStatBuf();
        int nSize =  pStat->st_size;
        if (nSize < 3*BLOCKLEN)
        {
            WriteRunInfo::WriteLog("filesize is too short, nsize is %d", nSize);
            return "";
        }
        
        string sData;
        //读取文件前面20k
        char pBuf[BLOCKLEN] = {0};
        myFile.FRead(pBuf, BLOCKLEN);
        sData.append(pBuf, BLOCKLEN);
        //中间20k
        int nCenterPos = nSize/3;
        myFile.FSeek(nCenterPos);        
        myFile.FRead(pBuf, BLOCKLEN); 
         sData.append(pBuf, BLOCKLEN);
        //后面20k
        int nEnd = nSize - BLOCKLEN;
        myFile.FSeek(nEnd);        
        myFile.FRead(pBuf, BLOCKLEN);        
        sData.append(pBuf, BLOCKLEN);

        unsigned char cid[20];
        ctx_sha1 hash;
        hash.update((unsigned char*)sData.c_str(), sData.length());
        hash.finish(cid);

        char sTmpCid[50] = {0};
        char *pEnd = sTmpCid;
        for (int i=0; i < 20; i++)
        {
                //pEnd += sprintf(pEnd, "%02X", cid[i]);
				pEnd += snprintf(pEnd, sizeof(sTmpCid)-(pEnd - sTmpCid), "%02X", cid[i]);
        }
        sCid = string(sTmpCid);
    }
    catch (CMyException &e)
    {
        WriteRunInfo::WriteLog("error is %s", e.GetErrMsg().c_str());
        return "";
    }
    return sCid;
}

string CToolKit::HexToBi(const char *pBuf, int nLen)
{
    string sOut;
    if (nLen < 0 || (nLen%2) !=0)
    {
        return "";
    }
    int nTmp = nLen/2;
    for (int ii=0; ii < nTmp; ii++)
    {
        char pTmp[3] = {0};
        strncpy(pTmp, pBuf + 2*ii, 2);
        char pChar = (char)strtoul(pTmp, 0, 16);
        sOut.append(1, pChar);
    }
    return sOut;
}

string CToolKit::BiToHex(const char *pBuf, int nLen)
{
    string sOut;
    if (nLen < 0)
    {
        return "";
    }
    for (int ii=0; ii < nLen; ii++)
    {
        char pTmp[3] = {0};
        //sprintf(pTmp, "%02X", (unsigned char)pBuf[ii]);
        snprintf(pTmp, sizeof(pTmp), "%02X", (unsigned char)pBuf[ii]);
        sOut.append(pTmp, 2);
    }
    return sOut;
}


off_t CToolKit::GetFiveFileCid(const string &sFileName, string &sCid)
{
    try
    {
        int BLOCKSIZE=20*1024;
        CMyFile myFile(sFileName);
        myFile.FOpen("r");
        off_t filesize = myFile.GetStatBuf()->st_size;
        char tmpBuf[5*BLOCKSIZE];
        int Len = 5*BLOCKSIZE;
        if (filesize > BLOCKSIZE)
        {
            for (int ii=0; ii < 5; ii++)
            {
                myFile.FSeek(filesize*ii/5);
                myFile.FRead(tmpBuf+BLOCKSIZE*ii, BLOCKSIZE);
            }
        }
        else
        {
            myFile.FRead(tmpBuf, filesize);
            Len=filesize;
        }
        unsigned char cid[20];
        ctx_sha1 hash;
        hash.update((unsigned char*)tmpBuf, Len);
        hash.finish(cid);
        sCid=string((char*)cid, 20);
        return filesize;
    }
    catch(CMyException &e)
    {
        WriteRunInfo::WriteLog("error, %s", e.GetErrMsg().c_str());
        return 0;
    }
}

void CToolKit::WriteBufToTrace(const char *pBuf, int nLen)
{
	
	int nCh = 0;
	char iConVert[10];	
	int nCnt = nLen%16 == 0 ? nLen/16 : nLen/16+1;	
	for(int i=0; i<nCnt; i++)
	{
		string sTmp;
		for(int j=0; j<16&&nCh<nLen; j++)
		{
			sprintf(iConVert, "%X%X ", (pBuf[nCh]>>4)&0xF, pBuf[nCh]&0xF);
			sTmp += iConVert, nCh++;
		}
		WriteRunInfo::WriteTrace("%s", sTmp.c_str());
		sTmp.clear();
	}
}

}

