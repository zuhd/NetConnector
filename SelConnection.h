#ifndef SELCONNECTION_H
#define SELCONNECTION_H
#include "SocketBase.h"
#include "EventQueue.h"
#include "System.h"
#include "GlobalDefine.h"

class IPacketParser;
class ISession;
class CNetEvent;
class CSocketSystem;

class CSelConnection : public ISocketBase
{
private:
	enum
	{
		NONE,
		SOCK4,
		SOCK5,
		HTTP,
	};
public:
	CSelConnection(CSocketSystem* pSocketSystem);
	~CSelConnection(void);
	virtual void Send(const char* pData, int nLen);
	virtual void CloseConnection();
	virtual void ReConnect();
	virtual bool IsConnected();
	virtual void GetPeerAddr(struct sockaddr* pRemoteAddr);
	virtual void SetSession(ISession* pSession){ m_pSession = pSession;}

	virtual int GetSocketType() { return SO_CONNECTION; }
	virtual LG_SOCKET GetSocket() { return m_fdClient; }
	virtual void SendTo(struct sockaddr* pRemoteAddr, const char* pData, int nLen){ assert(false); }

	virtual void OnConnection();
	virtual void OnDisConnection();
	virtual void OnDisConnect();
	virtual ISession* GetSession(){ return m_pSession; }

	virtual void Connect(const char* szRemoteAddr, int nPort, int nProxyType, 
		const char* szProxyAddr, int nProxyPort,
		const char* szProxyUser, const char* szProxyPwd);
	virtual void SetSockSession(ISession* pSession) { m_pSession = pSession; }
	virtual void SetSockPacketParser(IPacketParser* pParser);

public:
	void _OnConnection(LG_SOCKET fdcli);
	CSocketSystem* GetSocketSystem()	{ return m_pSocketSystem; }

protected:
	virtual void OnRecv();
	virtual void OnSend();
private:
	static p_thread_retval_t P_THREAD_PROC Threadproc(void* pArg); 

	char m_pSendBuffer[MAX_OVERLAP_BUFFER];
	leogon::CEventQueue<CNetEvent*, leogon::CLocker> m_quSendBuffer;

	char m_pRecvBuffer[MAX_OVERLAP_BUFFER];
	int m_nSendOffset;
	int m_nRecvOffset;
	leogon::CLocker m_lockSend;
	leogon::CLocker m_lockRecv;
	IPacketParser* m_pPacketParser;
	ISession* m_pSession;
	bool m_bIsConnected;	//是否连接成功
	bool m_bConnPending;	//是否在连接过程中
	bool m_bIsClient;		//是否为客户端用
	p_thread_t m_pThread;
	CSocketSystem* m_pSocketSystem;

public:
	LG_SOCKET m_fdClient;
	char m_szRemoteAddr[64];
	int m_nRemotePort;
	char m_szProxyAddr[64];
	int m_nProxyPort;
	char m_szProxyUser[64]; //代理用户名
	char m_szProxyPwd[64]; //代理密码
	int m_nProxyType; //代理类型
};
#endif
