/*
 * FileName:       
 * Author:         wangqifeng  Version: 1.0  Date: 2006-2-16
 * Description:    
 * Version:        
 * Function List:  
 *                 1.
 * History:        
 *     <author>   <time>    <version >   <desc>
 */
#ifndef  __COMMON_DEF_H
#define __COMMON_DEF_H
#include <string>
using namespace std;
namespace MYFRAMECOMMON{
typedef unsigned long long ULONG64;
typedef long long LONG64;

#define NAT_PEERID_LEN 16
#define NAT_UDT_HEAD_LEN 16
#define NAT_UDT_HEAD "kjkjkjkjkjkjkjkj"

const int TCPNORMALTYPE=0;
const int HTTPPROXY=1;
const int UDPTYPE=128;
const int HTTPGETTYPE=129;

const int TENMINUTE=600;
const int ONEHOUR=3600;
const int ONEDAY=86400;

const int MAXSQLLEN=50*1024; //sql缓冲的长度


const int SENDDATATIMEOUTEVENT=1000;
const int SENDDATATIMELENGTH=60;
const int RECVDATATIMEOUTEVENT=1001;
const int RECVDATATIMELENGTH = 60;

//[10-99]的实体类型保留给用户
const int TIMERENTITY=0;
const int LISTENENTITY=100;
const int SERVICEPROENTITY=101;
const int MAINTAINENTITY=102;
const int RECVANDSENDMSGENTITY=103;
const int MAINTAINUDPENTITY=104;
const int MAINENTITY=105;
const int LONGCONNECTEDENTITY=106;
const int EPOLLREACTORENTITY=107;
const int LONGCONNECTEDPROCESSENTITY=108;
const int LONGCONNECTEDLISTENENTITY=109;
const int ONLINEUDPRECVENTITY = 110;	//wangtiefeng : 09-02-09
const int ONLINEUDPSENDENTITY = 111;	//wangtiefeng : 09-02-09
const int ONTIMESYNSENDENTITY = 112;	//wangtiefeng : 09-02-09
const int LOGICPROCESSENTITY = 113;
const int ONLINEPEERSTORAGEENTITY = 114;	//wangtiefeng : 01-03-15
const int PINGLOGUDPRECVENTITY = 115;		//hanzhiliang : 09-04-08
const int PINGLOGTIMERINTERVALENTITY = 116; //hanzhiliang : 09-04-08
const int PINGLOGDECODELOGENTITY = 117;		//hanzhiliang : 09-05-08

	
//PC Server
const int PCUDPRECVENTITY = 118;
const int PCUDPSENDENTITY = 119;
const int PCBROADCASTENTITY = 120;

//common process thread type
const int PROCESSENTITY = 121;					//wangtiefeng : 09-07-16
const int UDPRECVENTITY = 122;					//wangtiefeng : 09-07-24
const int UDPSENDANDPROCESSENTITY = 123;		//wangtiefeng : 09-07-24


//machine type
const int SEEDMACHINETYPE = 0;				//wangtiefeng : 09-02-10
const int CONNECTSERVERMACHINETYPE = 1;		//wangtiefeng : 09-02-10
const int QSERVERMACHINETYPE = 2;			//wangtiefeng : 09-02-10
const int RSERVERMACHINETYPE = 3;			//wangtiefeng : 09-02-10
const int ONLINEMACHINETYPE = 4;			//wangtiefeng : 09-02-10


/************************************/
const int STATUPDATETIMEREVENT=102;
const int STATUPDATETIMERLENGTH=600;


struct TDbItem
{
        string sDbHost;
        string sDbUser;        
        string sDbPasswd;        
};

class CTaskArg
{
public:
	CTaskArg( void )
	{
	}
	
	virtual ~CTaskArg( void )
	{
	}
};

class CCommonTaskArg
	: public CTaskArg
{
public:
	CCommonTaskArg( unsigned char cMachineType, unsigned short usMachineIndex, bool bActive, int iListType )
		: m_cMachineType( cMachineType )
		, m_usMachineIndex( usMachineIndex )
		, m_bActive( bActive )
		, m_iListType( iListType )
	{
	}
	virtual ~CCommonTaskArg( void )
	{
	}

	unsigned char GetDestMachineType( void )
	{
		return m_cMachineType;
	}

	unsigned short GetDestMachineIndex( void )
	{
		return m_usMachineIndex;
	}

	bool IsActive( void )
	{
		return m_bActive;
	}

	int GetListType( void )
	{
		return m_iListType;
	}
private:
	unsigned char m_cMachineType;
	unsigned short m_usMachineIndex;
	bool                m_bActive;
	int                   m_iListType;
};

}
#endif

