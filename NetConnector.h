#ifndef NETCONNECTOR_H
#define NETCONNECTOR_H
#include "System.h"
#include "ISocketSystem.h"
#include "SocketBase.h"
class CSocketSystem;

class CNetConnector : public IConnector
{
public:
	CNetConnector(int nType, CSocketSystem* pSocketSystem);
	~CNetConnector(void);
	virtual void SetSession(ISession* pSession);
	virtual void SetPacketParser(IPacketParser* pParser);
	virtual void Connect(const char* szRemoteAddr, int nPort, int nProxyType, 
		const char* szProxyAddr, int nProxyPort,
		const char* szProxyUser, const char* szProxyPwd);
	virtual void Release();
private:
	LG_SOCKET m_fdClient;
	IPacketParser* m_pPacketParser;
	char m_szRemoteAddr[128];
	int m_nPort;
	int m_bConnected;
	ISocketBase* m_pConnection;
	ISession* m_pSession;
	CSocketSystem* m_pSocketSystem;
};

#endif
