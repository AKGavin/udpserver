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
#ifndef __WRITE_RUN_INFO_H
#define  __WRITE_RUN_INFO_H
#include <stdio.h>
#include <stdarg.h>
#include <string>
#include "threadmutex.h"
#include <vector>
using namespace std;

namespace MYFRAMECOMMON{

#define  MAXLOGSIZE  5000000

#define ONLINE_LOG_TYPE 50
#define RSERVER_LOG_TYPE 51
#define QSERVER_LOG_TYPE 52
#define CONNECT_LOG_TYPE 53
#define VPEER_LOG_TYPE	54
#define NAT_LOG_TYPE 55

#define PC_LOG_TYPE 56  //dijie 09-5-20
#define NC_LOG_TYPE 57  //dijie 09-7-8
#define STRACK_LOG_TYPE 58  //dijie 09-7-16


enum WRITELOGTYPE
{
        LOGTYPE=0,
        RUNLOGTYPE=1,
        TRACETYPE=2,
        INFOTYPE=3,
        ERRORTYPE=4,
        INFORUNTYPE=5,
        MEDIAINFO=6,
        DATAINFO=7
};

struct TLogType
{
        int nLogtype;
        int nFileNum;
        char sFileNameSuffix[256];
        int nLogOff;  //0:允许开关 2:固定打开的 不允许开关的
};

const int MAXLOGTYPE=100;        
class CLogTypeArray
{
public:
        CLogTypeArray();
        ~CLogTypeArray();
        TLogType& operator[](int nTypeIndex);
        void InsertLogType(TLogType &logType);
        int Size()
        {
                return m_nLogTypeNum;
        }
private:
        TLogType m_logTypeArray[MAXLOGTYPE];
        int m_nLogTypeNum;
};

#define WRITE_TRACE(tmplt, ...) \
    if (WriteRunInfo::LOGTYPEARRAY[TRACETYPE].nLogOff)\
        WriteRunInfo::WriteTrace(tmplt, ##__VA_ARGS__)
#define WRITE_DAT( pData , iSize , format , ... ) \
	if( WriteRunInfo::LOGTYPEARRAY[TRACETYPE].nLogOff)\
		WriteRunInfo::WriteData( pData , iSize , format , ##__VA_ARGS__)
#define WRITE_ALL_LOG( Logtype , format , ... ) \
	if(WriteRunInfo::LOGTYPEARRAY[Logtype].nLogOff)\
		WriteRunInfo::WriteAllLog( Logtype ,format , ##__VA_ARGS__)

class WriteRunInfo
{
public:
        static void Init(string fileName);    
        static void  WriteRunLog(const char * format, ...);
        static void  WriteLog(const char *format, ...);
        static void  WriteInfo(const char *format, ...);
        static void  WriteInfoRun(const char *format, ...);
        static void  WriteTrace(const char *format, ...);
        static void  WriteStat(const char *format, ...);
        static void  WriteError(const char *format, ...);

        static void  WriteAllLog(int nLogType, const char *format, ...);
                
        static void  SetLogFlag(int iModLogType, int iFlag);
        static void  WriteNowDetailTime();

        static void  InsertLogType(TLogType &logType);

	    static void  WriteData(const char *pData,int iSize, const char *format, ...);
	
	    static CLogTypeArray LOGTYPEARRAY;	
private:
        /**
         *
         *  \param   *format
         *  \param   &ap
         *  \param   *sFileName
         *  \param   &nFileNameIndex
         *  \return   static void
         */
        static void LogWrite(const char *format,  va_list &ap,  int nLogType);  

        static void LogWriteData(const char *pData,int iSize, const char *format,  va_list &ap,  int nLogType);  

        static  string m_sFileName;

        static char m_sStatFile[256];
        static CThreadMutex m_logMutex;        
};
}

#endif

