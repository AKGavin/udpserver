#ifndef __LOCAL_DOMAIN_PARSER_H__
#define __LOCAL_DOMAIN_PARSER_H__

#include <map>
#include "framecommon.h"

using namespace std;

namespace MYFRAMECOMMON{

const int ONEHOURSEC = 3600;
const int FIVEMINUTE = 300;

typedef struct 
{
	string sIp;
	time_t iUpdateTime;
}IPINFO;

class CLocalDomainParser
{
protected:
	CLocalDomainParser(){}
	~CLocalDomainParser(){}
	
public:
	//---
	static CLocalDomainParser *GetInstance();
	//---
	static void DestroyInstance();
	//---
	int ParseDomain(const string &sDomain, string &sIpAddr);

protected:
	map<string, IPINFO> m_domainList;
	CThreadMutex m_mutex;
	
	static CLocalDomainParser *m_pInst;
};

}
#endif

