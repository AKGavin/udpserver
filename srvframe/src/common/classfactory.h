/*
 * FileName:       
 * Author:         wangqifeng  Version: 1.0  Date: 2006-9-28
 * Description:    
 * Version:        
 * Function List:  
 *                 1.
 * History:        
 *     <author>   <time>    <version >   <desc>
 */
#ifndef __CLASS_FACTORY_H
#define __CLASS_FACTORY_H
#include "framecommon/framecommon.h"
#include "workthread.h"
#include "sockettask.h"
#include "maintainthread.h"
#include "basemaintaincmd.h"
#include "myhttpcmd.h"

using namespace MYFRAMECOMMON;


/**
* dec:
* ClassName:CClassFactory
* author: 
* version:
*CreateTime: 2006.9.28
*/
class CClassFactory
{
public:
        CClassFactory();
        
        virtual ~CClassFactory();

        
        virtual CWorkThread *GenWorkThread(int nEntityType, int nEntityId, string sAnnexData, void *arg=NULL);
        
        virtual CSocketTask *GenSocketTask(CNetProcessThread *pWorkThread, CTcpSocket *pTcpSocket,
                int iTaskType, CTaskArg * pTaskArg = NULL);
        
        virtual CMyHttpCmd *GenHttpCmd(string &sInHead, string &sInBody);
        
        virtual CBaseMaintainCmd *GenMaintainCmd(int nMsgType, int nBufLen, char *pBuf);

protected:
};
#endif

