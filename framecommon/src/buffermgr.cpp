#include "buffermgr.h"
#include <netinet/in.h>
#include <stdio.h>
namespace MYFRAMECOMMON{
ULONG64 CBaseBufMgr::LBER_HTONL(ULONG64 l)
{
        if (m_nIfNetTrans == 0)
            return l;
        
        ULONG64 a, b;
        sscanf("0x00000000FFFFFFFF", "%llx", &a);
        sscanf("0xFFFFFFFF00000000", "%llx", &b);
        return (((ULONG64)htonl( (l) &  a)) << 32 | htonl( ( (l) & b ) >> 32 ));
}

ULONG64 CBaseBufMgr::LBER_NTOHL(ULONG64 l )
{
        if (m_nIfNetTrans == 0)
            return l;
        
         ULONG64 a, b;
         sscanf("0x00000000FFFFFFFF", "%llx", &a);
         sscanf("0xFFFFFFFF00000000", "%llx", &b);
         return (((ULONG64)ntohl( (l) & a )) << 32 | ntohl( ( (l) & b ) >> 32 ));
}

int CBaseBufMgr::NTOHL(int i)
{    
    return (m_nIfNetTrans==0)?i:ntohl(i);
}

int CBaseBufMgr::HTONL(int i)
{
    return (m_nIfNetTrans==0)?i:htonl(i);
}

short CBaseBufMgr::NTOHS(short s)
{
    return (m_nIfNetTrans==0)?s:ntohs(s);
}

short CBaseBufMgr::HTONS(short s)
{
    return (m_nIfNetTrans==0)?s:htons(s);
}


int CBufMgr::GetInt(int &nInt)
{
        if (m_nOffset + 4 > m_nBufLen)
        {
                return -1;
        }
        memcpy(&nInt, m_pBuf + m_nOffset, 4);
        nInt = NTOHL(nInt);
        m_nOffset += 4;
        return 0;
}

int CBufMgr::GetLong64(LONG64  &lInt)
{        
        if (m_nOffset + 8 > m_nBufLen)
        {
                return -1;
        }

        memcpy(&lInt, m_pBuf + m_nOffset, 8);
        lInt = LBER_NTOHL(lInt);
        m_nOffset += 8;
        return 0;
}


int CBufMgr::GetShort(short &nShort)
{
        if (m_nOffset + 2 > m_nBufLen)
        {
                return -1;
        }

        memcpy(&nShort, m_pBuf + m_nOffset, 2);
        nShort = NTOHS(nShort);
        m_nOffset += 2;
        return 0;
}


int CBufMgr::GetChar(char &cChar)
{
        if (m_nOffset + 1 > m_nBufLen)
        {
                return -1;
        }

        memcpy(&cChar, m_pBuf + m_nOffset, 1);
        m_nOffset += 1;        
        return 0;
}
int CBufMgr::GetString(string &sString)
{
        if (m_nOffset + 4 > m_nBufLen)
        {
                return -1;
        }

        int iLen  = 0;
        memcpy(&iLen,  m_pBuf + m_nOffset, 4);
        m_nOffset += 4;
        iLen = NTOHL(iLen);    
        if (m_nOffset + iLen > m_nBufLen || iLen < 0)
        {
                return -1;
        }

        sString = string(m_pBuf + m_nOffset, iLen);
        m_nOffset += iLen;
        return 0;
}

int CBufMgr::GetString(char *pBuf, int nLen)
{
        if (m_nOffset + 4 > m_nBufLen) 
        {
                return -1;
        }

        int iLen  = 0;
        memcpy(&iLen,  m_pBuf + m_nOffset, 4);
        m_nOffset += 4;
        iLen = NTOHL(iLen);
        int  strLen = (iLen > nLen)?nLen:iLen;
        if (m_nOffset + iLen> m_nBufLen || iLen < 0)
        {
                return -1;
        }
        memcpy(pBuf, m_pBuf + m_nOffset, strLen);
        m_nOffset += iLen;
        return 0;
}

int CBufMgr::GetCharBuf(char *pBuf, int nLen)
{
        if (m_nOffset + nLen > m_nBufLen || nLen < 0)
        {
                return -1;
        }
        memcpy(pBuf, m_pBuf+ m_nOffset, nLen);
        m_nOffset += nLen;
        return 0;
}

int CBufMgr::GetCharBuf(string &sBuf, int nLen)
{
        if (m_nOffset + nLen > m_nBufLen || nLen < 0)
        {
                return -1;
        }
        sBuf.append(m_pBuf+m_nOffset, nLen);
        m_nOffset += nLen;
        return 0;
}

int CBufMgr::PutInt(int nInt)
{
        if (m_nOffset + 4 > m_nBufLen)
        {
                return -1;
        }

        nInt = HTONL(nInt);
        memcpy(m_pBuf + m_nOffset, &nInt, 4);
        m_nOffset += 4;
        return 0;
}

int CBufMgr::PutShort(short nShort)
{
        if (m_nOffset + 2 > m_nBufLen)
        {
                return -1;
        }

        nShort = HTONS(nShort);
        memcpy(m_pBuf + m_nOffset, &nShort, 2);
        m_nOffset += 2;
        return 0;
}



int CBufMgr::PutLong64(LONG64 lInt)
{
        if (m_nOffset + 8 > m_nBufLen)
        {
                return -1;
        }

        lInt = LBER_HTONL(lInt);
        memcpy(m_pBuf + m_nOffset, &lInt, 8);
        m_nOffset += 8;
        return 0;
}


int CBufMgr::PutChar(char cChar)
{
        if (m_nOffset + 1 > m_nBufLen)
        {
                return -1;
        }

        memcpy(m_pBuf + m_nOffset, &cChar, 1);
        m_nOffset += 1;
        return 0;
}

int CBufMgr::PutString(const string &sString)
{
        if (m_nOffset + 4 > m_nBufLen)
        {
                return -1;
        }

        int iLen = HTONL(sString.length());
        memcpy(m_pBuf+m_nOffset, &iLen, 4);
        m_nOffset += 4;

        if (m_nOffset + (int)sString.length() > m_nBufLen)
        {                
                return -1;
        }

        if (sString.length() > 0)
        {
                memcpy(m_pBuf + m_nOffset, sString.c_str(), sString.length());
                m_nOffset += sString.length();
        }
        return 0;
}

int CBufMgr::PutString(const char *pStr)
{
        if (m_nOffset + 4 > m_nBufLen)
        {                
                return -1;
        }
        
        int strLen = strlen(pStr);
        int iLen = HTONL(strLen);
        memcpy(m_pBuf+m_nOffset, &iLen, 4);
        m_nOffset += 4;
        
        if (m_nOffset + strLen > m_nBufLen)
        {                
                return -1;
        }
        
        if (strLen > 0)
        {
                memcpy(m_pBuf + m_nOffset, pStr, strLen);
                m_nOffset += strLen;
        }
        return 0;
}


int CBufMgr::PutCharBuf(const char *pBuf, int nLen)
{
        if (m_nOffset + nLen > m_nBufLen || nLen < 0)
        {                
                return -1;
        }
        memcpy(m_pBuf + m_nOffset, pBuf, nLen);
        m_nOffset += nLen;
        return 0;
}


/**********************************CNewBufMgr***************************************/
CNewBufMgr::CNewBufMgr()
{
        m_nOffset = 0;
        m_nBufLen = 0;
}

CNewBufMgr::CNewBufMgr(const string &sBuf)
{
        m_sBuf = sBuf;
        m_nOffset = 0;
        m_nBufLen = sBuf.length();
}

CNewBufMgr::~CNewBufMgr()
{
}

int CNewBufMgr::GetInt(int &nInt)
{
        if (m_nOffset + 4 > m_nBufLen)
        {
                return -1;
        }
        memcpy(&nInt, m_sBuf.c_str() + m_nOffset, 4);
        nInt = NTOHL(nInt);
        m_nOffset += 4;
        return 0;
}

int CNewBufMgr::GetShort(short &nShort)
{
        if (m_nOffset + 2 > m_nBufLen)
        {
                return -1;
        }

        memcpy(&nShort, m_sBuf.c_str() + m_nOffset, 2);
        nShort = NTOHS(nShort);
        m_nOffset += 2;
        return 0;
}

int CNewBufMgr::GetLong64( LONG64 &lInt)
{        
        if (m_nOffset + 8 > m_nBufLen)
        {
                return -1;
        }

        memcpy(&lInt, m_sBuf.c_str() + m_nOffset, 8);
        lInt = LBER_NTOHL(lInt);
        m_nOffset += 8;
        return 0;
}

int CNewBufMgr::GetChar(char &cChar)
{
        if (m_nOffset + 1 > m_nBufLen)
        {
                return -1;
        }

        memcpy(&cChar, m_sBuf.c_str() + m_nOffset, 1);
        m_nOffset += 1;        
        return 0;
}

int CNewBufMgr::GetString(string &sString)
{
        int nLen = 0;
        int ret = GetInt(nLen);
        if (ret != 0)
        {
                return -1;
        }
              
        if (m_nOffset + nLen > m_nBufLen || nLen < 0)
        {
                return -1;
        }
        sString = m_sBuf.substr(m_nOffset, nLen);
        m_nOffset += nLen;        
        return 0;
}

int CNewBufMgr::GetString(char *pBuf, int nLen)
{
        int iLen = 0;
        int ret = GetInt(iLen);
        if (ret != 0)
        {
                return -1;
        }
              
        if (m_nOffset + iLen > m_nBufLen || iLen < 0)
        {
                return -1;
        }
        int  strLen = (iLen > nLen)?nLen:iLen;
        if (m_nOffset + iLen > m_nBufLen || iLen < 0)
        {
                return -1;
        }
        
        memcpy(pBuf, m_sBuf.c_str() + m_nOffset, strLen);
        m_nOffset += strLen;        
        return 0;
}

int CNewBufMgr::GetCharBuf(char *pBuf, int nLen)
{
        if (m_nOffset + nLen > m_nBufLen || nLen < 0)
        {
                return -1;
        }
        memcpy(pBuf, m_sBuf.c_str() + m_nOffset, nLen);
        m_nOffset += nLen;
        return 0;
}

int CNewBufMgr::PutInt(int nInt)
{
        nInt = HTONL(nInt);
        m_sBuf.append((char*)(&nInt), 4);
        return 0;
}

int CNewBufMgr::PutShort(short nShort)
{
        nShort = HTONS(nShort);
        m_sBuf.append((char*)(&nShort), 2);
        return 0;
}

int CNewBufMgr::PutLong64( LONG64  lLong64)
{
        lLong64 = LBER_HTONL(lLong64);
        m_sBuf.append((char*)(&lLong64), 8);
        return 0;
}

int CNewBufMgr::PutChar(char cChar)
{
        m_sBuf.append(1, cChar);
        return 0;
}

int CNewBufMgr::PutString(const string &sString)
{
        PutInt(sString.length());
        m_sBuf.append(sString);
        return 0;
}

int CNewBufMgr::PutCharBuf(const char *pBuf, int nLen)
{
        m_sBuf.append(pBuf, nLen);
        return 0;
}

int CNewBufMgr::PutString(const char *pStr)
{
        int sLen = strlen(pStr);
        PutInt(sLen);
        m_sBuf.append(pStr, sLen);
        return 0;
}
}
