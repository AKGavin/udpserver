/*
 * FileName:       
 * Author:         wangqifeng  Version: 1.0  Date: 2009-1-8
 * Description:    
 * Version:        
 * Function List:  
 *                 1.
 * History:        
 *     <author>   <time>    <version >   <desc>
 */
#ifndef __MY_HASH_TABLE_H
#define __MY_HASH_TABLE_H
#include "threadmutex.h"
using namespace std;
namespace MYFRAMECOMMON{
#define HASHLOCK(index) \
	if(m_nIfLock == 1)\
	{\
		m_hashMutex[index].Lock();\
	}
	
#define HASHUNLOCK(index) \
	if (m_nIfLock==1)\
	{\
		m_hashMutex[index].UnLock();\
	}
/**
* dec:
* ClassName:CHashTable
* author: 
* version:
*CreateTime: 2009.1.8
*/
template<class T, class  T1>
class CMyHashTable
{
public:
    CMyHashTable(int nHashBucketLen, int nIfLock=0)
	{
		m_nHashBucketLen = nHashBucketLen;
		m_list = new T1[m_nHashBucketLen];
		m_nIfLock = nIfLock;
		if (m_nIfLock == 1)
		{
			m_hashMutex = new CThreadMutex[m_nHashBucketLen];
		}
		else
		{
			m_hashMutex = NULL;
		}
		m_nNodeNum=0;
	}
	
    ~CMyHashTable()
	{
		if (m_list != NULL)
		{
			delete []m_list;
		}
		if (m_hashMutex != NULL)
		{
			delete []m_hashMutex;
		}
	}
	
	/**
	 *
	 *  \param   &item
	 *  \return   	int  0:sucess, 1:exist
	 */
	int AddItem(T &item)
	{			
		int nRet = 0;
		int nTmpIndex = item.GetIndex();
		int nRealIndex = nTmpIndex%m_nHashBucketLen;
		HASHLOCK(nRealIndex);
		HashIterator listItr = m_list[nRealIndex].begin();
		for (; listItr != m_list[nRealIndex].end(); listItr++)
		{	
			if (item == *listItr)
			{
				break;
			}
		}
		if (listItr == m_list[nRealIndex].end())
		{
			m_list[nRealIndex].push_back(item);
			m_nNodeNum++;
		}
		else
		{
			nRet = 1;			
		}
		HASHUNLOCK(nRealIndex)
		return nRet;
	}

	/**
	 *
	 *  \param   &item
	 *  \return   	int  0:sucess, -1:fail
	 */
	int GetItem(T &item)
	{				
		int nRet = 0;
		int nTmpIndex = item.GetIndex();
		int nRealIndex = nTmpIndex%m_nHashBucketLen;
		HASHLOCK(nRealIndex);
		HashIterator listItr = m_list[nRealIndex].begin();
		for (; listItr != m_list[nRealIndex].end(); listItr++)
		{	
			if (item == *listItr)
			{
				item = *listItr;
				break;
			}
		}
		if (listItr == m_list[nRealIndex].end())
		{
			nRet = -1;
		}		
		HASHUNLOCK(nRealIndex)
		return nRet;
	}

	int GetNodeNum()
	{
		return m_nNodeNum;
	}

	int DelItem(T &item)
	{
		int nRet = 0;
		int nTmpIndex = item.GetIndex();
		int nRealIndex = nTmpIndex%m_nHashBucketLen;
		HASHLOCK(nRealIndex);
		HashIterator listItr = m_list[nRealIndex].begin();
		for (; listItr != m_list[nRealIndex].end(); listItr++)
		{	
			if (item == *listItr)
			{
				item = *listItr;
				m_list[nRealIndex].erase(listItr);
				m_nNodeNum--;
				break;
			}
		}
		if (listItr == m_list[nRealIndex].end())
		{
			nRet = -1;
		}		
		HASHUNLOCK(nRealIndex)
		return nRet;
	}

    int Clear()
    {
        for (int ii=0; ii < m_nHashBucketLen; ii++)
        {   
            HASHLOCK(ii);
            m_nNodeNum -= m_list[ii].size();
            m_list[ii].clear();
            HASHUNLOCK(ii)
        }
        return 0;
    }
protected:
	typedef typename T1::iterator HashIterator;
	int m_nHashBucketLen;
	T1* m_list;
	int m_nIfLock;
	CThreadMutex* m_hashMutex;
	int m_nNodeNum;
};
}
#endif

