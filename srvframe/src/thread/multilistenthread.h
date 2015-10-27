#ifndef __MULTILISTENTHREAD_
#define __MULTILISTENTHREAD_
#include "listenthread.h"
class CMultiListenThread:public CListenThread
{
public:
	CMultiListenThread(int iEntityType, int iEntityId, 
	 	string sHost, int nPort, TListenBlock *pListenBlock):CListenThread(iEntityType, 
	 	iEntityId, sHost, nPort, pListenBlock)
 	{
 	}
       ~CMultiListenThread()
    	{
    	}

	int AddListen(const string &sHost, unsigned short nPort);
private:
	
};
#endif
