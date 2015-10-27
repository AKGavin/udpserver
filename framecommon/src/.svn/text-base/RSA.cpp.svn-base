// RSA.cpp: implementation of the CRSA class.
//
//////////////////////////////////////////////////////////////////////
#include <stdlib.h>
#include <cstring>
#include "RSA.h"

namespace MYFRAMECOMMON{

#define RSA_ELEVEN 11
#define RSA_FORTY_TWO 42 

#define SSL_SIG_LENGTH 36
#define NID_md5_SIGN_LEN 99
#define NID_sha1_SIGN_LEN 102
#define NID_ripemd160_SIGN_LEN 102

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRSA::CRSA()
{

}

CRSA::~CRSA()
{

}

//////////////////////////////////////////////////////////////////////
// RSAGenKey describe:
//1.bitsKeyLen is the expected bit length of Generating Keys , it must equal to 512, 1024 or 2048 and KeyLen1024 is suggested for common user.
//2.ulExponent is the exponent used to generate Keys in method , it must equal to 3, 17, 65537 and Exponent3 is suggested for common user.
//////////////////////////////////////////////////////////////////////

int CRSA::RSAGenKey( int bitsKeyLen , unsigned long ulExponent , string &strOutPublicKeyN , string &strOutPublicKeyE , string &strOutPrivateKey )
{
	int iResult = 0;
	char * pKey = NULL;
	if ( bitsKeyLen != KeyLen512 && bitsKeyLen != KeyLen1024 && bitsKeyLen != KeyLen2048 )
		return -1;
	if ( ulExponent != Exponent3 && ulExponent != Exponent17 && ulExponent != Exponent65537 )
		return -1;
	RSA * pRSA = RSA_generate_key( bitsKeyLen , ulExponent  , NULL , NULL );
	if ( !pRSA )
		return -1;
	do 
	{
		pKey = BN_bn2hex( pRSA->n );
		if ( pKey )
		{
			strOutPublicKeyN = pKey;
			free( pKey );
		}			
		else
		{
			iResult = -1;
			break;
		}

		pKey = BN_bn2hex( pRSA->e );
		if ( pKey )
		{
			strOutPublicKeyE = pKey;
			free( pKey );	
		}
		else
		{
			iResult = -1;
			break;
		}

		pKey = BN_bn2hex( pRSA->d );
		if ( pKey )
		{
			strOutPrivateKey = pKey;
			free( pKey );
		}
		else
		{
			iResult = -1;
			break;
		}

	} while ( 0 );	
	
	RSA_free( pRSA );
	return iResult;
}


//if iPadding equal to RSA_PKCS1_PADDING or RSA_SSLV23_PADDING, the first parameter of RSA_public_encrypt must be less than RSA_size(pRsa) - RSA_ELEVEN 
//if iPadding equal to RSA_PKCS1_OAEP_PADDING , the first parameter of RSA_public_encrypt must be less than RSA_size(pRsa) - RSA_FORTY_TWO
//if iPadding equal to RSA_NO_PADDING , the first parameter of RSA_public_encrypt must be equal to RSA_size(pRsa)

int CRSA::RSAPublicEncrypt( const string &strInPublicKeyN , const string &strInPublicKeyE , const string &strInPlaintext ,
						   string &strOutCiphertext , int iPadding /* = RSA_PKCS1_PADDING */ )
{
	int iResult = 0;

	if( !strInPublicKeyN.size() || !strInPublicKeyE.size() )
		return -1;
	const int ciPlaintextLen = ( int )strInPlaintext.size();
	if( !ciPlaintextLen )
		return -1;

	RSA *pRsa = RSA_new();
	if ( !pRsa )
		return -1;

	do 
	{
		if( !BN_hex2bn( &pRsa->n , strInPublicKeyN.c_str() ) )
		{
			iResult = -1;
			break;
		}
		if( !BN_hex2bn( &pRsa->e , strInPublicKeyE.c_str() ) )
		{
			iResult = -1;
			break;
		}

		int iMaxSectionFLen = 0;

		const int ciRSASize = RSA_size( pRsa );
		if(  iPadding ==  RSA_PKCS1_PADDING || iPadding == RSA_SSLV23_PADDING )
			iMaxSectionFLen = ciRSASize - RSA_ELEVEN - 1;
		else if ( iPadding == RSA_PKCS1_OAEP_PADDING )
			iMaxSectionFLen = ciRSASize - RSA_FORTY_TWO - 1;
		else if ( iPadding == RSA_NO_PADDING )
			iMaxSectionFLen = ciRSASize;
		else
		{
			iResult = -1;
			break;
		}
		
		int iSignatureSize = 0;
		int iOffset = 0;
		int iSectionFLen = ( ( ciPlaintextLen - iOffset ) >= iMaxSectionFLen )? iMaxSectionFLen:( ciPlaintextLen - iOffset );
		const unsigned char * pStr = ( const unsigned char * )strInPlaintext.c_str();
		unsigned char * pOutBuf = new unsigned char[ciRSASize];
		strOutCiphertext.clear();
		while( !iResult && iSectionFLen )
		{
			iSignatureSize = RSA_public_encrypt( iSectionFLen , pStr ,  pOutBuf , pRsa , iPadding );
			if ( iSignatureSize == -1 )
			{
				iResult = -1;
				strOutCiphertext.clear();			
			}			
			else strOutCiphertext.append( ( char * )pOutBuf , iSignatureSize );
			iOffset += iSectionFLen;
			pStr += iSectionFLen;
			iSectionFLen = ( ( ciPlaintextLen - iOffset ) >= iMaxSectionFLen )? iMaxSectionFLen:( ciPlaintextLen - iOffset );
		}

		delete [] pOutBuf;
		pOutBuf = NULL;

	} while ( 0 );
	
	RSA_free( pRsa );
	pRsa = NULL;
	return iResult;
}

int CRSA::RSAPrivateDecrypt( const string &strInPublicKeyN , const string &strInPublicKeyE , const string &strInPrivateKeyD ,
							const string &strInCipertext , string &strOutPlaintext , int iPadding /* = RSA_PKCS1_PADDING */ )
{	
	int iResult = 0;

	if( !strInPrivateKeyD.size() )
		return -1;
	const int ciCipertextLen = ( int )strInCipertext.size();
	if( !ciCipertextLen )
		return -1;

	RSA *pRsa = RSA_new();
	if ( !pRsa )
		return -1;

	do 
	{
		iResult = 0;

		if( !BN_hex2bn( &pRsa->n , strInPublicKeyN.c_str() ) )
		{
			iResult = -1;
			break;
		}

		if( !BN_hex2bn( &pRsa->e , strInPublicKeyE.c_str() ) )
		{
			iResult = -1;
			break;
		}

		if( !BN_hex2bn( &pRsa->d , strInPrivateKeyD.c_str() ) )
		{
			iResult = -1;
			break;
		}

		const int ciSectionFLen = RSA_size( pRsa );
		if( strInCipertext.size() % ciSectionFLen != 0 )
		{
			iResult = -1;
			break;
		}
		else
		{
			int iSignatureSize = 0;
			int iOffset = 0;
			unsigned char * pStr = ( unsigned char * )strInCipertext.c_str();
			unsigned char * pOutBuf =  new unsigned char[ciSectionFLen];
			strOutPlaintext.clear();
			while( !iResult && ( ciCipertextLen - iOffset ) )
			{
				iSignatureSize = RSA_private_decrypt( ciSectionFLen , pStr ,  pOutBuf , pRsa , iPadding );
				if ( iSignatureSize == -1 )
				{
					iResult = -1;
					strOutPlaintext.clear();			
				}			
				else strOutPlaintext.append( (char *)pOutBuf , iSignatureSize );
				iOffset += ciSectionFLen;
				pStr += ciSectionFLen;
			}
			delete [] pOutBuf;
			pOutBuf = NULL;
			pStr = NULL;
		}

	} while ( 0 );
		
	RSA_free( pRsa );
	pRsa = NULL;
	return iResult;
}


//iPadding must be RSA_PKCS1_PADDING or RSA_NO_PADDING, and RSA_PKCS1_PADDING is suggested.
//if iPadding == RSA_NO_PADDING,the length of plaintext must be multiples of RSA_size(pRas)
//When generating or verifying PKCS #1 signatures,RSA_sign(3) and RSA_verify(3) should be used.
int CRSA::RSAPrivateEncrypt( const string &strInPublicKeyN , const string &strInPublicKeyE , const string &strInPrivateKeyD ,
							const string &strInPlaintext , string &strOutCiphertext , int iPadding /* = RSA_PKCS1_PADDING */ )
{
	int iResult = 0;
	if( !strInPublicKeyN.size() || !strInPrivateKeyD.size() )
		return -1;
	const int ciPlaintextLen = ( int )strInPlaintext.size();
	if( ciPlaintextLen == 0 )
		return -1;
	if( iPadding != RSA_PKCS1_PADDING && iPadding != RSA_NO_PADDING )
		return -1;
	RSA *pRsa = RSA_new();
	if ( !pRsa )
		return -1;

	do 
	{
		if( !BN_hex2bn( &pRsa->n , strInPublicKeyN.c_str() ) )
		{
			iResult = -1;
			break;
		}

		if( !BN_hex2bn( &pRsa->e , strInPublicKeyE.c_str() ) )
		{
			iResult = -1;
			break;
		}

		if( !BN_hex2bn( &pRsa->d , strInPrivateKeyD.c_str() ) )
		{
			iResult = -1;
			break;
		}

		int iMaxSectionFLen = 0;

		const int ciRSASize = RSA_size( pRsa );
		if(  iPadding ==  RSA_PKCS1_PADDING )
			iMaxSectionFLen = ciRSASize - RSA_ELEVEN - 1;
		else if ( iPadding == RSA_NO_PADDING )
		{
			iMaxSectionFLen = ciRSASize;
			if( strInPlaintext.size() % ciRSASize != 0  )
			{
				iResult = -1;
				break;
			}
		}			
		else
		{
			iResult = -1;
			break;
		}

		int iSignatureSize = 0;
		int iOffset = 0;
		int iSectionFLen = ( ( ciPlaintextLen - iOffset ) >= iMaxSectionFLen )? iMaxSectionFLen:( ciPlaintextLen - iOffset );
		const unsigned char * pStr = (const unsigned char *)strInPlaintext.c_str();
		unsigned char * pOutBuf = new unsigned char[ciRSASize];
		strOutCiphertext.clear();
		while( !iResult && iSectionFLen )
		{
			iSignatureSize = RSA_private_encrypt( iSectionFLen , pStr ,  pOutBuf , pRsa , iPadding );
			if ( iSignatureSize == -1 )
			{
				iResult = -1;
				strOutCiphertext.clear();			
			}			
			else strOutCiphertext.append( (char *)pOutBuf , iSignatureSize );
			iOffset += iSectionFLen;
			pStr += iSectionFLen;
			iSectionFLen = ( ( ciPlaintextLen - iOffset ) >= iMaxSectionFLen )? iMaxSectionFLen:( ciPlaintextLen - iOffset );
		}

		delete [] pOutBuf;
		pOutBuf = NULL;

	} while ( 0 );
	
	RSA_free( pRsa );
	return iResult;
}

//iPadding must be RSA_PKCS1_PADDING or RSA_NO_PADDING, and it must be the mode that was used to encrypt the data.
//When generating or verifying PKCS #1 signatures,RSA_sign(3) and RSA_verify(3) should be used.
int CRSA::RSAPublicDecrypt( const string &strInPublicKeyN , const string &strInPublicKeyE , const string &strInCipertext , 
						   string &strOutPlaintext , int iPadding /* = RSA_PKCS1_PADDING */ )
{
	int iResult = 0;
	if( !strInPublicKeyN.size() || !strInPublicKeyE.size() )
		return -1;
	const int ciCipertextLen = ( int )strInCipertext.size();
	if( ciCipertextLen == 0 )
		return -1;
	if( iPadding != RSA_PKCS1_PADDING && iPadding != RSA_NO_PADDING )
		return -1;
	RSA *pRsa = RSA_new();
	if ( !pRsa )
		return -1;

	do 
	{
		if( !BN_hex2bn( &pRsa->n , strInPublicKeyN.c_str() ) )
		{
			iResult = -1;
			break;
		}

		if( !BN_hex2bn( &pRsa->e , strInPublicKeyE.c_str() ) )
		{
			iResult = -1;
			break;
		}

		int iSignatureSize = 0;
		int iOffset = 0;
		unsigned const char * pStr = ( unsigned const char * )strInCipertext.c_str();
		strOutPlaintext.clear();
		const int ciSectionFLen = RSA_size( pRsa );
		if( strInCipertext.size() % ciSectionFLen != 0 )
		{
			iResult = -1;
			break;
		}
		unsigned char * pOutBuf = new unsigned char[ciSectionFLen];		//the buff len must be longer than ( RSA_size( pRsa ) - RSA_ELEVEN )

		while( !iResult && ( ciCipertextLen - iOffset ) )
		{
			iSignatureSize = RSA_public_decrypt( ciSectionFLen , pStr ,  pOutBuf , pRsa , iPadding );
			if ( iSignatureSize == -1 )
			{
				iResult = -1;
				strOutPlaintext.clear();			
			}			
			else strOutPlaintext.append( (char *)pOutBuf , iSignatureSize );
			iOffset += ciSectionFLen;
			pStr += ciSectionFLen;
		}
		delete [] pOutBuf;
		pOutBuf = NULL;
	
	} while ( 0 );
	
	RSA_free( pRsa );
	return iResult;
}

//iType must be NID_md5_sha1, NID_md5, NID_sha1 ,or NID_ripemd160 
	//if iType == NID_md5_sha1 , strInPlaintext.size() must multiples of SSL_SIG_LENGTH(36), and SSL_SIG_LENGTH is suggested
	//if iType == NID_md5 , strInPlaintext.size() is suggested no more than 99
	//if iType == NID_sha1 , strInPlaintext.size() is suggested no mmore 102
	//if iType == NID_ripemd160 ,  strInPlaintext.size() is suggested no mmore 102
int CRSA::RSASign( const string &strInPublicKeyN , const string &strInPublicKeyE , const string &strInPrivateKeyD , 
				  const string &strInPlaintext , string &strSignReturn , int iType /* = NID_md5 */ )
{

	int iResult = 0;
	if( !strInPublicKeyN.size() || !strInPrivateKeyD.size() )
		return -1;
	if( !strInPlaintext.size() )
		return -1;
	int iMaxSectionFLen = 0;
	const int ciPlaintextLen = ( int )strInPlaintext.size();
	if( iType == NID_md5_sha1 )
		if( ciPlaintextLen % SSL_SIG_LENGTH != 0 )
			return -1;
		else iMaxSectionFLen = SSL_SIG_LENGTH;
	else if( iType == NID_md5 )
		iMaxSectionFLen = NID_md5_SIGN_LEN;
	else if( iType == NID_sha1 )
		iMaxSectionFLen = NID_sha1_SIGN_LEN;
	else if( iType == NID_ripemd160 )
		iMaxSectionFLen = NID_ripemd160_SIGN_LEN;
	
	RSA * pRsa = RSA_new();
	if( !pRsa )
		return -1;
	do 
	{

		if( !BN_hex2bn( &pRsa->n , strInPublicKeyN.c_str() ) )
		{
			iResult = -1;
			break;
		}

		if( !BN_hex2bn( &pRsa->e , strInPublicKeyE.c_str() ) )
		{
			iResult = -1;
			break;
		}

		if( !BN_hex2bn( &pRsa->d , strInPrivateKeyD.c_str() ) )
		{
			iResult = -1;
			break;
		}

		const int ciRsaSize = RSA_size( pRsa );
		unsigned char * szSignReturn = new  unsigned char[ciRsaSize];
		unsigned int uiSignLen = 0;
		int iOffset = 0;
		int iSectionFLen = ( ( ciPlaintextLen - iOffset ) >= iMaxSectionFLen )? iMaxSectionFLen:( ciPlaintextLen - iOffset );
		const unsigned char * pStr = (const unsigned char *)strInPlaintext.c_str();

		strSignReturn.clear();
		while( !iResult && iSectionFLen != 0 )
		{
			if( !RSA_sign( iType , pStr  , iSectionFLen , szSignReturn , &uiSignLen , pRsa ) )
				iResult = -1;
			else
			{
				strSignReturn.append( (char *)szSignReturn , uiSignLen );
				iOffset += iSectionFLen;
				pStr += iSectionFLen;
				iSectionFLen = ( ( ciPlaintextLen - iOffset ) >= iMaxSectionFLen )? iMaxSectionFLen:( ciPlaintextLen - iOffset );
			}
		}		
		delete [] szSignReturn;
		szSignReturn = NULL;

	} while ( 0 );
	
	
	RSA_free( pRsa );
	pRsa = NULL;
	return iResult;
}

int CRSA::RSAVerify( const string &strInPublicKeyN , const string &strInPublicKeyE , const string &strInPlaintext ,
					const string &strInSigntext , int iType /* = NID_md5 */ )
{
	int iResult = 0;
	if( !strInPublicKeyN.size() || !strInPublicKeyE.size() )
		return -1;
	if( !strInPlaintext.size() || !strInSigntext.size() )
		return -1;
	const int ciPlaintextLen = ( int )strInPlaintext.size();
	const int ciSigntextLen = ( int )strInSigntext.size();
	int iMaxSectionFLen = 0;
	if( iType == NID_md5_sha1 )
		iMaxSectionFLen = SSL_SIG_LENGTH;
	else if( iType == NID_md5 )
		iMaxSectionFLen = NID_md5_SIGN_LEN;
	else if( iType == NID_sha1 )
		iMaxSectionFLen = NID_sha1_SIGN_LEN;
	else if( iType == NID_ripemd160 )
		iMaxSectionFLen = NID_ripemd160_SIGN_LEN;

	RSA * pRsa = RSA_new();
	if( !pRsa )
		return -1;
	do 
	{
		if( !BN_hex2bn( &pRsa->n , strInPublicKeyN.c_str() ) )
		{
			iResult = -1;
			break;
		}

		if( !BN_hex2bn( &pRsa->e , strInPublicKeyE.c_str() ) )
		{
			iResult = -1;
			break;
		}

		const int ciRsaSize = RSA_size( pRsa );
		if( strInSigntext.size() % ciRsaSize != 0)
		{
			iResult = -1;
			break;
		}
		else
		{
			unsigned char * szSignBuf = new unsigned char[strInSigntext.size()];
			unsigned char * szPlaintextBuf = new unsigned char[strInPlaintext.size()];
			memcpy( szSignBuf , strInSigntext.c_str() , strInSigntext.size() );
			memcpy( szPlaintextBuf , strInPlaintext.c_str() , strInPlaintext.size());
			int iPlainOffset = 0;
			int iSignOffset = 0;
			int iPlainSectionFLen = ( ( ciPlaintextLen - iPlainOffset ) >= iMaxSectionFLen )? iMaxSectionFLen:( ciPlaintextLen - iPlainOffset );

			while( !iResult && ( ciPlaintextLen - iPlainOffset ) && ( ciSigntextLen - iSignOffset )  )
			{
				if( !RSA_verify( iType , szPlaintextBuf + iPlainOffset ,iPlainSectionFLen , szSignBuf + iSignOffset , ciRsaSize , pRsa ) )
					iResult = -1;
				else
				{
					iPlainOffset += iPlainSectionFLen;
					iSignOffset += ciRsaSize;
					iPlainSectionFLen = ( ( ciPlaintextLen - iPlainOffset ) >= iMaxSectionFLen )? iMaxSectionFLen:( ciPlaintextLen - iPlainOffset );
					if( ( ciPlaintextLen - iPlainOffset )*( ciSigntextLen - iSignOffset ) == 0 && 
						( ciPlaintextLen - iPlainOffset ) != ( ciSigntextLen - iSignOffset ) )
						iResult = -1;
				}

			}
			delete [] szPlaintextBuf;
			delete [] szSignBuf ;
			szSignBuf = NULL;
		}

	} while ( 0 );
	
	
	RSA_free( pRsa );
	pRsa = NULL;
	return iResult;
}

int CRSA::RSA_md5_sign( const string &strInPublicKeyN , const string &strInPublicKeyE , const string &strInPrivateKeyD , 
					   const string &strInPlaintext , string &strSigntext )
{
	string strMd5;
	unsigned char szMd5[16] = { 0 };
	MD5( (const unsigned char *)strInPlaintext.c_str() , ( unsigned long )strInPlaintext.size() , szMd5 );
	strMd5.append( (char *)szMd5 , 16 );
	
	return RSAPrivateEncrypt( strInPublicKeyN , strInPublicKeyE , strInPrivateKeyD , strMd5 , strSigntext );	
}

int CRSA::RSA_md5_verify( const string &strInPublicKeyN , const string &strInPublicKeyE , 
						 const string &strInPlaintext , const string &strSigntext )
{
	string strMd5;
	string strPlainMd5;
	unsigned char szMd5[16] = { 0 };
	MD5( (const unsigned char *)strInPlaintext.c_str() , ( unsigned long )strInPlaintext.size() , szMd5 );
	strMd5.append( (char *)szMd5 , 16 );
	int iResult = RSAPublicDecrypt( strInPublicKeyN , strInPublicKeyE , strSigntext , strPlainMd5 );
	if( iResult )
		return -1;
	else if(  strMd5 == strPlainMd5 )
		return 0;
	else return 1;
}

int CRSA::Use_RSA_md5_sign( const string &strInPublicKeyN , const string &strInPublicKeyE , const string &strInPrivateKeyD ,
						   unsigned int uiMessageId , const string &strInPlaintext , string &strSigntext )
{
	string strRealPlaintext;
	strRealPlaintext.append( (char *)&uiMessageId , sizeof( uiMessageId ) );
	strRealPlaintext += strInPlaintext;
	return RSA_md5_sign( strInPublicKeyN , strInPublicKeyE , strInPrivateKeyD , strRealPlaintext , strSigntext );
}

int CRSA::Use_RSA_md5_verify( const string &strInPublicKeyN , const string &strInPublicKeyE , 
							 unsigned int uiMessageId , const string &strInPlaintext , const string &strSigntext )
{
	string strRealPlaintext;
	strRealPlaintext.append( (char *)&uiMessageId , sizeof( uiMessageId ) );
	strRealPlaintext += strInPlaintext;
	return RSA_md5_verify( strInPublicKeyN , strInPublicKeyE , strRealPlaintext , strSigntext );
}

}

