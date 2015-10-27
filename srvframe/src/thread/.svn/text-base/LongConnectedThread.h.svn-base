/****************************************************************************
**
**  Copyright  (c)  2008-2009,  Baofeng, Inc.  All Rights Reserved.
**
**  Subsystem:    ConnectServer
**  File:         	     LongConnectedThread.h
**  Created By:   fwx
**  Created On:   2009/01/08
**
**  Purpose:
**     LongConnectedThread
**
**  History:
**  Programmer      Date         Version        Number      Description
**  --------------- --------   ---------- ------------------------------
**    fwx       2009/01/08         1.0            01          creation
****************************************************************************/
#ifndef _LONGCONNECTEDTHREAD_H_H_
#define _LONGCONNECTEDTHREAD_H_H_

#include "netprocessthread.h"
#include "EpollReactor.h"

class CLongConnectedThread
	: public CNetProcessThread
{
public:
	CLongConnectedThread( int iEntityType, int iEntityId, int iListType = 0 );
        
	virtual ~CLongConnectedThread( void );

protected:
        virtual int Init( void );
        virtual int  Process( CMsg *pMsg );
		
protected:
	CEpollReactor *m_pEpollReactor;
	int                   m_iListType;
};
#endif //_LONGCONNECTEDTHREAD_H_H_
