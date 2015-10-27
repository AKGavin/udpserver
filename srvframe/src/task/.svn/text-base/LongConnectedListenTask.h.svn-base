/****************************************************************************
**
**  Copyright  (c)  2008-2009,  Baofeng, Inc.  All Rights Reserved.
**
**  Subsystem:    ConnectServer
**  File:         	     LongConnectedListenTask.h
**  Created By:   fwx
**  Created On:   2009/01/08
**
**  Purpose:
**     LongConnectedListenTask
**
**  History:
**  Programmer      Date         Version        Number      Description
**  --------------- --------   ---------- ------------------------------
**    fwx       2009/01/08         1.0            01          creation
****************************************************************************/

#ifndef _LONGCONNECTEDLISTENTASK_H_H_
#define _LONGCONNECTEDLISTENTASK_H_H_

#include "framecommon/framecommon.h"
#include "sockettask.h"
using namespace MYFRAMECOMMON;

typedef struct tagLISTENBLOCK
{
    int nSock;
    CThreadMutex mutex;
    tagLISTENBLOCK()
    {
        nSock = -1;
    }
}LISTENBLOCK, *PLISTENBLOCK;

class CLongConnectedListenTask
	: public CSocketTask
{
public:
	CLongConnectedListenTask( CTcpSocket *pSocket, CNetProcessThread*pWorkThread );
	virtual ~CLongConnectedListenTask();

	virtual int Init( void );
	virtual int Process( CMsg *pMsg );
	virtual int HandleProcess( int event );

protected:
       virtual int PutMsgToSendList(const char *pBuf, int nLen);	
        virtual int DestroySelf();
        virtual int AddToEpoll();

};
#endif //_LONGCONNECTEDLISTENTASK_H_H_
