/****************************************************************************
**
**  Copyright  (c)  2008-2009,  Baofeng, Inc.  All Rights Reserved.
**
**  Subsystem:    ConnectServer
**  File:         	     CommonTask.cpp
**  Created By:   fwx
**  Created On:   2009/01/08
**
**  Purpose:
**     CommonTask
**
**  History:
**  Programmer      Date         Version        Number      Description
**  --------------- --------   ---------- ------------------------------
**    fwx       2009/01/08         1.0            01          creation
****************************************************************************/

#ifndef _COMMONTASK_H_H_
#define _COMMONTASK_H_H_

#include <vector>
#include <list>

#include "framecommon/framecommon.h"
#include "recvandsendtask.h"
#include "netprocessthread.h"

using namespace MYFRAMECOMMON;

#define CONNECT_ATTRIBUTE_CLOSE 0
#define CONNECT_ATTRIBUTE_ESTABLISH 1

struct TTaskConnectAttrib
{
	unsigned char ucMachineType;
	unsigned short usMachineIndex;
	int iConnectState;				//CONNECT_ATTRIBUTE_CLOSE : 长连接断开或未建立 | CONNECT_ATTRIBUTE_ESTABLISH：长连接建立
};

class CCommonTask
	: public CRecvAndSendTask
{
public:
	CCommonTask( CTcpSocket *pSocket, CNetProcessThread *pWorkThread,  unsigned char ucMachineType, unsigned short usMachineIndex, bool bActive, int iListType );
	
	virtual ~CCommonTask( void );

	static int CheckConnectState( unsigned char ucMachineType , unsigned short usMachineIndex );

protected:
	virtual int Init();
    virtual int DestroySelf( void );
    virtual int RecvProcess( void );
    virtual int SendProcess( void ); 
	virtual int PutMsgToSendList( const char *pBuf, int iLen );
	virtual int ProcessPacket( PSERVERCOMMANDPACKET pServerCommandPacket );
	void AddTaskConnectAttrib( unsigned char ucMachineType , unsigned short usMachineIndex , int iConnectState );

protected:
	unsigned char m_cTaskType;
private:
	int AddToEpoll( void );
	int DelFromEpoll( void );
	void ResetRecvBuffer( unsigned int uReceivePos, unsigned int uReceiveSize );
	int ParseOnePacket( void );
	int PacketFinished( void ) const
	{
		return ( m_uReceiveSize == m_uReceivePos ) ? 1 : 0;
	}

	int SendConnectMsg( void );
	int SendToList(const char *pBuf, int nLen);
	
	
private:
	unsigned int       m_uReceivePos;	
	unsigned int       m_uReceiveSize;
	unsigned char    m_ucDestMachineType;
	unsigned short   m_usDestMachineIndex;
	bool                  m_bActive;
	enum State
	{
		ReadPacketHead = 0,
		ReadPacket
	};
		
	State m_state;

	std::vector<char>  m_vReceiveBuffer;
private:
	static std::list<TTaskConnectAttrib> m_listTaskConnectAttrib;

};
#endif //_COMMONTASK_H_H_

















