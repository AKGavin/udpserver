// RSA.h: interface for the CRSA class.
#ifndef __RSA_H
#define __RSA_H

#include <openssl/rsa.h>
#include <openssl/md5.h>
#include <openssl/objects.h>
#include <string>

using namespace std;

namespace MYFRAMECOMMON{


#define BYTE char

//define certain byte_macro 1byte = 8bits 
#define Bits512 64
#define Bits1024 128
#define Bits2048 256

//define 
#define KeyLen512 512
#define KeyLen1024 1024
#define KeyLen2048 2048

#define Exponent3 0x3L
#define Exponent17 0x11L
#define Exponent65537 0x10001L

class CRSA
{
public:
	CRSA();
	virtual ~CRSA();
public:
	static int RSAGenKey( int bitsKeyLen , unsigned long ulExponent ,
		string &strOutPublicKeyN , string &strOutPublicKeyE , string &strOutPrivateKey );

	
	static int RSAPublicEncrypt( const string &strInPublicKeyN , const string &strInPublicKeyE , const string &strInPlaintext , 
		string &strOutCiphertext , int iPadding = RSA_PKCS1_PADDING );
	static int RSAPrivateDecrypt( const string &strInPublicKeyN , const string &strInPublicKeyE ,  const string &strInPrivateKeyD ,
		const string &strInCipertext , string &strOutPlaintext , int iPadding = RSA_PKCS1_PADDING );


	static int RSAPrivateEncrypt( const string &strInPublicKeyN , const string &strInPublicKeyE , const string &strInPrivateKeyD ,
		const string &strInPlaintext , string &strOutCiphertext , int iPadding = RSA_PKCS1_PADDING ); 
	static int RSAPublicDecrypt( const string &strInPublicKeyN , const string &strInPublicKeyE , const string &strInCipertext , 
		string &strOutPlaintext , int iPadding = RSA_PKCS1_PADDING );
	 

	static int RSASign( const string &strInPublicKeyN , const string &strInPublicKeyE , const string &strInPrivateKeyD , 
		const string &strInPlaintext , string &strSignReturn , int iType = NID_md5 );
	static int RSAVerify( const string &strInPublicKeyN , const string &strInPublicKeyE , const string &strInPlaintext ,
		const string &strInSigntext , int iType = NID_md5 );

	static int RSA_md5_sign( const string &strInPublicKeyN , const string &strInPublicKeyE , const string &strInPrivateKeyD ,
		const string &strInPlaintext , string &strSigntext ); 
	static int RSA_md5_verify( const string &strInPublicKeyN , const string &strInPublicKeyE , const string &strInPlaintext ,
		const string &strSigntext );

	static int Use_RSA_md5_sign( const string &strInPublicKeyN , const string &strInPublicKeyE , const string &strInPrivateKeyD ,
		unsigned int uiMessageId , const string &strInPlaintext , string &strSigntext );
	static int Use_RSA_md5_verify( const string &strInPublicKeyN , const string &strInPublicKeyE , 
		unsigned int uiMessageId , const string &strInPlaintext , const string &strSigntext );

};

}
#endif

