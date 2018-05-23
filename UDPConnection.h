#ifndef UDPCONNECTION_H
#define UDPCONNECTION_H
#include "SocketBase.h"
#include "EventQueue.h"
#include "System.h"
#include "GlobalDefine.h"

class CSocketSystem;
class CUDPConnection : public ISocketBase
{
public:
	CUDPConnection(CSocketSystem* pSocketSystem);
	~CUDPConnection(void);
	virtual void Send(const char* pData, int nLen){ assert(false); }
	virtual void CloseConnection();
	virtual int GetSocketType() { return SO_UDPCONNECTION; }
	virtual void OnRecv();
	virtual ISession* GetSession() { return m_pSession; }
	virtual void GetPeerAddr(struct sockaddr* pRemoteAddr) { pRemoteAddr = NULL;}
	virtual void SetSession(ISession* pSession){assert(false);}

	virtual void SendTo(struct sockaddr* pRemoteAddr, const char* pData, int nLen);
	virtual void SetSockSession(ISession* pSession) { m_pSession = pSession; }
	virtual void SetSockPacketParser(IPacketParser* pParser);
	bool BindHostAddr(const char* szHostAddr, int nPort);
	virtual void ReConnect(){ assert(false);}
	virtual bool IsConnected(){ assert(false); return false; }

private:
	IPacketParser* m_pPacketParser;
	ISession* m_pSession;
	LG_SOCKET m_fdClient;
	char m_pRecvBuffer[MAX_PACKET_LEN];
	struct sockaddr m_stRemoteAddr;
	CSocketSystem* m_pSocketSystem;
};
#endif
