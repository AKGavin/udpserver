// encryption_algorithm_2.cpp: implementation of the encryption_algorithm_2 class.
//
//////////////////////////////////////////////////////////////////////

#include "encryption_algorithm_2.h"
#include <stdlib.h>
#include <memory.h>
#include <assert.h>
#include<netinet/in.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

bool encryption_algorithm_2::create_key( const unsigned char *data, unsigned len, void *key, unsigned &key_len )
{
	// 创建首部
	if (len == 0)
	{
		_key_len = 0;

		// 加密方法
		unsigned head = ((8192*2 + rand() % 8192) << 16) + rand() % 65536; // [0x40000000, 0x5FFFFFFF]
		head = htonl( head);
		memcpy(_key, &head, 4);
		_key_len += 4;

		// 随机长度
		unsigned char rand_len = rand();
		memcpy(_key+_key_len, &rand_len, 1);
		_key_len += 1;
		rand_len = rand_len %2 + 2;

		// 填充字符
		unsigned char padding;
		for (unsigned char i=0; i<rand_len; i++)
		{
			padding = rand();
			memcpy(_key+_key_len, &padding, 1);
			_key_len += 1;
		}

		// 校验字符
		padding = (_key[_key_len-1]*9 ^ ((_key_len+2)*30));
		memcpy(_key+_key_len, &padding, 1);
		_key_len += 1;
		padding = (_key[_key_len-1]*9 ^ ((_key_len+2)*30));
		memcpy(_key+_key_len, &padding, 1);
		_key_len += 1;

		// 拷贝首部
		assert(key != NULL);
		memcpy(key, _key, _key_len);
		key_len = _key_len;

		_key_pos = 0;
		return true;
	}
	// 解析首部
	else
	{
		if (len < 5)
		{
			return false;
		}

		// 首部长度
		_key_len = data[4] % 2 + 9;
		if (len < _key_len)
		{
			return false;
		}

		key_len = _key_len;
		memcpy(_key, data, _key_len);

		// 检查校验字符
		unsigned char padding;
		padding = (_key[_key_len-2]*9 ^ ((_key_len+1)*30));
		if (_key[_key_len-1] != padding)
		{
			return false;
		}
		padding = (_key[_key_len-3]*9 ^ ((_key_len+0)*30));
		if (_key[_key_len-2] != padding)
		{
			return false;
		}

		_key_pos = 0;
		return true;
	}
}

bool encryption_algorithm_2::encrypt( unsigned char *in_buffer, unsigned len )
{
	unsigned next;
	for (unsigned i=0; i<len; i++)
	{
		next = _key_pos + 1;
		if (next == _key_len)
		{
			next = 0;
		}
		_key[_key_pos] = _key[_key_pos] ^ (_key[next]+30*9);
		in_buffer[i] = in_buffer[i] + _key[_key_pos];
		_key_pos = next;
	}
	return true;
}

bool encryption_algorithm_2::decrypt( unsigned char *in_buffer, unsigned len )
{
	unsigned next;
	for (unsigned i=0; i<len; i++)
	{
		next = _key_pos + 1;
		if (next == _key_len)
		{
			next = 0;
		}
		_key[_key_pos] = _key[_key_pos] ^ (_key[next]+30*9);
		in_buffer[i] = in_buffer[i] - _key[_key_pos];
		_key_pos = next;
	}
	return true;
}
