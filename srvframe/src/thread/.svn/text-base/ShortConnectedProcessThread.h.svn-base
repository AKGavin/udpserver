/****************************************************************************
**
**  Copyright  (c)  2008-2009,  Baofeng, Inc.  All Rights Reserved.
**
**  Subsystem:    ConnectServer
**  File:         	     ShortConnectedProcessThread.h
**  Created By:   fwx
**  Created On:   2009/01/08
**
**  Purpose:
**     ShortConnectedProcessThread
**
**  History:
**  Programmer      Date         Version        Number      Description
**  --------------- --------   ---------- ------------------------------
**    fwx       2009/01/08         1.0            01          creation
****************************************************************************/

#ifndef _SHORTCONNECTEDPROCESSTHREAD_H_H_
#define _SHORTCONNECTEDPROCESSTHREAD_H_H_

#include "netprocessthread.h"

class CShortConnectedProcessThread
	: public CNetProcessThread
{
public:
	CShortConnectedProcessThread( int iEntityType, int iEntityId );
	virtual ~CShortConnectedProcessThread();

protected:
	virtual int  Process(CMsg *pMsg);
};
#endif //_SHORTCONNECTEDPROCESSTHREAD_H_H_

