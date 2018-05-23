#ifndef SOCKETBASE_H
#define SOCKETBASE_H

#include "GlobalDefine.h"
#include "ISocketSystem.h"
#include "System.h"
#include <assert.h>

const int SO_LISTENER = 1;
const int SO_CONNECTION = 2;
const int SO_CONNECTOR = 3;
const int SO_UDPCONNECTION = 4;
const int SO_UDPLISTENNER = 5;

class ISocketBase : public IConnection
{
public:
	enum
	{
		MAX_OVERLAP_BUFFER = MAX_PACKET_LEN * 2,
		ON_CONNECTION = -1,
		ON_DISCONNECTION = -2,
		ON_DISCONNECT = -3,
	};
	virtual ~ISocketBase(void){}
	virtual int GetSocketType() = 0;
	virtual void GetPeerAddr(struct sockaddr* pRemoteAddr) { pRemoteAddr = NULL;}
	virtual void SetSession(ISession* pSession){assert(false);}
	virtual void SetSockPacketParser(IPacketParser* pParser = NULL) = 0;/* {assert(false);}*/
	static bool SetReusePort(LG_SOCKET sock);
	static bool SetNonBlocking(LG_SOCKET sock);
	virtual void OnConnection(){assert(false);};
	virtual void OnDisConnect(){assert(false);};
	virtual void OnDisConnection(){assert(false);};
	virtual ISession* GetSession(){ return NULL;};
	virtual void OnRecv(){assert(false);};
	virtual void OnSend(){assert(false);};
	virtual void OnAccept(){assert(false);};
	virtual void Connect(const char* szRemoteAddr, int nPort, int nProxyType = 0, 
		const char* szProxyAddr = 0, int nProxyPort = 0,
		const char* szProxyUser = 0, const char* szProxyPwd = 0){assert(false);}
};
#endif
