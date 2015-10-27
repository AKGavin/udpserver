/*
* =======================================================================
* Copyright (c) 2009-
* All rights reserved 
* -----------------------------------------------------------------------
* Version		: 1.0
* FileName		: AESEncryptApplication.cpp
* Description	: 
* -----------------------------------------------------------------------
* Author        : wangtiefeng (wtf), wangtf418@163.com
* Company		: 
* Date			: 2009-07-20
* -----------------------------------------------------------------------
* History		: 
*		<author>		  <time>		<version>		<description>
*      ---------------------------------------------------------------
*		wangtiefeng		2009-07-20			1.0			
* =======================================================================
*/
#include <cstring>
#include "AESEncryptApplication.h"
#include<netinet/in.h>

using namespace std;

//openssl_md5================================================================================
openssl_md5::openssl_md5()
{
	initialize();
}

void openssl_md5::initialize()
{
	//m_MD5_CTX.A = 0x67452301;
	//m_MD5_CTX.B = 0xefcdab89;
	//m_MD5_CTX.C = 0x98badcfe;
	//m_MD5_CTX.D = 0x10325476;
	//m_MD5_CTX.Nl = 0;
	//m_MD5_CTX.Nh = 0;
	//memset(m_MD5_CTX.data, 0 , MD5_LBLOCK);
	MD5_Init(&m_MD5_CTX);
}

void openssl_md5::update(const unsigned char *pdata, unsigned long count)
{
	MD5_Update(&m_MD5_CTX, pdata, count);
}

void openssl_md5::finish(unsigned char cid[16])
{
	MD5_Final(cid, &m_MD5_CTX);
}
//openssl_aes================================================================================
openssl_aes::openssl_aes(int keySize, unsigned char *keyBytes)
{
	m_keySize = keySize * 8;
	memcpy(m_aesKey, keyBytes, keySize);
}

void openssl_aes::Cipher(unsigned char input[16], unsigned char output[16])
{
	AES_set_encrypt_key((unsigned char*)m_aesKey, m_keySize, &m_EncryptKey);
	AES_encrypt(input, output, &m_EncryptKey);
}

void openssl_aes::InvCipher(unsigned char input[16], unsigned char output[16])
{
	AES_set_decrypt_key((unsigned char*)m_aesKey,  m_keySize, &m_DecryptKey);
	AES_decrypt(input, output, &m_DecryptKey);
}
//CAESEncryptApplication====================================================================

bool CAESEncryptApplication::AESEncrypt(char *pDataBuff,unsigned int &nBuffLen)
{
	if (pDataBuff == NULL)
	{
		return false;
	}

	char *pOutBuff = new char[nBuffLen + 16];
	int  nOutLen = 0;
	int nBeginOffset = sizeof(unsigned int)*3;

	bool bReturn = false;
	try
	{
		unsigned char szKey[16];
		openssl_md5 rMd5;

		rMd5.update((unsigned char*)pDataBuff,sizeof(unsigned int)*2);
		rMd5.finish(szKey);


		openssl_aes kaes(16,(unsigned char*)szKey);

		int nInOffset = nBeginOffset; //不是从头开始加密
		int nOutOffset = 0;

		unsigned char inBuff[ENCRYPT_BLOCK_SIZE],ouBuff[ENCRYPT_BLOCK_SIZE];
		memset(inBuff,0,ENCRYPT_BLOCK_SIZE);
		memset(ouBuff,0,ENCRYPT_BLOCK_SIZE);

		while(true)
		{
			if (nBuffLen - nInOffset >= ENCRYPT_BLOCK_SIZE)
			{
				memcpy(inBuff,pDataBuff+nInOffset,ENCRYPT_BLOCK_SIZE);
				kaes.Cipher(inBuff,ouBuff);

				memcpy(pOutBuff+nOutOffset,ouBuff,ENCRYPT_BLOCK_SIZE);

				nInOffset += ENCRYPT_BLOCK_SIZE;
				nOutOffset += ENCRYPT_BLOCK_SIZE;
			}
			else
			{
				int nDataLen = nBuffLen - nInOffset;
				int nFillData = ENCRYPT_BLOCK_SIZE - nDataLen;

				memset(inBuff,nFillData,ENCRYPT_BLOCK_SIZE);
				memset(ouBuff,0,ENCRYPT_BLOCK_SIZE);

				if (nDataLen > 0)
				{
					memcpy(inBuff,pDataBuff+nInOffset,nDataLen);
					kaes.Cipher(inBuff,ouBuff);

					memcpy(pOutBuff+nOutOffset,ouBuff,ENCRYPT_BLOCK_SIZE);

					nInOffset += nDataLen;
					nOutOffset += ENCRYPT_BLOCK_SIZE;
				}
				else
				{
					kaes.Cipher(inBuff,ouBuff);

					memcpy(pOutBuff+nOutOffset,ouBuff,ENCRYPT_BLOCK_SIZE);

					nOutOffset += ENCRYPT_BLOCK_SIZE;
				}

				break;
			}
		}
		nOutLen = nOutOffset;

		memcpy(pDataBuff + nBeginOffset, pOutBuff, nOutLen);

		memcpy(pDataBuff + sizeof(unsigned int)*2, &nOutLen, sizeof(unsigned int));

		nBuffLen = nOutLen + nBeginOffset;

		bReturn = true;
	}
	catch (...)
	{
		bReturn = false;
	}

	delete[] pOutBuff;

	return bReturn!=0;
}

bool CAESEncryptApplication::AESEncryptEx(char *pDataBuff,unsigned int &nBuffLen)
{
	if (pDataBuff == NULL)
	{
		return false;
	}

	char *pOutBuff = new char[nBuffLen + 16];
	int  nOutLen = 0;
	int nBeginOffset = sizeof(unsigned int)*3;

	bool bReturn = false;
	try
	{
		unsigned char szKey[16];
		openssl_md5 rMd5;

		unsigned int ver, sn, key;
		memcpy(&ver, pDataBuff, sizeof(ver));
		ver = ntohl(ver);
		memcpy(&sn, pDataBuff+sizeof(ver), sizeof(sn));
		sn = ntohl(sn);
		key = sn + ver + 20090504;

		//rMd5.update((unsigned char*)pDataBuff,sizeof(unsigned int)*2);
		rMd5.update((unsigned char*)&key, sizeof(key));
		rMd5.finish(szKey);


		openssl_aes kaes(16,(unsigned char*)szKey);

		int nInOffset = nBeginOffset; //不是从头开始加密
		int nOutOffset = 0;

		unsigned char inBuff[ENCRYPT_BLOCK_SIZE],ouBuff[ENCRYPT_BLOCK_SIZE];
		memset(inBuff,0,ENCRYPT_BLOCK_SIZE);
		memset(ouBuff,0,ENCRYPT_BLOCK_SIZE);

		while(true)
		{
			if (nBuffLen - nInOffset >= ENCRYPT_BLOCK_SIZE)
			{
				memcpy(inBuff,pDataBuff+nInOffset,ENCRYPT_BLOCK_SIZE);
				kaes.Cipher(inBuff,ouBuff);

				memcpy(pOutBuff+nOutOffset,ouBuff,ENCRYPT_BLOCK_SIZE);

				nInOffset += ENCRYPT_BLOCK_SIZE;
				nOutOffset += ENCRYPT_BLOCK_SIZE;
			}
			else
			{
				int nDataLen = nBuffLen - nInOffset;
				int nFillData = ENCRYPT_BLOCK_SIZE - nDataLen;

				memset(inBuff,nFillData,ENCRYPT_BLOCK_SIZE);
				memset(ouBuff,0,ENCRYPT_BLOCK_SIZE);

				if (nDataLen > 0)
				{
					memcpy(inBuff,pDataBuff+nInOffset,nDataLen);
					kaes.Cipher(inBuff,ouBuff);

					memcpy(pOutBuff+nOutOffset,ouBuff,ENCRYPT_BLOCK_SIZE);

					nInOffset += nDataLen;
					nOutOffset += ENCRYPT_BLOCK_SIZE;
				}
				else
				{
					kaes.Cipher(inBuff,ouBuff);

					memcpy(pOutBuff+nOutOffset,ouBuff,ENCRYPT_BLOCK_SIZE);

					nOutOffset += ENCRYPT_BLOCK_SIZE;
				}

				break;
			}
		}
		nOutLen = nOutOffset;

		memcpy(pDataBuff + nBeginOffset, pOutBuff, nOutLen);

		int nOutLenTmp;
		nOutLenTmp = htonl(nOutLen);

		memcpy(pDataBuff + sizeof(unsigned int)*2, &nOutLenTmp, sizeof(unsigned int));

		nBuffLen = nOutLen + nBeginOffset;

		bReturn = true;
	}
	catch (...)
	{
		bReturn = false;
	}

	delete[] pOutBuff;

	return bReturn!=0;
}

bool CAESEncryptApplication::AESDncrypt(char* pDataBuff,unsigned int& nBuffLen)
{
	if (pDataBuff == NULL)
	{
		return false;
	}

	int nBeginOffset = sizeof(unsigned int)*3;

	if ((nBuffLen-nBeginOffset)%ENCRYPT_BLOCK_SIZE != 0)
	{
		return false;
	}

	char *pOutBuff = new char[nBuffLen + 16];
	int  nOutLen = 0;


	bool bReturn = false;
	try
	{
		unsigned char szKey[16];
		openssl_md5 rMd5;

		rMd5.update((unsigned char*)pDataBuff,sizeof(unsigned int)*2);
		rMd5.finish(szKey);

		openssl_aes kaes(16,(unsigned char*)szKey);

		int nInOffset = nBeginOffset;
		int nOutOffset = 0;

		unsigned char inBuff[ENCRYPT_BLOCK_SIZE],ouBuff[ENCRYPT_BLOCK_SIZE];
		memset(inBuff,0,ENCRYPT_BLOCK_SIZE);
		memset(ouBuff,0,ENCRYPT_BLOCK_SIZE);

		while(nBuffLen - nInOffset > 0)
		{
			memcpy(inBuff,pDataBuff+nInOffset,ENCRYPT_BLOCK_SIZE);
			kaes.InvCipher(inBuff,ouBuff);

			memcpy(pOutBuff+nOutOffset,ouBuff,ENCRYPT_BLOCK_SIZE);

			nInOffset += ENCRYPT_BLOCK_SIZE;
			nOutOffset += ENCRYPT_BLOCK_SIZE;
		}
		nOutLen = nOutOffset;
		memcpy(pDataBuff + nBeginOffset,pOutBuff,nOutLen);
		char * out_ptr = pOutBuff + nOutLen - 1;
		if (*out_ptr <= 0 || *out_ptr > ENCRYPT_BLOCK_SIZE)
		{
			bReturn = false;
		}
		else
		{
			nBuffLen = nBeginOffset + nOutLen - *out_ptr;
			*(int *)(pDataBuff + 8) = nBuffLen - 12;
			bReturn = true;
		}
	}
	catch (...)
	{
		bReturn = false;
	}
	delete[] pOutBuff;
	return bReturn;
}

bool CAESEncryptApplication::AESDncryptEx(char* pDataBuff,unsigned int& nBuffLen)
{
	if (pDataBuff == NULL)
	{
		return false;
	}

	int nBeginOffset = sizeof(unsigned int)*3;

	if ((nBuffLen-nBeginOffset)%ENCRYPT_BLOCK_SIZE != 0)
	{
		return false;
	}

	char *pOutBuff = new char[nBuffLen + 16];
	int  nOutLen = 0;


	bool bReturn = false;
	try
	{
		unsigned char szKey[16];
		openssl_md5 rMd5;

		unsigned int ver, sn, key;
		memcpy(&ver, pDataBuff, sizeof(ver));
		ver = ntohl(ver);
		memcpy(&sn, pDataBuff+sizeof(ver), sizeof(sn));
		sn = ntohl(sn);
		key = sn + ver + 20090504;

		rMd5.update((unsigned char*)&key, sizeof(key));
		//rMd5.update((unsigned char*)pDataBuff,sizeof(unsigned int)*2);
		rMd5.finish(szKey);

		openssl_aes kaes(16,(unsigned char*)szKey);

		int nInOffset = nBeginOffset;
		int nOutOffset = 0;

		unsigned char inBuff[ENCRYPT_BLOCK_SIZE],ouBuff[ENCRYPT_BLOCK_SIZE];
		memset(inBuff,0,ENCRYPT_BLOCK_SIZE);
		memset(ouBuff,0,ENCRYPT_BLOCK_SIZE);

		while(nBuffLen - nInOffset > 0)
		{
			memcpy(inBuff,pDataBuff+nInOffset,ENCRYPT_BLOCK_SIZE);
			kaes.InvCipher(inBuff,ouBuff);

			memcpy(pOutBuff+nOutOffset,ouBuff,ENCRYPT_BLOCK_SIZE);

			nInOffset += ENCRYPT_BLOCK_SIZE;
			nOutOffset += ENCRYPT_BLOCK_SIZE;
		}
		nOutLen = nOutOffset;
		memcpy(pDataBuff + nBeginOffset,pOutBuff,nOutLen);
		char * out_ptr = pOutBuff + nOutLen - 1;
		if (*out_ptr <= 0 || *out_ptr > ENCRYPT_BLOCK_SIZE)
		{
			bReturn = false;
		}
		else
		{
			nBuffLen = nBeginOffset + nOutLen - *out_ptr;
			*(int *)(pDataBuff + 8) = htonl(nBuffLen - 12);
			bReturn = true;
		}
	}
	catch (...)
	{
		bReturn = false;
	}
	delete[] pOutBuff;
	return bReturn;
}


