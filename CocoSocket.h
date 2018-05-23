#ifndef _COCO_SOCKET_H_
#define _COCO_SOCKET_H_

#define CC_PLATFORM_IOS 0
#define CC_PLATFORM_ANDROID 1
#define CC_PLATFORM_WINDOWS 2
#define CC_TARGET_PLATFORM CC_PLATFORM_WINDOWS

#include "SocketSystem.h"
#include "NetPacketParser.h"
#include "GlobalDefine.h"
#include <list>
#include <string>
#include <string.h>
using namespace std;

class IBuffer
{
	char* buffer;
	unsigned int length;
	unsigned int LEN;
public:
	IBuffer(char* p,unsigned int l)
		:buffer(p)
		,length(0)
		,LEN(l)
	{

	}
public:
	char* GetPtr()
	{
		return buffer;
	}

	int GetLength()
	{
		return length;
	}

	int GetFreeLength()
	{
		return LEN - length;
	}

	void Fill(unsigned int len)
	{
		length += len;
	}

	void Fill(const void* data,unsigned int len)
	{
		memcpy(buffer+length,data,len);
		length += len;
	}

	void Shrink(unsigned int len)
	{
		memmove(buffer,buffer+len,length - len);
		length -= len;
	}
};

template<int MAX_LEN>
class CCocoBuffer:public IBuffer
{
	char buffer[MAX_LEN];
public:
	CCocoBuffer():IBuffer(buffer,MAX_LEN)
	{

	}
};

//------------------------------------------------------------------------------------

class ISocketHandler
{
public:
	virtual void OnConnection() = 0;
	virtual void OnRecv(const char* msg,int len) = 0;
	virtual void OnDisConnection() = 0;
	virtual void OnDisConnect() = 0;
};

class CocoSocket
{
	friend class CocoSocketReactor;
	char m_szIp[256];
	unsigned short m_usPort;
	int m_fd;
	bool m_bConnected;
	CCocoBuffer<10240> m_ReadBuffer;
	CCocoBuffer<10240> m_WriteBuffer;
	ISocketHandler* m_pSessionHandler;
	bool m_bWritable;
	bool m_bReadable;
	float m_TimeOut;
	float m_CurrentTime;
	CNetPacketParser m_pPacketParser;
	bool m_bNeedDel;
	bool m_bIPV6;

public:
	CocoSocket();
	void SetNeedDel(bool bNeed) {m_bNeedDel = true;}
	bool NeedDel() {return m_bNeedDel;}
	void setHandler(ISocketHandler* p){ m_pSessionHandler = p;}
	int fd(){ return m_fd; }
	const char* ip()const { return m_szIp; }
	unsigned short	port()const { return m_usPort; }
	bool isConnected(){ return m_bConnected; }
	bool connect(const char* ip, unsigned short port, float timeout = -1);
	int forceClose();
	void shutdown();
	void write(const char* data, int len);
	bool FlushData();
public:
	static bool dnsParse(const char* domain, char*ip);
private:
	bool create(int af, int type, int protocol = 0);
	void onErrorEvent();
	bool onReadEvent();
	bool onWriteEvent();
	void onConnectedEvent();
	void onConnectFailedEvent();
	void onMessageEvent(const char* msg, int len);
	void onDisconnectedEvent();
	int onDispatch(const char* data, int len);
};

class CocoSocketReactor
{
	friend class CocoSocket;
	std::list<CocoSocket*> m_ConnectorList;
private:
	CocoSocketReactor();
public:
	static CocoSocketReactor& getInstance();
public:
	void update(float dt);
	void handleEvents();
	void addSocket(CocoSocket* tor);
	void delSocket(CocoSocket* tor);
};

#endif