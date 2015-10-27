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
#ifndef __URL_ENCRYPT_H
#define __URL_ENCRYPT_H
#include <string>
using namespace std;
namespace MYFRAMECOMMON{
#define AESKEYSTR "xunlei123!@#media"

const int MD5TEXTLEN=32;
/**
* dec:
* ClassName:CUrlEncrypt
* author: 
* version:
*CreateTime: 2008.7.12
*/
class CUrlEncrypt
{
public:
    CUrlEncrypt(const string aeskey=AESKEYSTR);
    ~CUrlEncrypt();
	string Encrypt(const string &sUrl);
	string Decrypt(const string &sCryptograph);
private:
	string m_aesKey;
	string m_md5Key;
};
}
#endif

