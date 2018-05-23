#ifndef UDPPEER_H
#define UDPPEER_H
#include "ISocketSystem.h"

class CUDPConnection;
class IPacketParser;
class CSocketSystem;

class CUdpPeer : public IUdpPeer
{
public:
	CUdpPeer(int nType, CSocketSystem* pSocketSystem);
	~CUdpPeer(void);
	virtual void SetSession(ISession* pSession);
	virtual void SetPacketParser(IPacketParser* pParser);
	virtual bool BindHostAddr(const char* szHostAddr, int nPort);
	virtual void Release();
private:
	ISession* m_pSession;
	CUDPConnection* m_pConnection;
	IPacketParser* m_pPacketParser;
	char m_szUDPIP[64];
	int m_nUDPPort;
	CSocketSystem* m_pSocketSystem;
};
#endif
