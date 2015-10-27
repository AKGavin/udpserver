// mhxy_udp.cpp: implementation of the mhxy_udp class.
//
//////////////////////////////////////////////////////////////////////

#include "mhxy_udp.h"
#include "encryption_algorithm.h"
#include "encryption_algorithm_1.h"
#include "encryption_algorithm_2.h"
#include "encryption_algorithm_3.h"
#include <memory.h>
#include <assert.h>
#include<netinet/in.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

mhxy_udp::mhxy_udp()
{

}

mhxy_udp::~mhxy_udp()
{

}

void mhxy_udp::encrypt(int type, unsigned char *in_buffer, unsigned in_len, unsigned char *out_buffer, unsigned &out_len)
{
	assert((in_buffer!=0)&&(out_buffer!=0));
	unsigned key_len;
	switch (type)
	{
	case 0:
		memcpy(out_buffer, in_buffer, in_len);
		out_len = in_len;
		break;

	case 1:
		{
			encryption_algorithm_1 ea;
			ea.create_key(0, 0, out_buffer, key_len);
			memcpy(out_buffer+key_len, in_buffer, in_len);
			ea.encrypt(out_buffer+key_len, in_len);
			out_len = key_len + in_len;
		}
		break;

	case 2:
		{
			encryption_algorithm_2 ea;
			ea.create_key(0, 0, out_buffer, key_len);
			memcpy(out_buffer+key_len, in_buffer, in_len);
			ea.encrypt(out_buffer+key_len, in_len);
			out_len = key_len + in_len;
		}
		break;

	case 3:
		{
			encryption_algorithm_3 ea;
			ea.create_key(0, 0, out_buffer, key_len);
			memcpy(out_buffer+key_len, in_buffer, in_len);
			ea.encrypt(out_buffer+key_len, in_len);
			out_len = key_len + in_len;
		}
		break;

	default:
		assert(false);
		memcpy(out_buffer, in_buffer, in_len);
		out_len = in_len;
		break;
	}
}

int mhxy_udp::decrypt(unsigned char *in_buffer, unsigned in_len, unsigned char *out_buffer, unsigned &out_len)
{
	assert((in_buffer!=0)&&(out_buffer!=0));
	int type;
	unsigned key_len;
	unsigned head = *(unsigned *)in_buffer;
	head = ntohl(head);  //added by dijie 09-11-12
	switch (head/0x20000000)
	{
	case 1:
		{
			encryption_algorithm_1 ea;
			if (ea.create_key(in_buffer, in_len, 0, key_len))
			{
				type = 1;
				out_len = in_len - key_len;
				memcpy(out_buffer, in_buffer+key_len, out_len);
				ea.decrypt(out_buffer, out_len);
			}
			else
			{
				type = 0;
				out_len = in_len;
				memcpy(out_buffer, in_buffer, in_len);
			}
		}
		break;

	case 2:
		{
			encryption_algorithm_2 ea;
			if (ea.create_key(in_buffer, in_len, 0, key_len))
			{
				type = 2;
				out_len = in_len - key_len;
				memcpy(out_buffer, in_buffer+key_len, out_len);
				ea.decrypt(out_buffer, out_len);
			}
			else
			{
				type = 0;
				out_len = in_len;
				memcpy(out_buffer, in_buffer, in_len);
			}
		}
		break;

	case 3:
		{
			encryption_algorithm_3 ea;
			if (ea.create_key(in_buffer, in_len, 0, key_len))
			{
				type = 3;
				out_len = in_len - key_len;
				memcpy(out_buffer, in_buffer+key_len, out_len);
				ea.decrypt(out_buffer, out_len);
			}
			else
			{
				type = 0;
				out_len = in_len;
				memcpy(out_buffer, in_buffer, in_len);
			}
		}
		break;

	default:
		type = 0;
		out_len = in_len;
		memcpy(out_buffer, in_buffer, in_len);
		break;
	}
	return type;
}

void mhxy_udp::encrypt(int type, unsigned char *io_buffer, unsigned &io_len)
{
	unsigned char temp_data[2048];
	unsigned temp_len;
	encrypt(type, io_buffer, io_len, temp_data, temp_len);
	memcpy(io_buffer, temp_data, temp_len);
	io_len = temp_len;
}

int mhxy_udp::decrypt(unsigned char *io_buffer, unsigned &io_len)
{
	unsigned char temp_data[2048];
	unsigned temp_len;
	int ret = decrypt(io_buffer, io_len, temp_data, temp_len);
	memcpy(io_buffer, temp_data, temp_len);
	io_len = temp_len;
	return ret;
}
