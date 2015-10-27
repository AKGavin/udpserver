/*
 * FileName:       
 * Author:         wangqifeng  Version: 1.0  Date: 2007-1-5
 * Description:    
 * Version:        
 * Function List:  
 *                 1.
 * History:        
 *     <author>   <time>    <version >   <desc>
 */
#ifndef __BASE_MAINTAIN_CMD_H
#define __BASE_MAINTAIN_CMD_H
#include "framecommon/framecommon.h"
using namespace MYFRAMECOMMON;
/**
* dec:
* ClassName:CBaseMaintainCmd
* author: 
* version:
*CreateTime: 2007.1.5
*/
class CBaseMaintainCmd
{
public:
        CBaseMaintainCmd(int nMsgType, int nBodyLen, char *pBody);
        virtual  ~CBaseMaintainCmd();
        virtual int Process();
        CBufMgr &GetSendBuf()
        {
                return m_sSendBuf;
        }
protected:
        CBufMgr m_sSendBuf;
        CBufMgr m_sRecvBuf;
        int m_nMsgType;
};

#endif
