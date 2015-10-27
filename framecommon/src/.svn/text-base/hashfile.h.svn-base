/*
 * FileName:       
 * Author:         wangqifeng  Version: 1.0  Date: 2007-5-30
 * Description:    
 * Version:        
 * Function List:  
 *                 1.
 * History:        
 *     <author>   <time>    <version >   <desc>
 */
#ifndef __HASH_FILE_H
#define  __HASH_FILE_H
#include <string>
#include <list>
#include <vector>
#include "myfile.h"
//#include "threadmutex.h"
#include "threadrwlock.h"
#include "writeRunInfo.h"
using namespace std;

namespace MYFRAMECOMMON{

const int HASHFILEINDEXLEN=999983;

struct THashFileIndex
{
        int nOffset;
        int nBucketLen;
};
const int HASHINDEXSIZE=sizeof(THashFileIndex);

      

/**
* dec:
* ClassName:CHashFile
* author: 
* version:
*CreateTime: 2007.5.30
*/
template<class T>
class CHashFile
{
public:
        CHashFile(string sFileName, unsigned int nHashIndexLen=HASHFILEINDEXLEN)
        {
                if (nHashIndexLen < 100000 || nHashIndexLen > 10000000)
                {                        
                        m_nHashIndexLen = HASHFILEINDEXLEN;
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
        
        ~CHashFile()
        {
                for (int i=0; i < m_nHashIndexLen; i++)
                {
                        list<void*>::iterator listItr = m_pidList[i].begin();
                        for (; listItr != m_pidList[i].end(); ++listItr)
                        {
                                delete (T*)(*listItr);
                        }
                 }
        }
        int AddItem(T &item)
        {                            
                int nIndex =  item.GetIntIndex()%m_nHashIndexLen; 
                T *tmpObj =  new T(item);
                m_pidList[nIndex].push_back((void*)tmpObj);
                return 0;
        }

        T *GetItem(T &item)
        {
                T *tmpRet = NULL;
                int nIndex =  item.GetIntIndex()%m_nHashIndexLen; 
                list<void*>::iterator tmpItr = m_pidList[nIndex].begin();
                for (; tmpItr != m_pidList[nIndex].end(); tmpItr++)
                {
                        T* aaT = (T*)(*tmpItr);
                        if (item == *aaT)
                        {
                                tmpRet = aaT;
                        }
                }
                return tmpRet;
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
                                THashFileIndex tmpIndex;
                                tmpIndex.nOffset = tmpOffset;
                                tmpIndex.nBucketLen = m_pidList[i].size();
                                m_indexFile.FWrite(&tmpIndex, HASHINDEXSIZE);
                                list<void*>::iterator tmpItr = m_pidList[i].begin();
                                for (; tmpItr != m_pidList[i].end(); ++tmpItr)
                                {
                                        nNum ++;
                                        T *tmpObj = (T*)*tmpItr;
                                        string sData = tmpObj->GetData();
                                        tmpOffset += sData.length() + 4;
                                        int nLen = sData.length();
                                        m_dataFile.FWrite(&nLen, 4);
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

        int SaveBak(string sFileName)
        {
                try
                {
                        CMyFile myfile(sFileName);
                        myfile.FOpen("w");
                        for (int i=0; i < m_nHashIndexLen; i++)
                        {
                                list<void*>::iterator tmpItr = m_pidList[i].begin();
                                for (; tmpItr != m_pidList[i].end(); tmpItr++)
                                {
                                        T *tmpTT = (T*)(*tmpItr);
                                        string tmpstr = tmpTT->GetBackData();
                                        myfile.FWrite((void*)tmpstr.c_str(), tmpstr.length());
                                }
                        }
                }
                catch (CMyException &e)
                {
                        WriteRunInfo::WriteLog("Save bak fail, file is %s, error is %s", sFileName.c_str(),  e.GetErrMsg().c_str());
                }
                return 0;
        }



private:
        vector<list<void*> > m_pidList;
        unsigned int m_nHashIndexLen;
        string m_sFileName;        
        CMyFile m_indexFile;  //索引数据
        CMyFile m_dataFile;   //真正的数据
        CThreadMutex m_threadMutex;
};


template<class T>
class CHashFileQuery
{
public:
        CHashFileQuery(string sFileName, unsigned int nHashIndexLen=HASHFILEINDEXLEN)
        {
                if (nHashIndexLen < 100000 || nHashIndexLen > 10000000)
                {                        
                        m_nHashIndexLen = HASHFILEINDEXLEN;
                }
                else
                {
                        m_nHashIndexLen = nHashIndexLen;
                }
                
                m_sFileName = sFileName;

        }
        ~CHashFileQuery()               
        {
        }
        
        int GetItem(T &item)
        {
                m_threadMutex.GetReadLock();
                int ret = -1;
                try
                {                       
                        OpenFile();                        
                        int nIndex = item.GetIntIndex()%m_nHashIndexLen;
                        THashFileIndex tmpHashIndex;
                        m_indexFile.FSeek(nIndex*HASHINDEXSIZE);
                        m_indexFile.FRead(&tmpHashIndex, HASHINDEXSIZE);                        
                        m_dataFile.FSeek(tmpHashIndex.nOffset);                        
                        int nOffset1 = 0;
                        for (int i=0; i < tmpHashIndex.nBucketLen; i++)
                        {      
                                int nDataLen = 0;
                                m_dataFile.FRead(&nDataLen, 4);
                                nOffset1 += 4;
                                char *tmpBuf = new char[nDataLen];                                
                                m_dataFile.FRead(tmpBuf, nDataLen);                                
                                nOffset1 += nDataLen;
                                CBufMgr bufMgr(tmpBuf, nDataLen);
                                T  tmpIndex(bufMgr);
                                
                                if (item == tmpIndex)
                                {
                                        ret = 0;
                                        item.Init(tmpIndex);
                                        break;
                                }                        
                         }                                               
                }
                catch (CMyException &e)
                {
                        WriteRunInfo::WriteLog("File op error, %s", e.GetErrMsg().c_str());
                        ret = -1;
                }
                m_threadMutex.ReleaseLock();
                return ret;
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

        int SwitchFile(string sFilename)
        {
                int ret = 0;
                m_threadMutex.GetWriteLock();
                try
                {
                        m_sFileName = sFilename;
                        string sIndexFile = sFilename + ".index";
                        m_indexFile.SetFileName(sIndexFile);
                        m_indexFile.FOpen("r");
                        string sDataFile = sFilename + ".data";
                        m_dataFile.SetFileName(sDataFile);
                        m_dataFile.FOpen("r");
                        WriteRunInfo::WriteLog("Switch file ok, %s", sFilename.c_str());
                }
                catch (CMyException &e)
                {
                        WriteRunInfo::WriteLog("Switch file fail, %s", e.GetErrMsg().c_str());                        
                        ret = -1;
                }
                m_threadMutex.ReleaseLock();
                return ret;
        }

private:
        unsigned int m_nHashIndexLen;
        string m_sFileName;        
        CMyFile m_indexFile;  //索引数据
        CMyFile m_dataFile;   //真正的数据
        CThreadRwLock m_threadMutex;
};
}
#endif

