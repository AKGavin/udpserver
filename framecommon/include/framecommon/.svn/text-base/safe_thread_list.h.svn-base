/*
 * =====================================================================================
 *
 *       Filename:  safe_thread_list.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  11/02/2009 10:33:17 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  dijie (sophy), dijie009@baofeng.com
 *        Company:  Baofeng, Inc.
 *
 * =====================================================================================
 */

#ifndef __SAFE_THREAD_LIST_H
#define __SAFE_THREAD_LIST_H

#include <list>
#include "threadmutex.h"
using namespace std;
using namespace MYFRAMECOMMON;

template <class T>
class CSafeThreadList
	:protected list<T>
{
public:
	typedef _List_iterator<T> iterator;
	CSafeThreadList()
	{
		m_nNum = 0;
	}
	void push_back(const T& __x)
	{
		m_Mutex.Lock();
		++m_nNum;
		list<T>::push_back(__x);
		m_Mutex.UnLock();
	}
	void clear()
	{
		m_Mutex.Lock();
		list<T>::clear();
		m_Mutex.UnLock();
	}
	iterator erase(iterator itr)
	{
		m_Mutex.Lock();
		--m_nNum;
		iterator itrTmp = list<T>::erase(itr);
		m_Mutex.UnLock();
		return itrTmp;
	}
	iterator begin()
	{
		return list<T>::begin();
	}
	iterator end()
	{
		return list<T>::end();
	}
	int GetNum()
	{
		return m_nNum;
	}
protected:
	int m_nNum;
	CThreadMutex m_Mutex;
};



#endif

