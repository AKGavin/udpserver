/*
 * FileName:       
 * Author:         wangqifeng  Version: 1.0  Date: 2007-4-5
 * Description:    
 * Version:        
 * Function List:  
 *                 1.
 * History:        
 *     <author>   <time>    <version >   <desc>
 */
#ifndef __THUNDER_HTTP_CMD_H
#define __THUNDER_HTTP_CMD_H
#include <string>
#include <map>
#include "framecommon/framecommon.h"
using namespace MYFRAMECOMMON;
using namespace std;

/**
* dec:
* ClassName:CMyHttpCmd
* author: 
* version:
*CreateTime: 2007.4.5
*/
class CMyHttpCmd
{
public:
        /**
         *
         *  \param   &sInHead
         *  \param   &sInBody
         *  \return   
         */
        CMyHttpCmd(string &sInHead, string &sInBody, string &sUrl);
     
        virtual ~CMyHttpCmd();

        
        int ProcessCmd();
        
        string &GetResBuf()
        {
                return m_sOut;
        }

        int GetParaList();

        void SetIp(unsigned int nIp)
        {
                m_nIp = nIp;
        }

        void SetAnnexInfo(int nIfAnnexInfo)
        {
                m_nIfAnnexInfo = nIfAnnexInfo;
        }

        u_short GetLingerOn()
        {
            return m_lingerOn;
        }
protected:
        virtual int Process()=0;
        virtual int Decode()=0;        
        virtual int Encode()=0;
        
        string m_sInHead;
        string m_sInBody;
        string m_sOut;
        string m_sOutBody;

        string m_sResHeader;
        
        int m_nIfSendRes;        
        int m_nResult;        
        string m_sUrl;
        map<string, string> m_paraList;
        map<string, string> m_cookieList;
        string m_sUuid;
        int m_nIfUuid;
        unsigned int m_nIp;
        int m_nPreTime;
        int m_nIfAnnexInfo;
        string m_sHostDomain;
        string m_sReferUrl;        
        int m_nIfExtra;

        string m_sPeerId;
        u_short m_lingerOn;
};
#endif

