#ifndef __BUFF_MGR_H
#define __BUFF_MGR_H
#include <cstring>
#include "commondef.h"
#include <arpa/inet.h>
using namespace std;

namespace MYFRAMECOMMON{

const int INTSIZE=sizeof(int);    
const int SHORTSIZE=sizeof(short);
const int INT64SIZE=sizeof(long long);
const int CHARSIZE=sizeof(char);
class CBaseBufMgr
{
public:
        CBaseBufMgr()
        {
              m_nIfNetTrans=1;
        }
        ~CBaseBufMgr()
        {
        }
        void SetIfNetTrans(int nIfNetTranse)
        {
                m_nIfNetTrans = nIfNetTranse;
        }
protected:
        ULONG64 LBER_HTONL(ULONG64 l ); 
        ULONG64 LBER_NTOHL(ULONG64 l );
        int NTOHL(int i);
        int HTONL(int i);
        short NTOHS(short s);
        short HTONS(short s);
        int m_nIfNetTrans;
};


class CBufMgr:public CBaseBufMgr
{       
public:
        CBufMgr(char *pBuf, int nBufLen=0, int nIfDel=1, int nIfNetTrans=1)
        {
                m_pBuf = pBuf;
                m_nBufLen = nBufLen;
                m_nOffset = 0;
                m_nIfDel = nIfDel;
                m_nIfNetTrans = nIfNetTrans;
        }
        CBufMgr()
        {
                m_pBuf = NULL;
                m_nBufLen = 0;
                m_nOffset = 0;
                m_nIfDel = 1;
        }
        ~CBufMgr()
        {
                if (m_pBuf != NULL && m_nIfDel == 1)
                {
                        delete [] m_pBuf;
                }
        }

        int AttachBuf(char *pBuf, int nBuffLen)
        {
                m_pBuf = pBuf;
                m_nBufLen = nBuffLen;
                m_nOffset = 0;
                return 0;
        }

        int DistachBuf()
        {
                m_pBuf = NULL;
                m_nBufLen = 0;
                m_nOffset = 0;
                return 0;
        }

        char *GetBuf()
        {
                return m_pBuf;
        }
        int GetBufLen()
        {
                return m_nBufLen;
        }

        int GetRealBufLen()
        {
                return m_nOffset;
        }
        
        int GetInt(int &nInt);
        int GetShort(short &nShort);
        int GetLong64(LONG64 &lInt);
        int GetChar(char &cChar);
        int GetString(string &sString);        
        int GetString(char *pBuf, int nLen);
        int GetCharBuf(char *pBuf, int nLen);
        int GetCharBuf(string &sBuf, int nLen);

        int PutInt(int nInt);
        int PutShort(short nShort);
        int PutLong64(LONG64 lLong64);
        int PutChar(char cChar);
        int PutString(const string &sString);
        int PutCharBuf(const char *pBuf, int nLen);
        void SetOffset(int nOffset)
        {
                m_nOffset = nOffset;              
        }
        int PutString(const char *pStr);

        int GetLeftLen()
        {
                return m_nBufLen - m_nOffset;
        }
protected:        
        char*  m_pBuf;
        int  m_nBufLen;
        int  m_nOffset;
        int m_nIfDel;
};

class CNewBufMgr:public CBaseBufMgr
{
public:
        CNewBufMgr();
        CNewBufMgr(const string &sBuf);
        ~CNewBufMgr();
        int GetInt(int &nInt);
        int GetShort(short &nShort);
        int GetLong64(LONG64 &lInt);
        int GetChar(char &cChar);
        int GetString(string &sString);        
        int GetString(char *pBuf, int nLen);
        int GetCharBuf(char *pBuf, int nLen);
        
        int PutInt(int nInt);
        int PutShort(short nShort);
        int PutLong64(LONG64 lLong64);
        int PutChar(char cChar);
        int PutString(const string &sString);
        int PutCharBuf(const char *pBuf, int nLen);     
        int PutString(const char *pStr);
        string &GetBuf()
        {
            return m_sBuf;
        }
protected:
        string m_sBuf;
        int m_nOffset;
        int m_nBufLen;
};
/**
* dec:
* ClassName:CTmpBufMgr
* author: 
* version:
*CreateTime: 2009.3.8
*/
class CTmpBufMgr:public CBaseBufMgr
{
public:
    CTmpBufMgr(char *pBuf, int nLen, int nIfDel, int nIfNetTrans=1)
        {
			m_pBuf=pBuf;
			m_nBufLen = nLen;
			m_Offset=0;
			m_nIfDel=nIfDel;
			m_nIfNetTrans = nIfNetTrans;
        }
    CTmpBufMgr()
        {
			m_pBuf=NULL;
			m_nBufLen = 0;
			m_Offset=0;
			m_nIfDel=1;
			m_nIfNetTrans=0;
        }
    ~CTmpBufMgr()
    {
        if (m_nIfDel == 1 && m_pBuf != NULL)
            delete[] m_pBuf;
    }

    void AttachBuf(char *pBuf, int nLen)
    {
        m_pBuf = pBuf;
        m_nBufLen = nLen;
        m_Offset = 0;
    }

    void DisAttachBuf()
    {
        m_pBuf = NULL;
        m_nBufLen = 0;
        m_Offset = 0;
    }

	char *GetBuf()
	{
		return m_pBuf;
	}

	int GetRealBufLen()
	{
		return m_Offset;
	}

	int GetBufLen()
	{
		return m_nBufLen;
	}

	void SetOffset( int nOffset)
	{
		m_Offset = nOffset;
	}

    template<class T1>
    int Get(T1 &t1)
    {
        if (m_nBufLen - m_Offset < (int)sizeof(T1))
            return -1;
        memcpy(&t1, m_pBuf + m_Offset, sizeof(T1));
        t1=NTOH(t1);
        m_Offset += sizeof(T1);
        return 0;
    }
    
    int Get(string &str)
    {
        int nLen=0;
        int nRet = Get(nLen);
        if (nRet != 0)
            return -1;
        if (nLen > m_nBufLen - m_Offset || nLen < 0)  //modified by dijie 09-11-20
        {
            return -1;
        }
        str.assign(m_pBuf+m_Offset, nLen);
        m_Offset += nLen;
        return 0;
    }
    
    template<class T1>
    int Put(const T1 &t1)
    {
        if (m_Offset + (int)sizeof(T1) > m_nBufLen)
            return -1;
        T1 tmp = HTON(t1);
        memcpy(m_pBuf+m_Offset, (char*)&tmp, sizeof(T1));
        m_Offset += sizeof(T1);
        return 0;
    }

    int Put(const char *str, int nLen)
    {
        if (m_Offset + (int)sizeof(int) + nLen > m_nBufLen || nLen < 0)  //modified by dijie 09-11-20
            return -1;
        
        int nRet=Put(nLen);
        if (nRet != 0)
            return -1;
        memcpy(m_pBuf + m_Offset, str, nLen);
        m_Offset +=  nLen;
        return 0;
    }
    
    int Put(const char *str)
    {
        int len = strlen(str);
        return Put(str, len);
    }
    
    int Put(const string &str)
    {
        return Put(str.c_str(), str.length());
    }
    
    int PutCharBuf(const char *pBuf, int nLen)
    {
    	  if (m_Offset +  nLen > m_nBufLen || nLen < 0)  //modified by dijie 09-11-20
    	  	  return -1;
    	  memcpy(m_pBuf+m_Offset, pBuf, nLen);
    	  m_Offset += nLen;
    	  return 0;
    }


   

    int GetInt(int &nInt)
    {
		return Get<int>(nInt);
    }

    int GetShort(short &nShort)
    {
		return Get<short>(nShort);
    }

    int GetLong64(LONG64 &lInt)
    {
		return Get<LONG64>(lInt);
    }

    int GetChar(char &cChar)
    {
		return Get<char>(cChar);
    }

    int GetCharBuf(char *pBuf, int nLen)
    {
		if (m_Offset + nLen > m_nBufLen || nLen < 0)  //modified by dijie 09-11-20
		{
			return -1;
		}
		memcpy(pBuf, m_pBuf + m_Offset, nLen);
		m_Offset += nLen;
		return	0;
    }

    int GetString(std::string &sString)
    {
	    return Get(sString);
    }

    int PutInt(int nInt)
    {
		return Put<int>(nInt);
    }

    int PutShort(short nShort)
    {
		return Put<short>(nShort);
    }

    int PutLONG64(LONG64 lLong64)
    {
		return Put<LONG64>(lLong64);
    }

    int PutChar(char cChar)
    {
		return Put<char>(cChar);
    }

    int PutString(const string &sString)
    {
		 return Put(sString);
    }

protected:
    template<class T1>
    T1 HTON(const T1 &t1)
    {        
        if (m_nIfNetTrans==0)
            return t1;
        return REALHTON(t1);
    }
    template<class T1>
    T1 REALHTON(const T1 &t1)
    {
        return t1;
    }
    int REALHTON(const int &t1){ return htonl(t1); }
    short REALHTON(const short &t1){ return htons(t1); }
    ULONG64 REALHTON(const LONG64 &t1)
    {
        ULONG64 a, b;
        a=0x00000000FFFFFFFFll;
        b=0xFFFFFFFF00000000ll;
        return (((ULONG64)htonl( (t1) &  a)) << 32 | htonl( ( (t1) & b ) >> 32 ));
    }

    template<class T1>
    T1 NTOH(const T1 &t1)
    {        
        if (m_nIfNetTrans==0)
            return t1;
        return REALNTOH(t1);
    }
    template<class T1>
    T1 REALNTOH(const T1 &t1)
    {
        return t1;
    }
    int REALNTOH(const int &t1){ return ntohl(t1); }
    short REALNTOH(const short &t1){ return ntohs(t1); }
    ULONG64 REALNTOH(const LONG64 &t1)
        { 
        ULONG64 a, b;
        a=0x00000000FFFFFFFFll;
        b=0xFFFFFFFF00000000ll;
        return (((ULONG64)ntohl( (t1) &  a)) << 32 | ntohl( ( (t1) & b ) >> 32 ));

        }
private:
    char* m_pBuf;
    int m_nBufLen;
    int m_Offset;
    int m_nIfDel;
};
}
#endif

