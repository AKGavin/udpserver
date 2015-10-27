/*
 * FileName:       
 * Author:         wangqifeng  Version: 1.0  Date: 2006-11-25
 * Description:    
 * Version:        
 * Function List:  
 *                 1.
 * History:        
 *     <author>   <time>    <version >   <desc>
 */
#include "myfile.h"
#include <stdarg.h>
#include <errno.h>
#include <cstring>
namespace MYFRAMECOMMON{

CMyFile::CMyFile()
{
        m_fp = NULL;
        m_pStatBuf = NULL;
}

CMyFile::CMyFile(const string &sFileName)
{       
        m_fp = NULL;
        m_pStatBuf = NULL;
        m_sFileName = sFileName;
}

CMyFile::~CMyFile()
{
        FClose();
}

void CMyFile::SetFileName(string sFileName)
{
        FClose();
        m_sFileName = sFileName;
}

size_t  CMyFile::FRead(void *sReadBuf, size_t nSize)
{
        if (m_fp == NULL)
        {
                throw CMyException("The file ponit is null");
                return -1;
        }

        int ret = fread(sReadBuf, nSize, 1, m_fp);
        if (ret != 1)
        {
                char sTmp[1024] = {0};
                //sprintf(sTmp, "Read  file fail , ret is %d, error is %s", ret, strerror(errno));
				snprintf(sTmp, sizeof(sTmp), "Read  file fail , ret is %d, error is %s", ret, strerror(errno));
                throw CMyException(sTmp);
                return -1;
        }        
        return ret*nSize;        
}

size_t CMyFile::FWrite(void *sWriteBuf, size_t nSize)
{
        if (m_fp == NULL)
        {
                throw CMyException("The file ponit is null");
                return -1;
        }

        int ret = fwrite(sWriteBuf, nSize, 1, m_fp);
        if (ret != 1)
        {
                char sTmp[1024] = {0};
                //sprintf(sTmp, "Write file fail , ret is %d, error is %s", ret, strerror(errno));
                snprintf(sTmp, sizeof(sTmp), "Write file fail , ret is %d, error is %s", ret, strerror(errno));
                throw CMyException(sTmp);
                return -1;
        }        
        return ret*nSize;        
}

int CMyFile::FSeek(off_t nOffset)
{
        if (m_fp == NULL)
        {
                throw CMyException("The file ponit is null");
                return -1;
        }

        if (fseeko(m_fp, nOffset, SEEK_SET) == -1)
        {
                char sTmp[1024] = {0};
                //sprintf(sTmp, "Fseek file fail ,error is %s",  strerror(errno));
				snprintf(sTmp, sizeof(sTmp), "Fseek file fail ,error is %s",  strerror(errno));
                throw CMyException(sTmp);
                return -1;
        }
        return 0;
}

int CMyFile::FOpen(const char *mode)
{
        if (mode == NULL)
        {
                throw CMyException("mode is null");
                return -1;
        }
        FClose(); //ÏÈ°Ñ¾ä±ú¹Ø±Õµô
                
        m_fp = fopen(m_sFileName.c_str(),  mode);
        if (m_fp == NULL)
        {
                string sTmp;
                sTmp =  "open file fail (filename is " + m_sFileName +"), error is" + string(strerror(errno));
                throw CMyException(sTmp);
                return -1;
        }
        return 0;
}

int CMyFile::FPrintf(const char *format, ...)
{
        if (m_fp == NULL)
        {
                throw CMyException("The file ponit is null");
                return -1;
        }
        va_list ap;
        va_start(ap, format);
        vfprintf(m_fp, format, ap);
        va_end(ap);
        return 0;
}

void CMyFile::FClose()
{
        if (m_fp != NULL)
        {
                fclose(m_fp);
                m_fp = NULL;
        }

        if (m_pStatBuf != NULL)
        {
                delete m_pStatBuf;
                m_pStatBuf = NULL;
        }
}

struct stat *CMyFile::GetStatBuf()
{
        if (m_pStatBuf == NULL)
        {
                struct stat *pBuf = new struct stat;
                int ret = stat(m_sFileName.c_str(), pBuf);
                if (ret == 0)
                {
                        m_pStatBuf = pBuf;
                }
                else
                {
                        delete pBuf;
                        char sError[1024] = {0};
                        //sprintf(sError, "Error info is %s", strerror(errno));
                        snprintf(sError, sizeof(sError), "Error info is %s", strerror(errno));
                        throw CMyException(string (sError));          
                }
        }
        return m_pStatBuf;
}

void CMyFile::ModifyFileTime(time_t nModifyTime, time_t nAccessTime)
{       
        time_t nowTime = time(NULL);
        struct utimbuf utimetmp;
        utimetmp.modtime = (nModifyTime == -1)?nowTime:nModifyTime;
        utimetmp.actime = (nAccessTime == -1)?nowTime:nAccessTime;
        int ret = utime(m_sFileName.c_str(), &utimetmp);
        if (ret != 0)
        {                
                throw CMyException(string(strerror(errno)));
        }
}
}


