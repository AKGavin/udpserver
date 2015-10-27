/****************************************************************************
**
**  Copyright  (c)  2008-2009,  Baofeng, Inc.  All Rights Reserved.
**
**  Subsystem:    ConnectServer
**  File:         	     CommonDef2.h
**  Created By:   fwx
**  Created On:   2009/01/08
**
**  Purpose:
**     CommonDef2
**
**  History:
**  Programmer      Date         Version        Number      Description
**  --------------- --------   ---------- ------------------------------
**    fwx       2009/01/08         1.0            01          creation
****************************************************************************/
#ifndef _COMMONDEF_H_H_
#define _COMMONDEF_H_H_

namespace MYFRAMECOMMON{
	
#pragma pack( push, 1 )

#define PROTOCOLVER 0x0101
#define Q_SERVER 1
#define R_SERVER 2
#define PCACHE_SERVER 3
#define MEDIA_CENTER_SERVER 4

typedef struct tagCLIENTCOMMANDPACKET {
	struct {
		int iProtocolVer;
		int iSeq;
		int iCommandLen;
	} clientCommandPacketHead;

#define iProtocolVer clientCommandPacketHead.iProtocolVer

#define iSeq clientCommandPacketHead.iSeq

#define iCommandLen clientCommandPacketHead.iCommandLen

	char clientCommandPacketContent[0];
} CLIENTCOMMANDPACKET, * PCLIENTCOMMANDPACKET;

typedef struct tagSERVERCOMMANDPACKET {
	struct {
		unsigned short usMsgId;
		unsigned int     uSrcClientIp;
		unsigned short  usSrcClientPort;
		unsigned long long ullSrcTaskId;
		int iSrcProcessId;
		unsigned long long ullDestTaskId;
		int iDestProcessId;
		int iMsgLen;
	} serverCommandPacketHead;

#define usMsgId serverCommandPacketHead.usMsgId

#define uSrcClientIp serverCommandPacketHead.uSrcClientIp

#define usSrcClientPort serverCommandPacketHead.usSrcClientPort

#define ullSrcTaskId serverCommandPacketHead.ullSrcTaskId

#define iSrcProcessId serverCommandPacketHead.iSrcProcessId

#define ullDestTaskId serverCommandPacketHead.ullDestTaskId

#define iDestProcessId serverCommandPacketHead.iDestProcessId

#define iMsgLen serverCommandPacketHead.iMsgLen

	char serverCommandPacketContent[0];

	tagSERVERCOMMANDPACKET()
	{
		usMsgId			= -1;
		uSrcClientIp	= 0;
		usSrcClientPort	= 0;
		ullSrcTaskId	= -1;
		iSrcProcessId	= -1;
		ullDestTaskId	= -1;		
		iDestProcessId	= -1;
		iMsgLen			= 0;
	}

	void SetDefault()
	{
		usMsgId			= -1;		
		uSrcClientIp	= 0;
		usSrcClientPort	= 0;
		ullSrcTaskId	= -1;
		iSrcProcessId	= -1;
		ullDestTaskId	= -1;		
		iDestProcessId	= -1;
		iMsgLen			= 0;
	}

} SERVERCOMMANDPACKET, * PSERVERCOMMANDPACKET;

typedef struct tagSERVERINNERCOMMANDPACKET {
	struct {
		unsigned char ucDestMachineType;
		unsigned short usDestMachineIndex;
	} serverInnerCommandPacketHead;

#define ucDestMachineType serverInnerCommandPacketHead.ucDestMachineType

#define usDestMachineIndex serverInnerCommandPacketHead.usDestMachineIndex	

	SERVERCOMMANDPACKET serverCommandPacket;
}SERVERINNERCOMMANDPACKET, * PSERVERINNERCOMMANDPACKET;
#pragma pack( pop )
}
#endif //_COMMONDEF_H_H_

