#include "./UdpPeer.h"
#include "UDPConnection.h"
#include "SocketSystem.h"
#include "NetPacketParser.h"

CUdpPeer::CUdpPeer(int nType, CSocketSystem* pSocketSystem)
{
	m_pConnection = new CUDPConnection(pSocketSystem);
	m_pSocketSystem = pSocketSystem;
}

CUdpPeer::~CUdpPeer(void)
{
	delete m_pConnection;
	m_pConnection = NULL;
}
void CUdpPeer::SetSession(ISession* pSession)
{
	m_pSession = pSession;
}
void CUdpPeer::SetPacketParser(IPacketParser* pParser)
{
	m_pPacketParser = pParser; 
}

bool CUdpPeer::BindHostAddr(const char* szHostAddr, int nPort)
{
	strncpy(m_szUDPIP, szHostAddr, sizeof(m_szUDPIP));
	m_nUDPPort = nPort;
	m_pConnection->SetSockSession(m_pSession);
	m_pSession->OnConnection(m_pConnection);
	m_pConnection->SetSockPacketParser(m_pPacketParser);
	return m_pConnection->BindHostAddr(m_szUDPIP, nPort);
}
void CUdpPeer::Release()
{
	delete this;
}
