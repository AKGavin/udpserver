/*
 * FileName:       
 * Author:         wangqifeng  Version: 1.0  Date: 2004-12-31
 * Description:    
 * Version:        
 * Function List:  
 *                 1.
 * History:        
 *     <author>   <time>    <version >   <desc>
 */
#include "writeRunInfo.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/time.h>
#include "toolkit.h"
#include <vector>
#include <errno.h>
#include <cstring>
namespace MYFRAMECOMMON{

string WriteRunInfo::m_sFileName = "test";
CThreadMutex WriteRunInfo::m_logMutex;
char WriteRunInfo::m_sStatFile[256];
CLogTypeArray  WriteRunInfo::LOGTYPEARRAY;

CLogTypeArray::CLogTypeArray()
{        
        TLogType tmpLogType = {LOGTYPE, 0, ".log", 2};
        memcpy(&m_logTypeArray[LOGTYPE], &tmpLogType, sizeof(TLogType));
        
        TLogType tmpLogType1 = {RUNLOGTYPE, 0, ".runlog", 0};        
        memcpy(&m_logTypeArray[RUNLOGTYPE], &tmpLogType1, sizeof(TLogType));
        
        TLogType tmpLogType2 =  {TRACETYPE, 0, ".trace", 0};
        memcpy(&m_logTypeArray[TRACETYPE], &tmpLogType2, sizeof(TLogType));
        
        TLogType tmpLogType3 =  {INFOTYPE, 0, ".info", 2};
        memcpy(&m_logTypeArray[INFOTYPE], &tmpLogType3, sizeof(TLogType));
        
        TLogType tmpLogType4 = {ERRORTYPE, 0, ".error", 2};
        memcpy(&m_logTypeArray[ERRORTYPE], &tmpLogType4, sizeof(TLogType));
        
        TLogType tmpLogType5 = {INFORUNTYPE, 0, ".runinfo", 0};
        memcpy(&m_logTypeArray[INFORUNTYPE], &tmpLogType5, sizeof(TLogType));

		TLogType tmpLogType6 = {DATAINFO, 0, ".dat", 0};
		memcpy(&m_logTypeArray[DATAINFO], &tmpLogType6, sizeof(TLogType));

	
        m_nLogTypeNum = 7;
}

TLogType& CLogTypeArray::operator[](int nTypeIndex)
{
        if (nTypeIndex >=  MAXLOGTYPE)
        {
                nTypeIndex = 0;
        }
        return m_logTypeArray[nTypeIndex];
}

CLogTypeArray::~CLogTypeArray()
{
}

void CLogTypeArray::InsertLogType(TLogType &logType)
{
        if (logType.nLogtype <= 7 || logType.nLogtype >= MAXLOGTYPE)
        {
                return;
        }
        memcpy(&m_logTypeArray[logType.nLogtype], &logType, sizeof(TLogType));
}



/***************************************writerunlog*******************************************************/
void WriteRunInfo::Init(string fileName)
{
        if (fileName.find("/") == string::npos)
        {
            m_sFileName = "logs/" +  fileName;               
        }
        else
        {
            m_sFileName = fileName;
        }
}


void  WriteRunInfo::WriteRunLog(const char * format, ...)
{
        if (LOGTYPEARRAY[RUNLOGTYPE].nLogOff == 0)
        {
                return;
        }
        
        va_list ap;
        va_start(ap, format);
        LogWrite(format, ap, RUNLOGTYPE);
        va_end(ap);
        return;
}


void  WriteRunInfo::WriteLog(const char *format, ...)
{
        va_list ap;
        va_start(ap, format);
        LogWrite(format, ap, LOGTYPE);
        va_end(ap);
        return;
}

void  WriteRunInfo::WriteStat(const char *format, ...)
{
        m_logMutex.Lock();
        FILE *fp;

        time_t nowTime = time(NULL);
        tm tmpTm;
        localtime_r((const time_t*)&nowTime, &tmpTm);        
        //sprintf(m_sStatFile, "stat/%s_%04d-%02d-%02d", m_sFileName.c_str(),  1900+tmpTm.tm_year, tmpTm.tm_mon+1, tmpTm.tm_mday);
        snprintf(m_sStatFile, sizeof(m_sStatFile), "stat/%s_%04d-%02d-%02d", m_sFileName.c_str(),  1900+tmpTm.tm_year, tmpTm.tm_mon+1, tmpTm.tm_mday);
        
                
        fp = fopen(m_sStatFile, "a+");
        if (fp== NULL)
        {                
                m_logMutex.UnLock();
                WriteLog("m_sStatFile is %s, error is %s",m_sStatFile,  strerror(errno));
                return;
        }               

        fprintf(fp, "[---%02d%02d%02d---]", tmpTm.tm_hour, tmpTm.tm_min, tmpTm.tm_sec);
        va_list ap;
        va_start(ap, format);
        vfprintf(fp, format, ap);
        va_end(ap);
        fprintf(fp, "\n");
        fclose(fp);               
        m_logMutex.UnLock();
        return;
}



void  WriteRunInfo::WriteInfo(const char *format, ...)
{
        va_list ap;
        va_start(ap, format);
        LogWrite(format, ap, INFOTYPE);
        va_end(ap);
        return;
}

void  WriteRunInfo::WriteInfoRun(const char *format, ...)
{     
        va_list ap;
        va_start(ap, format);
        LogWrite(format, ap, INFORUNTYPE);
        va_end(ap);           
        return;
}


void  WriteRunInfo::WriteTrace(const char *format, ...)
{
        if (LOGTYPEARRAY[TRACETYPE].nLogOff == 0)
        {
                return;
        }
        
        va_list ap;
        va_start(ap, format);
        LogWrite(format, ap, TRACETYPE);
        va_end(ap);
        return;
}


void  WriteRunInfo::WriteError(const char *format, ...)
{
        va_list ap;
        va_start(ap, format);
        LogWrite(format, ap, ERRORTYPE);
        va_end(ap);
}


void  WriteRunInfo::SetLogFlag(int iModLogType, int iFlag)
{
        if (iModLogType < 0 || iModLogType >= MAXLOGTYPE)
        {
                return;
        }

        if (LOGTYPEARRAY[iModLogType].nLogOff == 2) // 如果是2, 不允许改变的
        {
                return;
        }
        
        LOGTYPEARRAY[iModLogType].nLogOff = iFlag;
        return;
}


void WriteRunInfo::WriteNowDetailTime()
{
        if (LOGTYPEARRAY[TRACETYPE].nLogOff == 0)
        {
                return;
        }        

        
        timeval tm;
        gettimeofday(&tm, NULL);
        WriteTrace("Now is %d second and %d milsecond", tm.tv_sec, tm.tv_usec);
}

void WriteRunInfo::WriteAllLog(int nLogType, const char *format, ...)
{
        if (nLogType < 0 || nLogType >= MAXLOGTYPE)
        {
                return;
        }
        va_list ap;
        va_start(ap, format);
        LogWrite(format, ap, nLogType);
        va_end(ap);
        return;
}

void WriteRunInfo::LogWrite(const char *format,  va_list &ap, int nLogType)
{               
        if (nLogType < 0 || nLogType >= MAXLOGTYPE)
        {
                return;
        }

        if (LOGTYPEARRAY[nLogType].nLogOff == 0)
        {
                return;
        }
        
        m_logMutex.Lock();        
        string sFileName = m_sFileName + string(LOGTYPEARRAY[nLogType].sFileNameSuffix);
        FILE *fp;
        struct stat statBuf;
        int ret = stat(sFileName.c_str(), &statBuf);        
        if (ret == 0)
        {
                if (statBuf.st_size > MAXLOGSIZE)
                {
                        LOGTYPEARRAY[nLogType].nFileNum ++;
                        LOGTYPEARRAY[nLogType].nFileNum = LOGTYPEARRAY[nLogType].nFileNum%10;
                        char newPath[1024];
                        //sprintf(newPath, "%s_%02d", sFileName.c_str(), LOGTYPEARRAY[nLogType].nFileNum);
                        snprintf(newPath, sizeof(newPath), "%s_%02d", sFileName.c_str(), LOGTYPEARRAY[nLogType].nFileNum);
                        rename(sFileName.c_str(), newPath);
                }
        }
                
        fp = fopen(sFileName.c_str(), "a+");
        if (fp== NULL)
        {
                m_logMutex.UnLock();
                return;
        }       
        string dateTime = CToolKit::Secd2Time(time(NULL));
        fprintf(fp, "%s: ", dateTime.c_str());
        vfprintf(fp, format, ap);
        fprintf(fp, "\n");
        fclose(fp);   
        m_logMutex.UnLock();
}

void  WriteRunInfo::WriteData(const char *pData,int iSize, const char *format, ...)
{
     	 va_list ap;
        va_start(ap, format);
        LogWriteData(pData, iSize, format, ap, DATAINFO);
        va_end(ap);
        return;
}

void WriteRunInfo::LogWriteData(const char *pData,int iSize, const char *format,  va_list &ap,  int nLogType)
{
	if (nLogType < 0 || nLogType >= MAXLOGTYPE)
        {
                return;
        }

        if (LOGTYPEARRAY[nLogType].nLogOff == 0)
        {
                return;
        }
        
        m_logMutex.Lock();        
        string sFileName = m_sFileName + string(LOGTYPEARRAY[nLogType].sFileNameSuffix);
        FILE *fp;
        struct stat statBuf;
        int ret = stat(sFileName.c_str(), &statBuf);        
        if (ret == 0)
        {
                if (statBuf.st_size > MAXLOGSIZE)
                {
                        LOGTYPEARRAY[nLogType].nFileNum ++;
                        LOGTYPEARRAY[nLogType].nFileNum = LOGTYPEARRAY[nLogType].nFileNum%10;
                        char newPath[1024];
                        //sprintf(newPath, "%s_%02d", sFileName.c_str(), LOGTYPEARRAY[nLogType].nFileNum);
                        snprintf(newPath, sizeof(newPath), "%s_%02d", sFileName.c_str(), LOGTYPEARRAY[nLogType].nFileNum);
                        rename(sFileName.c_str(), newPath);
                }
        }
                
        fp = fopen(sFileName.c_str(), "a+");
        if (fp== NULL)
        {
                m_logMutex.UnLock();
                return;
        }       
        string dateTime = CToolKit::Secd2Time(time(NULL));
        fprintf(fp, "%s: ", dateTime.c_str());
        vfprintf(fp, format, ap);
        fprintf(fp, "\n");
	// add data
	fwrite(pData,  sizeof(char), iSize, fp);
       fprintf(fp, "\n");
		
        fclose(fp);   
        m_logMutex.UnLock();
}

}

        
