/*
* =======================================================================
* Copyright (c) 2009-
* All rights reserved 
* -----------------------------------------------------------------------
* Version		: 1.0
* FileName		: ShortHttpProcessThread.h
* Description	: 
* -----------------------------------------------------------------------
* Author        : wangtiefeng (wtf), wangtf418@163.com
* Company		: 
* Date			: 2009-07-15
* -----------------------------------------------------------------------
* History		: 
*		<author>		  <time>		<version>		<description>
*      ---------------------------------------------------------------
*		wangtiefeng		2009-07-15			1.0			
* =======================================================================
*/
#ifndef _SHORTHTTPPROCESSTHREAD_H_AB15713F_FF40_4351_9933_55BA8438EE8F_
#define _SHORTHTTPPROCESSTHREAD_H_AB15713F_FF40_4351_9933_55BA8438EE8F_

#include "netprocessthread.h"
#include "synqueue.h"

class CShortHttpProcessThread 
	: public CNetProcessThread
{
public:
	CShortHttpProcessThread(int iEntityType, int iEntityId);
	virtual ~CShortHttpProcessThread();

protected:
	virtual int Process(CMsg *pMsg);
};

#endif



