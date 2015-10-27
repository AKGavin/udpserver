/*
 * FileName:       
 * Author:         wangqifeng  Version: 1.0  Date: 2007-1-16
 * Description:    
 * Version:        
 * Function List:  
 *                 1.
 * History:        
 *     <author>   <time>    <version >   <desc>
 */
#ifndef __PID_LIST_H
#define __PID_LIST_H
#include <string>
#include <vector>
#include <list>
#include "commondef.h"
#include "myfile.h"
#include "threadmutex.h"
#include "writeRunInfo.h"

using namespace std;
namespace MYFRAMECOMMON{
const int HASHINDEXLEN=999983;

struct TIndex
{
        int nOffset;
        int nBucketLen;
};

struct TDataHead
{
        ULONG64 lPeerId;
        int nDataLen;
};

const int TINDEXSIZE=sizeof(TIndex);
const int TDATAHEADSIZE=sizeof(TDataHead);

typedef list<void*>::iterator TITR;

template<class T>
class CPidList
{
public:
        CPidList(string sFileName, unsigned int nHashIndexLen=HASHINDEXLEN)
        {
                if (nHashIndexLen < 100000 || nHashIndexLen > 10000000)
                {                        
                        m_nHashIndexLen = HASHINDEXLEN;
                }
                else
                {
                        m_nHashIndexLen = nHashIndexLen;
                }
                
                m_sFileName = sFileName;
                for (unsigned int i=0; i < m_nHashIndexLen; i++)
                {
                        list<void*>  tmpList;
                        m_pidList.push_back(tmpList);
                }
        }
        ~CPidList()
        {
                for (unsigned int i =0; i < m_nHashIndexLen; i++)
                {
                        list<void*>::iterator tmpItr = m_pidList[i].begin();
                        for (; tmpItr != m_pidList[i].end(); ++tmpItr)
                        {
                                delete (T*)(*tmpItr);
                        }
                }
        }
        
        int AddPidInfo(T &pidInfo)
        {
                ULONG64 lPid = pidInfo.GetPid();
                int nIndex =  lPid%m_nHashIndexLen; 
                T *tmpObj =  new T(pidInfo);
                m_pidList[nIndex].push_back((void*)tmpObj);
                return 0;
        }
        
        int SaveIndex()
        {
                try
                {
                        string sIndexFile = m_sFileName + ".index";
                        string sDataFile = m_sFileName + ".data";
                        m_indexFile.SetFileName(sIndexFile);
                        m_indexFile.FOpen("w");
                        m_dataFile.SetFileName(sDataFile);
                        m_dataFile.FOpen("w");

                        int tmpOffset = 0;
                        int nNum = 0;
                        for (unsigned int i=0; i < m_nHashIndexLen; i++)
                        {
                                TIndex tmpIndex;
                                tmpIndex.nOffset = tmpOffset;
                                tmpIndex.nBucketLen = m_pidList[i].size();
                                m_indexFile.FWrite(&tmpIndex, TINDEXSIZE);
                                TITR tmpItr = m_pidList[i].begin();
                                for (; tmpItr != m_pidList[i].end(); ++tmpItr)
                                {
                                        nNum ++;
                                        T *tmpObj = (T*)*tmpItr;
                                        string sData = tmpObj->GetData();
                                        tmpOffset += sData.length();
                                        m_dataFile.FWrite((void*)sData.c_str(), sData.length());
                                }
                        }
	          printf("nNum is  %d\n", nNum);
                }
                catch (CMyException &e)
                {
                        WriteRunInfo::WriteLog("Save index fail, file is %s, error is %s", m_sFileName.c_str(),  e.GetErrMsg().c_str());
                }
                return 0;
        }
protected:
        vector<list<void*> > m_pidList;
        unsigned int m_nHashIndexLen;
        string m_sFileName;        
        CMyFile m_indexFile;  //索引数据
        CMyFile m_dataFile;   //真正的数据
};

/**
* dec:
* ClassName:CPidList
* author: 
* version:
*CreateTime: 2007.1.16
*/
template <class T>
class CPidQueryList
{
public:
        CPidQueryList(string sFileName, unsigned int nHashIndexLen=HASHINDEXLEN)
        {
                if (nHashIndexLen < 100000 || nHashIndexLen > 10000000)
                {                        
                        m_nHashIndexLen = HASHINDEXLEN;
                }
                else
                {
                        m_nHashIndexLen = nHashIndexLen;
                }
                m_sFileName = sFileName;
        }

        ~CPidQueryList()
        {
        }

        void SetFileName(string sFileName)
        {
                CloseFile();
                m_sFileName = sFileName;
        }
        
        int GetPidInfo(ULONG64 lPid, T &pidInfo)
        {
                m_threadMutex.Lock();
                int ret = -1;
                try
                {                       
                        OpenFile();
                        
                        int nIndex = lPid%m_nHashIndexLen;
                        m_indexFile.FSeek(nIndex*TINDEXSIZE);
                        TIndex tmpIndex;
                        m_indexFile.FRead(&tmpIndex, TINDEXSIZE);
                        
                        m_dataFile.FSeek(tmpIndex.nOffset);                        
                        TDataHead dataHead;
                        int nOffset1 = 0;
                        for (int i=0; i < tmpIndex.nBucketLen; i++)
                        {      
                                m_dataFile.FRead(&dataHead, TDATAHEADSIZE);
                                nOffset1 += TDATAHEADSIZE + dataHead.nDataLen;
                                if (dataHead.lPeerId == lPid)
                                {
                                        char *tmpBuf = NULL;
                                        if (dataHead.nDataLen > 0)
                                        {
                                                tmpBuf = new char[dataHead.nDataLen];
                                                m_dataFile.FRead(tmpBuf, dataHead.nDataLen);
                                                pidInfo.Init(dataHead, tmpBuf);
                                                delete []tmpBuf;
                                        }                  
                                        else
                                        {
                                                pidInfo.Init(dataHead, tmpBuf);
                                        }
                                        ret = 0;
                                }
                                else
                                {
                                        m_dataFile.FSeek(tmpIndex.nOffset + nOffset1);
                                }
                        }                                               
                }
                catch (CMyException &e)
                {
                        WriteRunInfo::WriteLog("File op error, %s", e.GetErrMsg().c_str());
                        ret = -1;
                }
                m_threadMutex.UnLock();
                return ret;
        }
        
        int SwitchFile(string sFilename)
        {
                int ret = 0;
                m_threadMutex.Lock();
                try
                {
                        string sIndexFile = sFilename + ".index";                        
                        string sDataFile = sFilename + ".data";
                        CMyFile tmpFile(sIndexFile);
                        CMyFile tmpFile1(sDataFile);
                        
                        if (tmpFile.GetStatBuf()->st_mtime != m_indexFile.GetStatBuf()->st_mtime ||
                                tmpFile1.GetStatBuf()->st_mtime == m_dataFile.GetStatBuf()->st_mtime) //没有变化
                        {
                                m_sFileName = sFilename;                        
                                m_indexFile.SetFileName(sIndexFile);
                                m_indexFile.FOpen("r");
                                m_dataFile.SetFileName(sDataFile);
                                m_dataFile.FOpen("r");
                        }
                }
                catch (CMyException &e)
                {
                        WriteRunInfo::WriteLog("Switch file fail, %s", e.GetErrMsg().c_str());                        
                        ret = -1;
                }
                m_threadMutex.UnLock();
                return ret;
        }

        string &GetFileName()
        {
                return m_sFileName;
        }

        int OpenFile()
        {
                try
                {
                        if (m_indexFile.GetFp() == NULL)
                        {
                                string sIndexFile = m_sFileName + ".index";
                                m_indexFile.SetFileName(sIndexFile);
                                m_indexFile.FOpen("r");
                        }
                        if (m_dataFile.GetFp() == NULL)
                        {
                                string sDataFile = m_sFileName + ".data";
                                m_dataFile.SetFileName(sDataFile);
                                m_dataFile.FOpen("r");
                        }
                }
                catch (CMyException &e)
                {
                        WriteRunInfo::WriteLog("Open file fail");
                        return -1;
                }
                return 0;
        }

        void CloseFile()
        {
                m_indexFile.FClose();
                m_dataFile.FClose();
        }
protected:
        unsigned int m_nHashIndexLen;
        string m_sFileName;        
        CMyFile m_indexFile;  //索引数据
        CMyFile m_dataFile;   //真正的数据
        CThreadMutex m_threadMutex;
};
}
#endif

