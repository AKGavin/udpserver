// AES.cpp: implementation of the CAES class.
//
//////////////////////////////////////////////////////////////////////
#include "AES.h"
#include <openssl/aes.h>
namespace MYFRAMECOMMON{

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAES::CAES()
{

}

CAES::~CAES()
{

}


//////////////////////////////////////////////////////////////////////
// AESDncrypt describe:
//1.bitLen is the bit length of sKey(sKey.length*8) and it must equal to 128 , 192 or 256
//2.pInData must be an array with length 16 bytes(128bits)
//3.the out string sOut will have a length of 16
//////////////////////////////////////////////////////////////////////
int CAES::AESDecrypt(const string &sKey, int bitLen, const char* pInData,int nInLen, string &sOut)
{
        if (pInData == NULL)
        {
                return -1;
        }

        if (nInLen%ENCRYPT_BLOCK_SIZE != 0)
        {
                return -1;
        }

        int bReturn = -1;
        AES_KEY key;
        AES_set_decrypt_key((unsigned char*)sKey.c_str(), bitLen, &key);
        
        int nInOffset = 0;
        //int nOutOffset = 0;
        unsigned char inBuff[ENCRYPT_BLOCK_SIZE+1]={0};
        unsigned char ouBuff[ENCRYPT_BLOCK_SIZE+1]={0};
        while(nInLen - nInOffset > 0)
        {
                memcpy(inBuff,pInData+nInOffset,ENCRYPT_BLOCK_SIZE);
                AES_decrypt(inBuff, ouBuff, &key);
                sOut.append((const char*)ouBuff, ENCRYPT_BLOCK_SIZE);
                nInOffset += ENCRYPT_BLOCK_SIZE;
        }
        //nOutLen = nOutOffset;
        bReturn = 0;
        
        return bReturn;
}
//////////////////////////////////////////////////////////////////////
// AESDncrypt describe:
//1.bitLen is the bit length of sKey(sKey.length*8) and it must equal to 128 , 192 or 256
//2.pInData must be an array with length 16 bytes(128bits)
//3.the out string sOut will have a length of 16
//////////////////////////////////////////////////////////////////////
int CAES::AESEncrypt(const string &sKey,  int bitLen, const char* pInData,int nInLen,string &sOut)
{
        if (pInData == NULL) // || pOutData == NULL)
        {
                return -1;
        }

        int bReturn = -1;
       
        AES_KEY key;
        AES_set_encrypt_key((unsigned char*)sKey.c_str(), bitLen, &key);
        int nInOffset = 0;
        unsigned char inBuff[ENCRYPT_BLOCK_SIZE+1]={0};
        unsigned char ouBuff[ENCRYPT_BLOCK_SIZE+1]={0};
        while(1)
        {
                if (nInLen - nInOffset >= ENCRYPT_BLOCK_SIZE)
                {
                        memcpy(inBuff,pInData+nInOffset,ENCRYPT_BLOCK_SIZE);
                        AES_encrypt(inBuff, ouBuff, &key);
                        sOut.append((const char*)ouBuff, ENCRYPT_BLOCK_SIZE);
                        nInOffset += ENCRYPT_BLOCK_SIZE; 
                }
                else
                {
                        memset(inBuff,0,ENCRYPT_BLOCK_SIZE+1);
                        memset(ouBuff,0,ENCRYPT_BLOCK_SIZE+1);
                        int nData = nInLen - nInOffset;
                        if (nData > 0)
                        {
                                memcpy(inBuff,pInData+nInOffset,nData);
                                AES_encrypt(inBuff, ouBuff, &key);        
                                sOut.append((const char*)ouBuff, ENCRYPT_BLOCK_SIZE);
                                nInOffset += nData;                                
                        }
                        break;
                }
        }
        //nOutLen = nOutOffset;
        bReturn = 0;
    
        return bReturn;
}
}

