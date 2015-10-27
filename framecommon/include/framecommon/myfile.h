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
#ifndef __MY_FILE_H
#define __MY_FILE_H
#include <stdio.h>
#include <errno.h>
#include "myexception.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/types.h>
#include <utime.h>
namespace MYFRAMECOMMON{

/**
* dec:
* ClassName:CMyFile
* author: 
* version:
*CreateTime: 2006.11.25
*/
class CMyFile
{
public:
        CMyFile();
        CMyFile(const string &sFileName);
        ~CMyFile();
        size_t  FRead(void *sReadBuf, size_t nSize);
        size_t FWrite(void *sWriteBuf, size_t nSize);
        int FPrintf(const char *format, ...);
        int FSeek(off_t nOffset);
        int FOpen(const char *mode);
        void FClose();

        void ModifyFileTime(time_t nModifyTime, time_t nAccessTime);
        
        void SetFileName(string sFileName);      
        
        string &GetFileName()
        {
                return m_sFileName;
        }

        FILE *GetFp()
        {
                return m_fp;
        }

        struct stat *GetStatBuf();
protected:
        void Init ();
        struct stat *m_pStatBuf;
        string m_sFileName;
        FILE *m_fp;        
};
}

#endif

