/*
 * FileName:       
 * Author:         wangqifeng  Version: 1.0  Date: 2008-7-12
 * Description:    
 * Version:        
 * Function List:  
 *                 1.
 * History:        
 *     <author>   <time>    <version >   <desc>
 */
#include "urlencrypt.h" 
#include "AES.h"
#include <openssl/md5.h>
#include "toolkit.h"
namespace MYFRAMECOMMON{
CUrlEncrypt::CUrlEncrypt(const string sAesKey)
{
    m_aesKey = sAesKey;
    unsigned char md[16] = {0};
    MD5((unsigned char*)m_aesKey.c_str(), m_aesKey.length(), md);
    m_md5Key = string((char*)md, 16);
}
CUrlEncrypt::~CUrlEncrypt()
{
}

string CUrlEncrypt::Encrypt(const string &sUrl)
{
    string sEncrypt;
    unsigned char md[16]={0};
    string sOut;
    CAES::AESEncrypt(m_md5Key, 128, sUrl.c_str(), sUrl.length(), sOut);      
    MD5((unsigned char*)sUrl.c_str(), sUrl.length(), md);
    string sTmp;
    sEncrypt=CToolKit::BiToHex((const char*)md, 16);
    sEncrypt+=CToolKit::BiToHex(sOut.c_str(), sOut.length());
    return sEncrypt;
}

string CUrlEncrypt::Decrypt(const string &sCryptograph)
{
    string sDecrypt="";
    if (sCryptograph.length() < MD5TEXTLEN*2 || sCryptograph.length()%MD5TEXTLEN != 0)
    {
        return sDecrypt;
    }
    string sMd5;
    sMd5=CToolKit::HexToBi(sCryptograph.c_str(), MD5TEXTLEN);
    string sTmp = sCryptograph.substr(MD5TEXTLEN);
    string sTmpUrl;
    sTmpUrl=CToolKit::HexToBi(sTmp.c_str(), sTmp.length());
    string sOut;
    CAES::AESDecrypt(m_md5Key, 128, sTmpUrl.c_str(), sTmpUrl.length(), sOut);
    unsigned char md[16]={0};
    MD5((unsigned char*)sOut.c_str(), strlen(sOut.c_str()), md);
    if (memcmp(sMd5.c_str(), md, 16) == 0)
    {
        sDecrypt = string(sOut.c_str(), strlen(sOut.c_str()));
    }
    return sDecrypt;    
}
}
