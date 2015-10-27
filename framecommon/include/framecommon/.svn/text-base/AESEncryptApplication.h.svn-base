/*
* =======================================================================
* Copyright (c) 2009-
* All rights reserved 
* -----------------------------------------------------------------------
* Version		: 1.0
* FileName		: AESEncryptApplication.h
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
#ifndef _AESENCRYPTAPPLICATION_H_F2ECAF4E_B149_46f7_B178_836265395589_
#define _AESENCRYPTAPPLICATION_H_F2ECAF4E_B149_46f7_B178_836265395589_

#include <openssl/aes.h>
#include <openssl/md5.h>

#define ENCRYPT_BLOCK_SIZE 16


class openssl_md5
{
public:
	openssl_md5();
public:
	void update(const unsigned char *pdata, unsigned long count);
	void finish(unsigned char cid[16]);

protected:
	void initialize(void);
private:
	MD5_CTX m_MD5_CTX;
};

class openssl_aes
{
public:
	openssl_aes(int keySize, unsigned char *keyBytes);
public:
	void Cipher(unsigned char input[16], unsigned char output[16]); 
	void InvCipher(unsigned char input[16], unsigned char output[16]);
private:
	AES_KEY m_EncryptKey;
	AES_KEY m_DecryptKey;
	char m_aesKey[32];
	int m_keySize;
};

class CAESEncryptApplication
{
public:
	static bool AESEncrypt(char*, unsigned int&);
	static bool AESEncryptEx(char*, unsigned int&);  //modified by dijie 09-11-16
	static bool AESDncrypt(char*, unsigned int&);
	static bool AESDncryptEx(char*, unsigned int&);  //modified by dijie 09-11-16
};

#endif



