/*
 * =====================================================================================
 *
 *       Filename:  command.h
 *
 *    Description:  命令的公共接口
 *
 *        Version:  1.0
 *        Created:  2009年01月16日 20时13分26秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Dr. Fritz Mehner (mn), mehner@fh-swf.de
 *        Company:  FH S眉dwestfalen, Iserlohn
 *
 * =====================================================================================
 */
#ifndef __COMMON_COMMAND_H
#define __COMMON_COMMAND_H
#include "framecommon/framecommon.h"
#include <string>
#include <vector>
#include "protocol.h"
#include "framecommon/buffermgr.h"
using namespace std;
using namespace MYFRAMECOMMON;
//define command id
const int ISRCONLINEREQ=1000;
const int ISRCONLINERESP=1001;
const int REPORTRCLISTREQ=1002;
const int REPORTRCLISTRESP=1003;
const int INSERTRCREQ=1004;
const int INSERTRCRESP=1005;
const int DELETERCREQ=1006;
const int DELETERCRESP=1007;
const int PEERQUERYREQ=1008;
const int PEERQUERYRESP=1009;

const int INVALIDPEERREQ=1010;

const int INVALID_CMD_ID=-1; //zz
//*******************************************/
const int REQHEADLEN=14;
const int CMDIDOFF=12;

const unsigned int PEERLENGTH=16; //peeridlen
const unsigned int CIDLEN=20;    //cidlen

/*********************************************/
//pcserver command id
const int REQUESTNEIGHBORREQ=5000;
const int REQUESTNEIGHBORRES=5001;
const int REPORTNEIGHBORREQ=5002;
const int REPORTNEIGHBORRES=5003;
const int QUERYNEIGHBORREQ=5004;
const int QUERYNEIGHBORRES=5005;
const int BROADCASTMSGTOCLIENT=5006; //pcserver向客户端发送广播消息
const int REQUESTPUBLICKEYREQ=5008;
const int REQUESTPUBLICKEYRES=5009;
const int SENDMSGTOPCSERVERREQ=5010; //消息发布管理器向pcserver发送需要广播的消息
const int SENDMSGTOPCSERVERRES=5011;//对5010的回应

/********************************************/

struct CBaseCommandHead
{
		int m_nProtocolVersion; //协议版本
		int m_nSeqence; //消息序列号
		int m_nBodylen; //消息体长度
		short m_nCmd;
};

struct CDefault
{
	int Decode (CBufMgr &bufMgr)
	{
		return 0;
	}
};

struct CDefaultRes
{
	int Encode(CTmpBufMgr &bufMgr)
	{
		return 0;
	}
    int GetSize()
    {
        return 0;
    }
};

class CCommand
{
    public:
        CCommand(){m_nIfRes=1;}
        virtual ~CCommand(){}
        virtual int Process(int &nResult)=0;
		void SetIp(int nIp)
		{
			m_nIp = nIp;
		}
		virtual char *GetResBuf(int &nLen)=0;
		virtual short GetMsgType()=0;
		virtual void DisAttachBuf()=0;
        int GetIfRes()
        {
            return m_nIfRes;
        }
    protected:
        virtual int Encode()=0;
        virtual int Decode()=0;
		int m_nIp;
        int m_nIfRes;
};

template <class T1, class T2=CDefaultRes> 
class CBaseCommand:public CCommand
{
	public:
		static int GetCommandId(char *pBuf, int nLen) //zz
		{
			if(nLen < REQHEADLEN) 
			{
				WriteRunInfo::WriteLog("The cmd buf len is less than %d, "
						"too short", REQHEADLEN);
				return INVALID_CMD_ID;
			}

			unsigned short sCmdId;
			memcpy(&sCmdId, pBuf + CMDIDOFF, sizeof(sCmdId));
			return	ntohs(sCmdId);
		}
		CBaseCommand(char *pBuf, int nLen):m_outBuf( NULL , 0 , 0 )
		{
			m_outHead.m_nProtocolVersion = VOD_PROTOCOL_VER;
			m_inBuf.AttachBuf(pBuf, nLen);
            m_nIfDecode=0;
            m_nResult = -1;
		}
		virtual ~CBaseCommand(){}

		int Process2()
		{
			return ProcessCmd();
		}

		virtual int Process(int &nResult) //modified by dijie 09-11-4
		{
			//解码
			nResult = 0;
		    int nRet=Decode();
            if (nRet == 0)
            {
                nResult =  ProcessCmd();
                m_nResult = nResult;
                if (nResult == 1)
                    m_nResult = 0;
            }
            
            if (nResult <= 1 && m_nIfRes==1) //>0一般是暂时不需要进行处理
            {   
                GenResHead(); //生成返回的头部
                nRet=Encode();
            }			
			return nRet;
		}

		T1 &GetIn()
		{
			return m_in;
		}

		char *GetResBuf(int &nLen)
		{
			nLen = m_outBuf.GetRealBufLen();
			return (char*)m_outBuf.GetBuf();
		}

		void DisAttachBuf()
		{
			m_outBuf.DisAttachBuf();
		}

		short GetMsgType()
			{
			return m_inHead.m_nCmd;
			}
	protected:
        void GenResHead()
        {
            m_outHead.m_nProtocolVersion = m_inHead.m_nProtocolVersion;
            m_outHead.m_nSeqence = m_inHead.m_nSeqence;
            m_outHead.m_nBodylen = 3;
            if (m_nResult == 0)
            {
                m_outHead.m_nBodylen += m_out.GetSize();
            }
        }
		int Encode()
		{
			int iPacketLength = sizeof(SERVERINNERCOMMANDPACKET) + m_out.GetSize() + 3*sizeof(int) + sizeof(short) + sizeof( char );
			char * pEncodeBuf = new char[iPacketLength];
			m_outBuf.AttachBuf( pEncodeBuf , iPacketLength );
			m_outBuf.SetOffset( sizeof(SERVERINNERCOMMANDPACKET) );

			int ret=0;
			ret+=m_outBuf.PutInt(m_outHead.m_nProtocolVersion);
			ret+=m_outBuf.PutInt(m_outHead.m_nSeqence);
			ret+=m_outBuf.PutInt(m_outHead.m_nBodylen);
			ret+=m_outBuf.PutShort(m_outHead.m_nCmd);
			ret+=m_outBuf.PutChar(m_nResult);
			if (m_nResult == 0)
			{				
				ret += m_out.Encode(m_outBuf);
			}
			return ret;
		}
		int Decode()
		{
			int ret=0;
            if (m_nIfDecode == 0)
                {
			ret+=m_inBuf.GetInt(m_inHead.m_nProtocolVersion);
			ret+=m_inBuf.GetInt(m_inHead.m_nSeqence);
			ret+=m_inBuf.GetInt(m_inHead.m_nBodylen);
			ret+=m_inBuf.GetShort(m_inHead.m_nCmd);
			if (m_inHead.m_nBodylen + 12 != m_inBuf.GetBufLen())
			{
				WriteRunInfo::WriteLog("The body len is invalid, %d != %d",
						m_inHead.m_nBodylen + 12, m_inBuf.GetBufLen());
				return -1;
			}
			ret+=m_in.Decode(m_inBuf);	
            m_nIfDecode = 1;
                }            
			return ret;
		}


		virtual int ProcessCmd() = 0;
		int m_nIfDecode;
		CBaseCommandHead m_inHead;	
		CBaseCommandHead m_outHead;	
		CBufMgr m_inBuf;
		CTmpBufMgr m_outBuf;
		int m_nResult;
		T1 m_in;
		T2 m_out;
};

//全网联通服务器 使用
#define UDT_HEAD_LEN 16
#define UDT_HEAD "kjkjkjkjkjkjkjkj"

template <class T1, class T2=CDefaultRes> 
class CUdpBaseCommand:public CCommand
{
	public:
		static int GetCommandId(char *pBuf, int nLen) //zz
		{
			if(nLen < REQHEADLEN) 
			{
				WriteRunInfo::WriteLog("The cmd buf len is less than %d, "
						"too short", REQHEADLEN);
				return INVALID_CMD_ID;
			}

			unsigned short sCmdId;
			memcpy(&sCmdId, pBuf + CMDIDOFF, sizeof(sCmdId));
			return	ntohs(sCmdId);
		}
		CUdpBaseCommand(char *pBuf, int nLen):m_outBuf( NULL , 0 , 1 )
		{
			m_outHead.m_nProtocolVersion = VOD_PROTOCOL_VER;
			m_inBuf.AttachBuf(pBuf, nLen);
            m_nIfDecode=0;
            m_nResult = -1;
		}
		virtual ~CUdpBaseCommand(){}

		int Process2()
		{
			return ProcessCmd();
		}

		int Process(int &nResult)
		{
			//解码
			nResult = 0;
		    int nRet=Decode();
            if (nRet == 0)
            {
                nResult =  ProcessCmd();
                m_nResult = nResult;
                if (nResult == 1)
                    m_nResult = 0;
            }
            
            if (nResult <= 1 && m_nIfRes==1) //>0一般是暂时不需要进行处理
            {   
                GenResHead(); //生成返回的头部
                nRet=Encode();
            }			
			return nRet;
		}

		T1 &GetIn()
		{
			return m_in;
		}

		char *GetResBuf(int &nLen)
		{
			nLen = m_outBuf.GetRealBufLen();
			return (char*)m_outBuf.GetBuf();
		}

		void DisAttachBuf()
		{
			m_outBuf.DisAttachBuf();
		}

		short GetMsgType()
			{
			return m_inHead.m_nCmd;
			}
	protected:
        void GenResHead()
        {
            m_outHead.m_nProtocolVersion = m_inHead.m_nProtocolVersion;
            m_outHead.m_nSeqence = m_inHead.m_nSeqence;
            m_outHead.m_nBodylen = 3;
            if (m_nResult == 0)
            {
                m_outHead.m_nBodylen += m_out.GetSize();
            }
        }
		int Encode()
		{
			int iPacketLength = UDT_HEAD_LEN + m_out.GetSize() + 3*sizeof(int) + sizeof(short) + sizeof( char );
			char * pEncodeBuf = new char[iPacketLength];
			memcpy (pEncodeBuf, UDT_HEAD, UDT_HEAD_LEN);
			m_outBuf.AttachBuf( pEncodeBuf , iPacketLength );
			m_outBuf.SetOffset( UDT_HEAD_LEN );

			int ret=0;
			ret+=m_outBuf.PutInt(m_outHead.m_nProtocolVersion);
			ret+=m_outBuf.PutInt(m_outHead.m_nSeqence);
			ret+=m_outBuf.PutInt(m_outHead.m_nBodylen);
			ret+=m_outBuf.PutShort(m_outHead.m_nCmd);
			ret+=m_outBuf.PutChar(m_nResult);
			if (m_nResult == 0)
			{				
				ret += m_out.Encode(m_outBuf);
			}
			return ret;
		}
		int Decode()
		{
			int ret=0;
            if (m_nIfDecode == 0)
                {
			ret+=m_inBuf.GetInt(m_inHead.m_nProtocolVersion);
			ret+=m_inBuf.GetInt(m_inHead.m_nSeqence);
			ret+=m_inBuf.GetInt(m_inHead.m_nBodylen);
			ret+=m_inBuf.GetShort(m_inHead.m_nCmd);
			if (m_inHead.m_nBodylen + 12 != m_inBuf.GetBufLen())
			{
				WriteRunInfo::WriteLog("The body len is invalid, %d != %d",
						m_inHead.m_nBodylen + 12, m_inBuf.GetBufLen());
				return -1;
			}
			ret+=m_in.Decode(m_inBuf);	
            m_nIfDecode = 1;
                }            
			return ret;
		}


		virtual int ProcessCmd() = 0;
		int m_nIfDecode;
		CBaseCommandHead m_inHead;	
		CBaseCommandHead m_outHead;	
		CBufMgr m_inBuf;
		CTmpBufMgr m_outBuf;
		int m_nResult;
		T1 m_in;
		T2 m_out;
};


//NAT Check服务器使用
template <class T1, class T2=CDefaultRes> 
class CNCUdpBaseCommand:public CCommand
{
public:
	CNCUdpBaseCommand(char *pBuf, int nLen):m_inBuf(NULL, 0, 1, 0), m_outBuf(NULL, 0, 1, 0)
	{
		m_inBuf.AttachBuf(pBuf, nLen);
		m_nIfDecode=0;
		m_nResult = -1;
	}
	virtual ~CNCUdpBaseCommand(){}

	int Process2()
	{
		return ProcessCmd();
	}

	int Process(int &nResult)
	{
		//解码
		nResult = 0;
		int nRet=Decode();
		if (nRet == 0)
		{
			nResult = ProcessCmd();
			m_nResult = nResult;
			if (nResult == 1)
				m_nResult = 0;
		}
		
		if (nResult <= 1 && m_nIfRes==1 && 0 == nRet) //>0一般是暂时不需要进行处理
		{   
			nRet=Encode();
		}			
		return nRet;
	}

	T1 &GetIn()
	{
		return m_in;
	}

	char *GetResBuf(int &nLen)
	{
		nLen = m_outBuf.GetRealBufLen();
		return (char*)m_outBuf.GetBuf();
	}

	void DisAttachBuf()
	{
		m_outBuf.DisAttachBuf();
	}

	short GetMsgType()
	{
		return 0;
	}

protected:
	int Encode()
	{
		int iPacketLength = m_out.GetSize();
		char* pEncodeBuf = new char[iPacketLength];
		m_outBuf.AttachBuf (pEncodeBuf, iPacketLength);
		int ret=0;
		ret += m_out.Encode(m_outBuf);
		return ret;
	}
	int Decode()
	{
		int ret=0;
		if (m_nIfDecode == 0)
		{
			ret+=m_in.Decode(m_inBuf);	
			m_nIfDecode = 1;
		}
		return ret;
	}


	virtual int ProcessCmd() = 0;
	int m_nIfDecode;
	CBufMgr m_inBuf;
	CTmpBufMgr m_outBuf;
	int m_nResult;
	T1 m_in;
	T2 m_out;
};
#endif

