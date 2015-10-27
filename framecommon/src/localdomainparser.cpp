
#include "localdomainparser.h"

CLocalDomainParser* CLocalDomainParser::m_pInst = NULL;

CLocalDomainParser* CLocalDomainParser::GetInstance()
{
	if(NULL == m_pInst)
	{
		m_pInst = new CLocalDomainParser;
	}

	return m_pInst;
}

void CLocalDomainParser::DestroyInstance()
{
	if(NULL != m_pInst)
		delete m_pInst;

	m_pInst = NULL;
}

int CLocalDomainParser::ParseDomain(const string &sDomain, string &sIpAddr)
{
	int iRet = 0;
	
	if(m_domainList.find(sDomain) == m_domainList.end())
	{
		iRet = CToolKit::ParseDomain(sDomain, sIpAddr);
		if(0 == iRet)
		{
			IPINFO item;
			item.iUpdateTime = time(NULL);
			item.sIp = sIpAddr;
			CThreadLock lock(&m_mutex);
			m_domainList.insert(make_pair(sDomain, item));

			return iRet;
		}

		return -1;
	}

	time_t now = time(NULL);
	CThreadLock lock(&m_mutex);
	if(m_domainList[sDomain].iUpdateTime - now > FIVEMINUTE)
	{
		iRet = CToolKit::ParseDomain(sDomain, sIpAddr);
		if(0 != iRet)
			return -1;
		
		m_domainList[sDomain].iUpdateTime = now;
		m_domainList[sDomain].sIp = sIpAddr;
	}
	else
	{
		sIpAddr = m_domainList[sDomain].sIp;
	}

	return 0;
}

