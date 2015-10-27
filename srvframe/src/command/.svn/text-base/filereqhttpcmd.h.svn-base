/*
 * FileName:       
 * Author:         wangqifeng  Version: 1.0  Date: 2007-7-27
 * Description:    
 * Version:        
 * Function List:  
 *                 1.
 * History:        
 *     <author>   <time>    <version >   <desc>
 */
#ifndef __FILE_REQ_HTTP_CMD_H
#define __FILE_REQ_HTTP_CMD_H
#include "myhttpcmd.h"
#include <map>
using namespace std;
class CMineType
{
public:
       map<string, string> m_minetype;
       CMineType()
       {
              m_minetype["html"] = "text/html";
              m_minetype["htm"] = "text/html";
              m_minetype["xml"] = "application/xml";
              m_minetype["js"] = "application/javascript";
       }
};

/**
* dec:
* ClassName:CFileReqHttpCmd
* author: 
* version:
*CreateTime: 2007.7.27
*/
class CFileReqHttpCmd:public CMyHttpCmd
{
public:
        CFileReqHttpCmd(string &sInHead, string &sInBody, string &sUrl);
        ~CFileReqHttpCmd();
protected:
        int Process();
        int Decode();        
        int Encode();
        string m_sFileData;
        static string m_sFileRootDir;
        static CMineType m_mineType;
		string m_sFileModTime;
};
#endif

