// encryption_algorithm_3.h: interface for the encryption_algorithm_3 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ENCRYPTION_ALGORITHM_3_H__E3C54AD6_A4E4_4897_8144_B62FCD609440__INCLUDED_)
#define AFX_ENCRYPTION_ALGORITHM_3_H__E3C54AD6_A4E4_4897_8144_B62FCD609440__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "encryption_algorithm.h"

class encryption_algorithm_3 : encryption_algorithm
{
public:
	virtual bool create_key( const unsigned char *data, unsigned len, void *key, unsigned &key_len );
	virtual bool encrypt( unsigned char *io_buffer, unsigned len );
	virtual bool decrypt( unsigned char *io_buffer, unsigned len );
	
private:
	unsigned char _key[12];
	unsigned _key_len;
	unsigned _key_pos;
};

#endif // !defined(AFX_ENCRYPTION_ALGORITHM_3_H__E3C54AD6_A4E4_4897_8144_B62FCD609440__INCLUDED_)
