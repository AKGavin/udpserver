/****************************************************************************
**
**  Copyright  (c)  2008-2009,  Baofeng, Inc.  All Rights Reserved.
**
**  Subsystem:    ConnectServer
**  File:         	     LongConnectedListenThread.h
**  Created By:   fwx
**  Created On:   2009/01/08
**
**  Purpose:
**     LongConnectedListenThread
**
**  History:
**  Programmer      Date         Version        Number      Description
**  --------------- --------   ---------- ------------------------------
**    fwx       2009/01/08         1.0            01          creation
****************************************************************************/
#ifndef _LONGCONNECTEDLISTENTHREAD_H_H_
#define _LONGCONNECTEDLISTENTHREAD_H_H_

#include "LongConnectedListenTask.h"
#include "netprocessthread.h"

class CLongConnectedListenThread
	: public CNetProcessThread
{
public:
	CLongConnectedListenThread( int iEntityType, int iEntityId, string sHost, int nPort, PLISTENBLOCK pListenBlock );
	virtual ~CLongConnectedListenThread();

	 void SetListenSocket(CTcpSocket *pSocket)
        {
                m_pTcpSocket  = pSocket;
        }
        int Init();
        LISTENBLOCK *GetListenBlock()
        {
            return m_pListenBlock;
        }

protected:
        string m_sHost;
        int m_nPort;
        CTcpSocket *m_pTcpSocket;
        CLongConnectedListenTask *m_pListenTask;        
        PLISTENBLOCK m_pListenBlock;		
};
#endif //_LONGCONNECTEDLISTENTHREAD_H_H_
