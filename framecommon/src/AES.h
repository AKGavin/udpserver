// AES.h: interface for the CAES class.
//
//////////////////////////////////////////////////////////////////////
#ifndef __AES__H
#define __AES__H
#include <string.h> 
#include <string>
using namespace std;
namespace MYFRAMECOMMON{

#define BYTE char

//define certain byte_macro 1byte = 8bits 
#define Bits128	16  //√‹‘ø≥§∂»
#define Bits192	24
#define Bits256	32

//define Key length
#define KeyLen128 128
#define KeyLen192 192
#define KeyLen256 256

#define ENCRYPT_BLOCK_SIZE 16

class CAES  
{
public:
	CAES();
	virtual ~CAES();
public:
       static int AESDecrypt(const string &sKey,  int bitLen, const char* pInData,int nInLen, string &sOut);        
       static int AESEncrypt(const string &sKey,  int bitLen,  const char* pInData,int nInLen,  string &sOut);
};
}
#endif 
