// encryption_algorithm.h: interface for the encryption_algorithm class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ENCRYPTION_ALGORITHM_H__D7B1F5C8_E8AC_46A6_9483_8978C002BEB6__INCLUDED_)
#define AFX_ENCRYPTION_ALGORITHM_H__D7B1F5C8_E8AC_46A6_9483_8978C002BEB6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class encryption_algorithm  
{
public:
	encryption_algorithm();
	virtual ~encryption_algorithm();

	/**
	@purpose    : 创建密钥
	@param	data: 用于创建密钥的数据
	@param	len : 用于创建密钥的数据的长度
	@param  key	: 返回密钥
	@param  key_len : 返回密钥长度
	@return		: 是否创建成功
	*/
	virtual bool create_key( const unsigned char *data, unsigned len, void *key, unsigned &key_len ) = 0;
	
	/**
	@purpose			: 加密需要发送的数据 
	@param	io_buffer	: 数据
	@param  len			: 数据的长度
	@return				: 返回结果值
	*/
	virtual bool encrypt( unsigned char *io_buffer, unsigned len ) = 0;

	/**
	@purpose			: 将收到的数据解密
	@param	io_buffer	: 数据
	@param  len			: 数据的长度
	@return				: 返回结果值
	*/
	virtual bool decrypt( unsigned char *io_buffer, unsigned len ) =0;
};

#endif // !defined(AFX_ENCRYPTION_ALGORITHM_H__D7B1F5C8_E8AC_46A6_9483_8978C002BEB6__INCLUDED_)
