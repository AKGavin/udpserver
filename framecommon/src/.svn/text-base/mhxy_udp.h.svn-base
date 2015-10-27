// mhxy_udp.h: interface for the mhxy_udp class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MHXY_UDP_H__5FEDF880_201D_4217_89B2_7D430E8942A2__INCLUDED_)
#define AFX_MHXY_UDP_H__5FEDF880_201D_4217_89B2_7D430E8942A2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define MHXY_ENCRYPT_EXTRA_LEN 20

class mhxy_udp  
{
public:
	mhxy_udp();
	virtual ~mhxy_udp();

	// 第一套接口

	/**
	@purpose			: 加密数据 
	@param	type		: 加密方法
	@param	in_buffer	: 需加密的数据
	@param	in_len		: 需加密的数据长度
	@param	out_buffer	: 加密后的数据
	@param	out_len		: 加密后的数据长度
	@return				: 返回结果值
	*/
	static void encrypt(int type, unsigned char *in_buffer, unsigned in_len, unsigned char *out_buffer, unsigned &out_len);

	/**
	@purpose			: 解密数据 
	@param	in_buffer	: 需解密的数据
	@param	in_len		: 需解密的数据长度
	@param	out_buffer	: 解密后的数据
	@param	out_len		: 解密后的数据长度
	@return				: 返回加密方法
	*/
	static int decrypt(unsigned char *in_buffer, unsigned in_len, unsigned char *out_buffer, unsigned &out_len);

	// 第二套接口
	
	/**
	@purpose			: 加密数据 
	@param	type		: 加密方法
	@param	io_buffer	: 加密前后的数据
	@param	io_len		: 加密前后的数据长度
	@return				: 返回结果值
	*/
	static void encrypt(int type, unsigned char *io_buffer, unsigned &io_len);

	/**
	@purpose			: 解密数据 
	@param	io_buffer	: 解密前后的数据
	@param	io_len		: 解密前后的数据长度
	@return				: 返回加密方法
	*/
	static int decrypt(unsigned char *io_buffer, unsigned &io_len);
};

#endif // !defined(AFX_MHXY_UDP_H__5FEDF880_201D_4217_89B2_7D430E8942A2__INCLUDED_)
