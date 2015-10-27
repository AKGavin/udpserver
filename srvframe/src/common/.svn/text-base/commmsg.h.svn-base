/*
 * FileName:       
 * Author:         wangqifeng  Version: 1.0  Date: 2005-10-13
 * Description:    
 * Version:        
 * Function List:  
 *                 1.
 * History:        
 *     <author>   <time>    <version >   <desc>
 */
#ifndef __COMM_MSG_H
#define __COMM_MSG_H
#include <string>
#include <list>
#include "commonstat.h"
#include "timestat.h"
#include "framecommon/framecommon.h"
using namespace MYFRAMECOMMON;
using namespace std;

#define COMMON_PEERID_LEN 16
#define COMMON_CID_LEN 20



/*****************消息统一定义处******************/
//[0-999]的消息类型保留给用户
//这几条消息是定时器消息
const int TIMERADDTYPE=100000;
const int TIMERDELTYPE=100001;
const int TIMERREACHTYPE=100002;

const int PROCESSMSGTYPE	= 10000;		//wangtiefeng : 09-07-16 //common process msg type
const int UDPCLIENTMSG		= 10001;		//wangtiefeng : 09-07-24 //common udp client msg type

const int CLIENTMSGTYPE=2000; //客户端消息
const int RECVMSGTYPE=2001;
const int SENDMSGTYPE=2002;
const int RECVANDSENDMSGTYPE=2003;
const int CONNECTMSGTYPE=2004;
const int TOCONNECTMSGTYPE=2005;
const int CLIENTRETMSGTYPE = 2006;
const int CIDUPDATEMSGTYPE = 2007;

const int ONLINE_MESTYPE = 2009;		//wangtiefeng : 2009-01-10
const int ADDONLINE_MSGTYPE = 2010;		//wangtiefeng : 2009-01-10
const int RESYN_MSGTYPE = 2011;			//wangtiefeng : 2009-01-10
const int ONTIMESYN_MSGTYPE = 2012;		//wangtiefeng : 2009-01-10
const int SYN_AFFIRM_MSGTYPE = 2013;	//wangtiefeng : 2009-03-12
const int SYN_MSGTYPE = 2014;			//wangtiefeng : 2009-03-13
/******************************/
const int PCSERVER_MSGTYPE = 6000;			 //dijie 09-05-25
const int PCSERVER_BROADCAST_MSGTYPE = 6001; //dijie 09-6-3
/*****************************/


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//系统维护进程消息//外部查询消息类型
const int CMD_DEBUGINFO = 1001; //调试开关消息
const int PRINTSTATINFO = 1002;
const int PRINTTIMER = 1003;
const int ONLINE_CHECK_PEERSTORAGEINFO = 2000;		//wangtiefeng : 09-03-26
const int ONLINE_CHECK_PEERINFO = 2001;				//wangtiefeng : 09-03-26
const int QSERVER_CHECK_PEERSTORAGEINFO = 3000;		//wangtiefeng : 09-03-26
const int QSERVER_CHECK_PEERINFO = 3001;			//wangtiefeng : 09-03-26
const int QSERVER_CHECK_RC_PEERTOTALNO = 3002 ;		//wangtiefeng : 09-04-17
const int QSERVER_CHECK_SINGLERC_PEERNO = 3003;		//wangtiefeng : 09-04-17
const int PQSERVER_UPDATE_CDNCONF = 3004;			//wangtiefeng : 09-10-26
const int RSERVER_CHECK_PEERSTORAGEINFO = 4000;		//wangtiefeng : 09-03-26
const int RSERVER_CHECK_PEERINFO = 4001;			//wangtiefeng : 09-03-26
const int SUPERNODE_CHECK_PEEREXIST = 5000;			//wangtiefeng : 09-04-22


struct TMaintainHead
{
		int nMsgType;
        int nMsgBodyLen;        
		int GetSize()
		{
			int iLength = 0;
			iLength += sizeof( nMsgType );
			iLength += sizeof(  nMsgBodyLen );
			return iLength;
		}
		int Encode( CBufMgr &BufMgr )
		{
			int iRet = 0;
			iRet += BufMgr.PutInt( nMsgType );
			iRet += BufMgr.PutInt( nMsgBodyLen );
			return iRet;
		}
};

const int MAINTAINHEADSIZE = sizeof(TMaintainHead);

struct TMaintainHeadRes
{
	int nMsgType;
	int nMsgBodyLen;	
	char cReslut;
	int GetSize()
	{
		int iLength = 0;
		iLength += sizeof( nMsgType );
		iLength += sizeof( nMsgBodyLen );
		iLength += sizeof( cReslut );
		return iLength;
	}
	int Encode(  CBufMgr &BufMgr )
	{
		int iRet = 0;	
		iRet += BufMgr.PutInt( nMsgBodyLen );
		iRet += BufMgr.PutInt( nMsgType );
		iRet += BufMgr.PutChar( cReslut );
		return iRet;
	}
	int Decode( CBufMgr &BufMgr )
	{
		int iRet = 0;
		iRet += BufMgr.GetInt( nMsgType );
		iRet += BufMgr.GetInt( nMsgBodyLen );		
		iRet += BufMgr.GetChar( cReslut );
		return iRet;
	}
};



//调试开关关闭消息
struct CmdModLogInfo
{
		int iFlag;
        int iModLogType;
        

		int GetSize()
		{
			int iLength = 0;
			iLength += sizeof(  iFlag );
			iLength += sizeof( iModLogType );
			return iLength;
		}
		int Encode( CBufMgr &BufMgr  )
		{
			int iRet = 0;
			iRet += BufMgr.PutInt( iFlag );
			iRet += BufMgr.PutInt( iModLogType );
			return iRet;
		}
};

//调试：返回的OnlinePeerStorage状态信息
struct TECOnlinePeerStorageRes
{
	unsigned int uiHashLength;
	unsigned short usOnlineServerNum;
	unsigned short usOnlineServerIndex;
	unsigned int uiPeerNum;

	int GetSize()
	{
		int iLength = 0;
		iLength += sizeof( uiHashLength );
		iLength += sizeof( usOnlineServerNum );
		iLength += sizeof( usOnlineServerIndex );
		iLength += sizeof( uiPeerNum );
		return iLength;
	}

	int Encode(  CBufMgr &BufMgr )
	{
		int iRet = 0;
		iRet += BufMgr.PutInt( uiHashLength );
		iRet += BufMgr.PutShort( usOnlineServerNum );
		iRet += BufMgr.PutShort( usOnlineServerIndex );
		iRet += BufMgr.PutInt( uiPeerNum );
		return iRet;
	}

	int Decode( CBufMgr &BufMgr )
	{
		int iRet = 0;
		iRet += BufMgr.GetInt( (int&)uiHashLength );
		iRet += BufMgr.GetShort( (short&)usOnlineServerNum );
		iRet += BufMgr.GetShort( (short&)usOnlineServerIndex );
		iRet += BufMgr.GetInt( (int&)uiPeerNum );
		return iRet;
	}

};


//调试：查询特定Peer在线信息
struct TECOnlinePeerCheck
{
	char szPeerId[COMMON_PEERID_LEN];
	int Encode( CBufMgr &BufMgr )
	{
		int iRet = 0;
		iRet += BufMgr.PutInt( COMMON_PEERID_LEN );
		iRet += BufMgr.PutCharBuf( szPeerId , COMMON_PEERID_LEN );
		return iRet;
	}

	int Decode( CBufMgr &BufMgr )
	{
		int iRet = 0;
		int iPeerIdLen;
		iRet += BufMgr.GetInt( iPeerIdLen );
		if( iRet != 0 || iPeerIdLen !=  COMMON_PEERID_LEN )
			return -1;
		iRet += BufMgr.GetCharBuf( szPeerId , COMMON_PEERID_LEN );
		return iRet;
	}

	int GetSize()
	{
		int iLength = 0;
		iLength += COMMON_PEERID_LEN + sizeof( int );
		return iLength;
	}
};

//调试：查询特定Peer在线信息(返回)
struct TECOnlinePeerCheckRes
{
	char szPeerId[COMMON_PEERID_LEN];
	unsigned int uiInternalIp;
	unsigned short usInternalPort;
	unsigned int uiPublicIp;
	unsigned short usPublicPort;
	int iAttribute;
	char cLogoutFlag;
	int Decode( CBufMgr &BufMgr )
	{
		int iRet = 0;
		int iPeerIdLen; 
		iRet += BufMgr.GetInt( iPeerIdLen );
		if(  iRet != 0 || iPeerIdLen != COMMON_PEERID_LEN  )
			return -1;
		iRet += BufMgr.GetCharBuf( szPeerId , iPeerIdLen );
		iRet += BufMgr.GetInt( (int&) uiInternalIp );
		iRet += BufMgr.GetShort( (short&)usInternalPort );
		iRet += BufMgr.GetInt( (int&)uiPublicIp );
		iRet += BufMgr.GetShort( (short&)usPublicPort );
		iRet += BufMgr.GetInt( iAttribute );
		iRet += BufMgr.GetChar( cLogoutFlag );
		return iRet;
	}
	int Encode( CBufMgr &BufMgr )
	{
		int iRet = 0;
		iRet += BufMgr.PutInt( COMMON_PEERID_LEN );
		iRet += BufMgr.PutCharBuf( szPeerId ,COMMON_PEERID_LEN );
		iRet += BufMgr.PutInt( uiInternalIp );
		iRet += BufMgr.PutShort( usInternalPort );
		iRet += BufMgr.PutInt( uiPublicIp );
		iRet += BufMgr.PutShort( usPublicPort );
		iRet += BufMgr.PutInt( iAttribute );
		iRet += BufMgr.PutChar( cLogoutFlag );
		return iRet;
	}

	int GetSize()
	{
		int iLength = 0;
		iLength += COMMON_PEERID_LEN + sizeof( int );
		iLength += sizeof( uiInternalIp );
		iLength += sizeof( usInternalPort );
		iLength += sizeof( uiPublicIp );
		iLength += sizeof( usPublicPort );
		iLength += sizeof( iAttribute );
		iLength += sizeof( cLogoutFlag );
		return iLength;
	}
};
//调试：查询supernode中是否含有某个Peer
struct TECSupernodePeerExist
{
	char szPeerId[COMMON_PEERID_LEN];
	int Decode( CBufMgr &BufMgr )
	{
		int iRet = 0;
		int iPeerIdLen;
		iRet += BufMgr.GetInt( iPeerIdLen );
		if( iRet != 0 || iPeerIdLen != COMMON_PEERID_LEN )
			return -1;
		iRet += BufMgr.GetCharBuf( szPeerId , iPeerIdLen );
		return iRet;
	}
	int Encode( CBufMgr &BufMgr )
	{
		int iRet = 0;
		iRet += BufMgr.PutInt( COMMON_PEERID_LEN );
		iRet += BufMgr.PutCharBuf( szPeerId , COMMON_PEERID_LEN );
		return iRet;
	}
	int GetSize()
	{
		int iLength = 0;
		iLength += COMMON_PEERID_LEN + sizeof( int );
		return iLength;
	}
};

struct TECSupernodePeerExistRes
{
	char szPeerId[COMMON_PEERID_LEN];
	int iPublicIp;
	short sPort;
	int Decode( CBufMgr &BufMgr )
	{
		int iRet = 0;
		int iPeerIdLen;
		iRet += BufMgr.GetInt( iPeerIdLen );
		if( iRet != 0 || iPeerIdLen != COMMON_PEERID_LEN )
			return -1;
		iRet += BufMgr.GetCharBuf( szPeerId , iPeerIdLen );
		iRet += BufMgr.GetInt( iPublicIp );
		iRet += BufMgr.GetShort( sPort );
		return iRet;
	}
	int Encode( CBufMgr &BufMgr )
	{
		int iRet = 0;
		iRet += BufMgr.PutInt( COMMON_PEERID_LEN );
		iRet += BufMgr.PutCharBuf( szPeerId , COMMON_PEERID_LEN );
		iRet += BufMgr.PutInt( iPublicIp );
		iRet += BufMgr.PutShort( sPort );
		return iRet;
	}
	int GetSize()
	{
		int iLength = 0;
		iLength += COMMON_PEERID_LEN + sizeof( int );
		iLength += sizeof( int );
		iLength += sizeof( short );
		return iLength;
	}
};

//调试：查询Qserver中特定Cid下的Peer数
struct TECQserverRCPeerNo
{
	char szCid[COMMON_CID_LEN];
	int Decode( CBufMgr &BufMgr )
	{
		int iRet = 0;
		int iCidIdLen; 
		iRet += BufMgr.GetInt( iCidIdLen );
		if(  iRet != 0 || iCidIdLen != COMMON_CID_LEN  )
			return -1;
		iRet += BufMgr.GetCharBuf( szCid , iCidIdLen );
		return iRet;
	}
	int Encode( CBufMgr &BufMgr )
	{
		int iRet = 0;
		iRet += BufMgr.PutInt( COMMON_CID_LEN );
		iRet += BufMgr.PutCharBuf( szCid ,COMMON_CID_LEN );
		return iRet;
	}
	int GetSize()
	{
		int iLength = 0;
		iLength += COMMON_CID_LEN + sizeof( int );
		return iLength;
	}

};
//调试：查询Qserver中特定Cid下的Peer数(返回)
struct  TECQserverRCPeerNoRes
{
	int iPeerNum;
	int Decode( CBufMgr &BufMgr )
	{
		int iRet = 0;
		iRet += BufMgr.GetInt( iPeerNum );
		return iRet;
	}
	int Encode( CBufMgr &BufMgr )
	{
		int iRet = 0;
		iRet += BufMgr.PutInt( iPeerNum );
		return iRet;
	}
	int GetSize()
	{
		int iLength = 0;
		iLength += sizeof( int );
		return iLength;
	}
};
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const int REPLYSUCCEED=0;  //成功响应

struct TRequestHead
{
        int nHeadLen;
        int nBodyLen;
        int nMsgType;
        int nMsgProctol;
        int nSequenceNum;
        int Encode(CBufMgr &bufMgr)
        {
                int ret = 0;
                ret += bufMgr.PutInt(nHeadLen);
                ret += bufMgr.PutInt(nBodyLen);
                ret += bufMgr.PutInt(nMsgType);
                ret += bufMgr.PutInt(nMsgProctol);
                ret += bufMgr.PutInt(nSequenceNum);
                return ret;
        }
        int Decode(CBufMgr &bufMgr)
        {
                int ret = 0;
                ret += bufMgr.GetInt(nHeadLen);
                ret += bufMgr.GetInt(nBodyLen);
                ret += bufMgr.GetInt(nMsgType);
                ret += bufMgr.GetInt(nMsgProctol);
                ret += bufMgr.GetInt(nSequenceNum);
                return ret;                
        }
        int Size()
        {
                int nLen = 20;
                return nLen;
        }

        int Clone(TRequestHead &reqHead)
        {
                nHeadLen = reqHead.nHeadLen;
                nBodyLen = reqHead.nBodyLen;
                nMsgType = reqHead.nMsgType;
                nMsgProctol = reqHead.nMsgProctol;
                nSequenceNum = reqHead.nSequenceNum;
                return 0;
        }
};

struct TResponseHead
{
        TRequestHead reqHead;
        int nResult;
        int Encode(CBufMgr &bufMgr)
        {
                int ret = 0;
                ret += reqHead.Encode(bufMgr);
                ret += bufMgr.PutInt(nResult);
                return ret;
        }
        
        int Decode(CBufMgr &bufMgr)
        {
                int ret = 0;
                reqHead.Decode(bufMgr);
                bufMgr.GetInt(nResult);
                return ret;                
        }
        int Size()
        {
                int nLen = reqHead.Size() + 4;
                return nLen;
        }

        int Clone(TResponseHead &respHead)
        {
                reqHead.Clone(respHead.reqHead);
                nResult = respHead.nResult;
                return 0;
        }
};


//内部头消息结构
struct TMsg
{
        int destEntityType;  //目的实体类型
        int destEntityId;      //目的实体ID       
        int srcEntityType;   //源实体类型
        int srcEntityId;       //源ID        
        ULONG64  srcTaskId;
        ULONG64        taskId;       //任务id
        int msgType;          //消息类型
        TMsg()
        {
                destEntityType = 0;  //目的实体类型
                destEntityId = 0;      //目的实体ID       
                srcEntityType = 0;   //源实体类型
                srcEntityId = 0;       //源ID        
                srcTaskId = 0;
                taskId = 0;       //任务id
                msgType = 0;          //消息类型
        }
};


/**
* dec:
* ClassName:CMsgBody
* author: 
* version:
*CreateTime: 2005.10.13
*/
class CMsgBody
{
public:
        CMsgBody(){};
        virtual ~CMsgBody(){};
        virtual int Size()=0;
        virtual CMsgBody *Clone()=0;
private:
};


class CMsgResBody:public CMsgBody
{
public:
        CMsgResBody()
        {
                m_iResult = REPLYSUCCEED;
        };
        virtual ~CMsgResBody(){};
        virtual int Size()=0;
        virtual CMsgBody *Clone()=0;

        int GetResult()
        {
                return m_iResult;
        };
        
        void SetResult(int iResult)
        {
                m_iResult = iResult;
        }
        
        string GetErrorInfo()
        {
                return m_sErrorInfo;
        }

        void SetErrorInf(string &errInfo)
        {
                m_sErrorInfo = errInfo;
        }
private:
        int m_iResult; //响应的结果0:成功
        string m_sErrorInfo; //错误原因 
};




/**
* dec:
* ClassName:CMsg
* author: 
* version:
*CreateTime: 2005.10.13
*/
class CMsg
{
public:  
        CMsg(TMsg &tMsg, CMsgBody *pMsgBody)
        {
                memcpy(&m_tMsg, &tMsg, sizeof(TMsg));
                m_pMsgBody = pMsgBody;
        }
        
        CMsg(CMsg &msg)
        {
                memcpy(&m_tMsg, &msg.m_tMsg, sizeof(TMsg));
                if (msg.m_pMsgBody != NULL)
                {
                        m_pMsgBody = (msg.m_pMsgBody)->Clone();
                }
                else
                {
                        m_pMsgBody = NULL;
                }
        }
        
        ~CMsg()
        {
                if (m_pMsgBody != NULL)
                {
                        delete m_pMsgBody;
                }
        };
        
        int Size()
        {
                int len = sizeof(TMsg);
                if (m_pMsgBody != NULL)
                {
                        len += m_pMsgBody->Size();
                }
                return  len;
        };

        CMsgBody *GetMsgBody()
        {
                return m_pMsgBody;
        }

        TMsg *GetTMsg()
        {
                return &m_tMsg;
        }

        
        void Print(int logType=0)
        {
                if (logType == 0)//trace
                {
                    WRITE_TRACE("CMsg info:\n"
                                " msgtype:%d\n"
                                " srcEntity:%d\n"
                                " srcEntityId:%d\n"
                                " srcTaskId:%llu\n"
                                " destEntityType:%d\n"
                                " destEntityId:%d\n"
                                " destTaskId:%llu", m_tMsg.msgType, m_tMsg.srcEntityType, m_tMsg.srcEntityId, m_tMsg.srcTaskId,
                                m_tMsg.destEntityType, m_tMsg.destEntityId, m_tMsg.taskId);
                }
                else //log
                {
                        WriteRunInfo::WriteLog("CMsg info:\n"
                                " msgtype:%d\n"
                                " srcEntity:%d\n"
                                " srcEntityId:%d\n"
                                " srcTaskId:%llu\n"
                                " destEntityType:%d\n"
                                " destEntityId:%d\n"
                                " destTaskId:%llu", m_tMsg.msgType, m_tMsg.srcEntityType, m_tMsg.srcEntityId, m_tMsg.srcTaskId,
                                m_tMsg.destEntityType, m_tMsg.destEntityId, m_tMsg.taskId);
                }
        }

		void SetMsgBody(CMsgBody *pBody)
		{
			m_pMsgBody = pBody;
		}
private:
        TMsg m_tMsg;
        CMsgBody *m_pMsgBody;
};



/**
* dec:
* ClassName:CBufMsgBody
* author: 
* version:
*CreateTime: 2005.10.13
*/
class CBufMsgBody:public CMsgBody
{
public:
        CBufMsgBody(char *pBuf, int bufLen)
        {
                m_pBuf = pBuf;
                m_iBufLen = bufLen;
        }
        
        virtual ~CBufMsgBody()
        {
                if (m_pBuf != NULL)
                {
                        delete [] m_pBuf;
                }
        }
        
        int Size()
        {
                return m_iBufLen;
        }

        CMsgBody *Clone()
        {
                char *pBuf = new char[m_iBufLen];
                memcpy(pBuf, m_pBuf, m_iBufLen);
                CBufMsgBody *pMsgBody = new CBufMsgBody(pBuf, m_iBufLen);
                return pMsgBody;
        }

        char *GetBuf()
        {
                return m_pBuf;                
        }

        void SetBuf(char *pBuf, int nLen)
        {
               m_pBuf = pBuf;
               m_iBufLen = nLen;
        }
private:
        char *m_pBuf;
        int    m_iBufLen;
};


class CClientBufBody:public CBufMsgBody
{
public:

        CClientBufBody(char *pBuf, int bufLen, CMySocket *pTcpSock, int nComType):CBufMsgBody(pBuf, bufLen)
        {
                m_nComType = nComType; //0:非http  1: http类型
                m_pTcpSock = pTcpSock;
        }

        virtual ~CClientBufBody()
        {
                if (m_pTcpSock != NULL)
                {
                        delete m_pTcpSock;
                }                
        }

        int GetComType()
        {
                return m_nComType;
        }


        CMySocket *GetSocket()
        {
                return m_pTcpSock;
        }

        void SetSocket(CMySocket *pSocket)
        {
                m_pTcpSock = pSocket;
        }

        void SetTimeStat(CTimeStat &timeStat)
        {
                timeStat.Clone(m_timeStat);
        }

        CTimeStat &GetTimeStat()
        {
                return m_timeStat;
        }
private:

        int m_nComType;
        
        CMySocket *m_pTcpSock;
        CTimeStat m_timeStat;
};

class CSendMsgBody:public CMsgBody
{
public:
        CSendMsgBody(CTcpSocket *pTcpSocket, char *pSendBuf, int nSendLen)
        {
                m_pTcpSocket = pTcpSocket;
                m_pSendBuf = pSendBuf;
                m_nSendLen = nSendLen;
        }
        ~CSendMsgBody()
        {
                if (m_pTcpSocket != NULL)
                {
                        delete m_pTcpSocket;
                }
                if (m_pSendBuf != NULL)
                {
                        delete [] m_pSendBuf;
                }
        }
        int Size()
        {
                return 0;
        }        
        CMsgBody *Clone()
        {
                return NULL;
        }

        CTcpSocket* GetTcpSocket()
        {
                return m_pTcpSocket;
        }
        char *GetSendBuf()
        {
                return m_pSendBuf;
        }
        int GetSendLen()
        {
                return m_nSendLen;
        }

        void SetTcpSocket(CTcpSocket *pSocket)
        {
                m_pTcpSocket = pSocket;
        }
        void SetSendBuf(char *pSendBuf)
        {
                m_pSendBuf = pSendBuf;
        }
        
private:
        CTcpSocket *m_pTcpSocket;
        char *m_pSendBuf;
        int m_nSendLen;
};

class CRecvMsgBody:public CMsgBody
{
public:
        CRecvMsgBody(int nSocket, sockaddr_in &tmpaddr)
        {
                m_nSocket = nSocket;
                memcpy(&m_sockAddr, &tmpaddr, sizeof(sockaddr_in));
        }
        ~CRecvMsgBody()
        {            
        }
        int Size()
        {
                return 0;
        }        
        CMsgBody *Clone()
        {
                return NULL;
        }    

        int GetSocket()
        {
                return m_nSocket;
        }

        sockaddr_in &GetSockAddr()
        {
                return m_sockAddr;
        }

        void SetTimeStat(CTimeStat &timeStat)
        {
                timeStat.Clone(m_timeStat);
        }

        CTimeStat &GetTimeStat()
        {
                return m_timeStat;
        }
private:
        int m_nSocket;
        sockaddr_in m_sockAddr;

        CTimeStat m_timeStat;
};

class CClientMsgBody 
	: public CMsgBody
{
public:
	CClientMsgBody( char *pBuffer, int iLen )
		: m_pBuffer( pBuffer )
		, m_iLen( iLen )
	{
	}
	
	virtual ~CClientMsgBody( void )
	{
		if ( m_pBuffer )
		{
			delete [] m_pBuffer;
		}
	}

        int Size( void )
        {
                return 0;
        }        
        CMsgBody *Clone( void )
        {
                return NULL;
        }    

	 char * GetBuffer( void )
	 {
	 	return m_pBuffer;
	 }

	 void DisAttachBuf()
	 {
		 m_pBuffer = NULL;
		 m_iLen = 0;
	 }

	int GetLen( void )
	{
		return m_iLen;
	}
	 
private:
	char *m_pBuffer;
	int m_iLen;
};

class CClientRetMsgBody 
	: public CMsgBody
{
public:
	CClientRetMsgBody( unsigned long long ullTaskId, char *pBuffer, int iLen )
		: m_ullDestTaskId( ullTaskId )
		, m_pBuffer( pBuffer )
		, m_iLen( iLen )
	{
	}
	
	virtual ~CClientRetMsgBody( void )
	{
		if ( m_pBuffer )
		{
			delete []m_pBuffer;
		}
	}

        int Size( void )
        {
                return 0;
        }        
        CMsgBody *Clone( void )
        {
                return NULL;
        }    

	unsigned long long GetDestTaskId( void )
	{
		return m_ullDestTaskId;
	}
	
	 const char * GetBuffer( void )
	 {
	 	return m_pBuffer;
	 }

	int GetLen( void )
	{
		return m_iLen;
	}
	 
private:
	unsigned long long m_ullDestTaskId;
	char *m_pBuffer;
	int m_iLen;
};

class CConnectMsgBody
	: public CMsgBody
{
public:
        CConnectMsgBody( int iSocket, const char * pcszRemoteIp, unsigned short usRemotePort, unsigned char ucMachineType, unsigned short usMachineIndex, int iState )
			: m_iSocket( iSocket )
			, m_sRemoteIp( pcszRemoteIp )
			, m_usRemotePort( usRemotePort )
			, m_ucMachineType( ucMachineType )
			, m_usMachineIndex( usMachineIndex )
			, m_iState( iState )
        {
        }
        virtual ~CConnectMsgBody( void )
        {
        	if ( m_iSocket != -1 && m_iState != 0 )
        	{
        		::close( m_iSocket );
        	}
        }
        int Size( void )
        {
                return 0;
        }        
        CMsgBody *Clone( void )
        {
                return NULL;
        }    

        int GetSocket( void )
        {
                return m_iSocket;
        }

	const char * GetRemoteIp( void )
	{
		return m_sRemoteIp.c_str();
	}

	unsigned short GetRemotePort( void )
	{
		return m_usRemotePort;
	}

	unsigned char GetDestMachineType( void )
	{
		return m_ucMachineType;
	}

	unsigned short GetDestMachineIndex( void )
	{
		return m_usMachineIndex;
	}
	
	int GetState( void )
	{
		return m_iState;
	}

private:
        int m_iSocket;
	string m_sRemoteIp;
	unsigned short m_usRemotePort;
	unsigned char m_ucMachineType;
	unsigned short m_usMachineIndex;
        int m_iState;

};

class CToConnectMsgBody
	: public CMsgBody
{
public:
        CToConnectMsgBody( const char * pcszRemoteIp, unsigned short usRemotePort, unsigned char ucMachineType, unsigned short usMachineIndex )
			: m_sRemoteIp( pcszRemoteIp )
			, m_usRemotePort( usRemotePort )
			, m_ucMachineType( ucMachineType )
			, m_usMachineIndex( usMachineIndex )
        {
        }
        virtual ~CToConnectMsgBody( void )
        {
        }
        int Size( void )
        {
                return 0;
        }        
        CMsgBody *Clone( void )
        {
                return NULL;
        }    

	const char * GetRemoteIp( void )
	{
		return m_sRemoteIp.c_str();
	}

	unsigned short GetRemotePort( void )
	{
		return m_usRemotePort;
	}

	unsigned char GetDestMachineType( void )
	{
		return m_ucMachineType;
	}

	unsigned short GetDestMachineIndex( void )
	{
		return m_usMachineIndex;
	}
	
private:
	string m_sRemoteIp;
	unsigned short m_usRemotePort;
	unsigned char m_ucMachineType;
	unsigned short m_usMachineIndex;
};

class CUdpCommonBufMsgBody : public CBufMsgBody
{
public:
	CUdpCommonBufMsgBody( char *pBuf, int iBufLen , int iSocket , unsigned int uiClientIp , unsigned short usClientPort )
		: CBufMsgBody( pBuf , iBufLen )
	{
		m_iSocket = iSocket;
		m_uiClientIp = uiClientIp;
		m_usClientPort = usClientPort;
	}
	int GetSocket()
	{
		return m_iSocket;
	}
	int GetClientIp()
	{
		return m_uiClientIp;
	}
	int GetCientPort()
	{
		return m_usClientPort;
	}

private:
	int m_iSocket;
	unsigned int m_uiClientIp;
	unsigned short m_usClientPort;
};


#endif

