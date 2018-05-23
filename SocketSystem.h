#ifndef SOCKETSYSTEM_H
#define SOCKETSYSTEM_H
#ifdef WIN32
//#include <winsock2.h>
//#include <winsock.h>
//#include <WS2tcpip.h>
//#include <windows.h>
#include "WinInclude.h" 
typedef int socklen_t;

#define ERR_EAGAIN	WSAEWOULDBLOCK
#define	ERR_EINTR	WSAEINTR
#define ERR_EINPROGRESS  WSAEWOULDBLOCK
#define fcntl ioctlsocket
#define close closesocket

#pragma comment(lib,"wsock32")
#pragma comment(lib,"ws2_32.lib")
class CSocketPrepare
{
public:
	CSocketPrepare()
	{
		WSADATA wsaData;
		WORD version = MAKEWORD(2,0);
		::WSAStartup(version,&wsaData);
	}
	~CSocketPrepare()
	{
		WSACleanup();
	}
};
#else
#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<netdb.h>
#include<fcntl.h>
#include<unistd.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<arpa/inet.h>
#include<sys/time.h>
#include<signal.h>
#include <errno.h>
#define INVALID_SOCKET        -1
#define SOCKET_ERROR        -1

#define ERR_EAGAIN	EAGAIN
#define	ERR_EINTR	EINTR
#define ERR_EINPROGRESS  EINPROGRESS
#define GetLastError() (errno)
class CSocketPrepare
{
public:
	CSocketPrepare()
	{
		struct sigaction sa;
		sa.sa_handler = SIG_IGN;
		sa.sa_flags = 0;
		if (sigemptyset(&sa.sa_mask) == -1 ||
			sigaction(SIGPIPE, &sa, 0) == -1) {
				perror("failed to ignore SIGPIPE; sigaction");
				exit(EXIT_FAILURE);
		}
	}
};
#endif

#include "SelConnection.h"
class CSelectWork;
class CSocketSystem : public ISocketSystem
{
public:
	CSocketSystem(void);
	~CSocketSystem(void);

	virtual void Release();
	virtual int Run(int nLimit = 1);

	CSelConnection* GetSelConnection() { return new CSelConnection(this); }
	void PutSelConnection(CSelConnection* pConnection) { delete pConnection; }
	CSelectWork* GetSelectWork()	{ return m_pSelectWork; }
	virtual IListener* CreateListener();
	virtual IConnector* CreateConnector(int nType);
	virtual IUdpPeer* CreateUdpPeer(int nType);	
private:
	CSelectWork* m_pSelectWork;
};
#endif
