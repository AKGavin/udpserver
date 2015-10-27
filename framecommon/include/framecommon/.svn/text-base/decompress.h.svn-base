/****************************************************************************
**
**  Copyright  (c)  2008-2009,  Baofeng, Inc.  All Rights Reserved.
**
**  Subsystem:    Packet Decoder
**  File:         Decompress.h
**  Created By:   hanzhiliang
**  Created On:   2009/07/22
**
**  Purpose:
**     BufferHelper
**
**  History:
**  Programmer      Date         Version        Number      Description
**  --------------- --------   ---------- ------------------------------
**  hanzhiliang    2009/07/22     1.0            01          creation
****************************************************************************/
#ifndef __DECOMPRESS_H
#define __DECOMPRESS_H

#include <vector>
#include "zlib.h"


class CDecompress
{
public:
	CDecompress()
	{
		
	}
	
	CDecompress(const char *pBuf, int &nBufSize)
	{
		m_pBuf = (char *)pBuf;
		m_nBufSize = nBufSize;
	}
	
	int decompress()
	{
		return decompress(m_pBuf, m_nBufSize);
	}

	int decompress(const char *pBuf, int &nBufSize)
	{
	    if(NULL == pBuf || nBufSize < 0)
	    {
	    	return -1;
	    }
	    
	    const unsigned int uncompress_ctl_len = 12;  //zlib Ô¤Áô³¤¶È	    
	    unsigned int retry_times = 0;
	    const unsigned int max_retry = 4;
	    unsigned long temp_len = 0;
		std::vector<char> temp_buff;

	    while( retry_times < max_retry )
	    {
	        temp_len = ( nBufSize + uncompress_ctl_len )<<( 1 + retry_times);
			temp_buff.resize(temp_len);
	        int ret = uncompress((unsigned char *)&temp_buff[0],&temp_len,(const unsigned char *)pBuf,nBufSize);
	        if( ret == Z_OK )
	        {
				m_Decoder.resize(temp_len);
				m_Decoder.assign(temp_buff.begin(), temp_buff.begin()+temp_len);
	            return 0;
	        }
	        else if( ret == Z_BUF_ERROR )
	        {
	            retry_times++;
	            continue;
	        }
	        else
	        {
	           return -1;
	        }
	    }	    
	    
	    return 0;
	}
	static int decompress(const char *pBuf, int &nBufSize, char *pDecompressbuf, unsigned long &nDecompressLen)
	{
		int ret = uncompress((unsigned char *)pDecompressbuf,&nDecompressLen,(const unsigned char *)pBuf,nBufSize);
		if(ret == Z_OK)
			return 0;
			
		return -1;
	}

	std::vector<char>& Get()
	{
		return m_Decoder;
	}
protected:

private:
	char *m_pBuf;
	int m_nBufSize;

	std::vector<char> m_Decoder;
};

#endif

