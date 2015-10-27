/****************************************************************************
**
**  Copyright  (c)  2008-2009,  Baofeng, Inc.  All Rights Reserved.
**
**  Subsystem:    ConnectServer
**  File:         	     LongConnectedProcessThread.h
**  Created By:   fwx
**  Created On:   2009/01/08
**
**  Purpose:
**     LongConnectedProcessThread
**
**  History:
**  Programmer      Date         Version        Number      Description
**  --------------- --------   ---------- ------------------------------
**    fwx       2009/01/08         1.0            01          creation
****************************************************************************/
#ifndef _LONGCONNECTEDPROCESSTHREAD_H_H_
#define _LONGCONNECTEDPROCESSTHREAD_H_H_

#include "netprocessthread.h"

class CLongConnectedProcessThread
	: public CNetProcessThread
{
public:
	CLongConnectedProcessThread( int iEntityType, int iEntityId, int iListType = 0 );
	virtual ~CLongConnectedProcessThread( void );

private:
        virtual int  Process( CMsg *pMsg );	

	int m_iListType;
};
#endif //_LONGCONNECTEDPROCESSTHREAD_H_H_
