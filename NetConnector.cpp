#include <assert.h>
#include <string.h>
#include "./NetConnector.h"
#include "./SelConnection.h"
#include "Log.h"

CNetConnector::CNetConnector(int nType, CSocketSystem* pSocketSystem)
{
	m_bConnected = false;
	m_pSocketSystem = pSocketSystem;
	if(nType == 0)
	{
		assert(false);
	}
	else if(nType == 1)
	{
		m_pConnection = new CSelConnection(pSocketSystem);
	}
	else
	{
		assert(false);
	}
	m_pPacketParser = NULL;
	m_pSession = NULL;
}

CNetConnector::~CNetConnector(void)
{
	m_bConnected = false;
	delete m_pConnection;
}
void CNetConnector::SetSession(ISession* pSession)
{
	m_pSession = pSession;
}
void CNetConnector::SetPacketParser(IPacketParser* pParser)
{
	m_pPacketParser = pParser;
}
void CNetConnector::Connect(const char* szRemoteAddr, int nPort, int nProxyType, 
							const char* szProxyAddr, int nProxyPort,
							const char* szProxyUser, const char* szProxyPwd)
{
	memset(m_szRemoteAddr,0,sizeof(m_szRemoteAddr));
	strncpy(m_szRemoteAddr, szRemoteAddr, sizeof(m_szRemoteAddr));
    m_nPort = nPort;
    LOGI("======%s===%d===",m_szRemoteAddr,m_nPort);

	m_pConnection->SetSockPacketParser(m_pPacketParser);
	m_pConnection->SetSession(m_pSession);
	m_pConnection->Connect(szRemoteAddr, nPort, nProxyType, szProxyAddr, nProxyPort, szProxyUser, szProxyPwd);
}
void CNetConnector::Release()
{
	delete this;
}
