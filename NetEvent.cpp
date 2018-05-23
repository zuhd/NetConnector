#include "./NetEvent.h"
#include "SocketBase.h"

CNetEvent::CNetEvent()
{
}
void CNetEvent::Set(ISocketBase* pConnection, char* pPacket, int nPacketLen)
{
	m_pConnection = pConnection;
	if(nPacketLen >= 0 && pPacket != NULL)
	{
		m_nPacketLen = nPacketLen > MAX_PACKET_LEN ? MAX_PACKET_LEN : nPacketLen;
		memcpy(m_pPacketBuffer, pPacket, m_nPacketLen);
	}
	else
	{
		m_nPacketLen = nPacketLen;
	}
}

CNetEvent::~CNetEvent(void)
{
}

void CNetEvent::Process()
{
	if(m_nPacketLen == ISocketBase::ON_CONNECTION)
	{
		m_pConnection->OnConnection();
	}
	else if(m_nPacketLen == ISocketBase::ON_DISCONNECT)
	{
		m_pConnection->OnDisConnect();
	}
	else if(m_nPacketLen == ISocketBase::ON_DISCONNECTION)
	{
		m_pConnection->OnDisConnection();
	}
	else
	{
		m_pConnection->GetSession()->OnRecv(m_pPacketBuffer, m_nPacketLen);
	}
}
void CNetEvent::Release()
{
	delete this;
}

//////////////////////////////////////////////////////////////////////////
CNetUDPEvent::CNetUDPEvent(ISocketBase* pConnection, char* pPacket, int nPacketLen, struct sockaddr* peeraddr)
{
	m_pConnection = pConnection;
	if(nPacketLen >= 0 && pPacket != NULL)
	{
		m_nPacketLen = nPacketLen > MAX_PACKET_LEN ? MAX_PACKET_LEN : nPacketLen;
		memcpy(m_pPacketBuffer, pPacket, m_nPacketLen);
	}
	else
	{
		m_nPacketLen = nPacketLen;
	}
	memcpy(&m_stPeerAddr, peeraddr, sizeof(m_stPeerAddr));
}
void CNetUDPEvent::Process()
{
	m_pConnection->GetSession()->OnRecvFrom(&m_stPeerAddr, m_pPacketBuffer, m_nPacketLen);
}
void CNetUDPEvent::Release()
{
	delete this;
}