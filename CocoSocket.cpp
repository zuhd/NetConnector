#include <iostream>
#include <list>
#include "CocoSocket.h"
#include "zlib.h"



#define MAX_MSG_LEN 1024*12
#ifdef _MSC_VER
#define CONST64(n) n ## ui64
typedef unsigned __int64 ulong64;
#else
#define CONST64(n) n ## ULL
typedef unsigned long long ulong64;
#endif


//-----------------implementions of class CocoSocket------------------------------------
CocoSocket::CocoSocket()
	:m_bConnected(false)
	,m_bReadable(false)
	,m_bWritable(false)
	,m_pSessionHandler(NULL)
	,m_TimeOut(0.0f)
	,m_CurrentTime(0.0f)
	,m_bNeedDel(false)
{
	m_szIp[0] = 0;
	m_bIPV6 = false;
	static CSocketPrepare prepare;
}

bool CocoSocket::create(int af, int type, int protocol)
{
	m_fd = ::socket(af, type, protocol);
	if (m_fd == INVALID_SOCKET)
	{
		return false;
	}

	//set no-blocking
#ifdef WIN32
	unsigned long ul = 1;
	::fcntl(m_fd, FIONBIO, (unsigned long*)&ul);
#else
	int flags = fcntl(m_fd, F_GETFL, 0);
	::fcntl(m_fd, F_SETFL, flags | O_NONBLOCK);
#endif
	int nNetTimeout = 33;//ms
	::setsockopt(m_fd, SOL_SOCKET, SO_SNDTIMEO, (char *)&nNetTimeout, sizeof(int));
	::setsockopt(m_fd, SOL_SOCKET, SO_RCVTIMEO, (char *)&nNetTimeout, sizeof(int));
	return true;
}

bool CocoSocket::connect(const char* ip, unsigned short port, float timeout)
{
	strcpy(m_szIp, ip);
	m_usPort = port;
	m_TimeOut = timeout;
	m_CurrentTime = 0;
	m_bConnected = false;

    int retval = -1;
	addrinfo* resaddr = NULL;
	addrinfo* paddrv6 = NULL;
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
	addrinfo hints;
	addrinfo* paddr = NULL;
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_flags = AI_DEFAULT;
	hints.ai_protocol = 0;
	hints.ai_socktype = SOCK_STREAM;
	ostringstream oss;
	oss << port;

	int err = getaddrinfo(ip, oss.str().c_str(), &hints, &resaddr);
	paddr = resaddr;
	while(paddr != NULL)
	{
		if (paddr->ai_family == AF_INET6)
		{
			m_bIPV6 = true;
			paddrv6 = paddr;
		}
		paddr = paddr->ai_next;
	}
#endif
	//configure socket info.
	if (m_bIPV6)
	{
        sockaddr_in6* addr6 = (sockaddr_in6*)paddrv6->ai_addr;
        struct sockaddr_in6 svraddr;
        memset(&svraddr, 0, sizeof(sockaddr_in6));
        svraddr.sin6_family = AF_INET6;
        svraddr.sin6_addr = addr6->sin6_addr;
        svraddr.sin6_port = htons(port);
        create(paddrv6->ai_family, paddrv6->ai_socktype, paddrv6->ai_protocol);
        retval = ::connect(m_fd, (struct sockaddr*)&svraddr, paddrv6->ai_addrlen);
	}
	else
	{
        create(AF_INET, SOCK_STREAM, 0);
		struct sockaddr_in svraddr;
        memset(&svraddr, 0, sizeof(sockaddr_in));
		svraddr.sin_family = AF_INET;
		svraddr.sin_addr.s_addr = inet_addr(ip);
		svraddr.sin_port = htons(port);
		retval = ::connect(m_fd, (struct sockaddr*)&svraddr, sizeof(svraddr));
	}
    if (resaddr != NULL)
    {
        freeaddrinfo(resaddr);
    }
    if (retval == 0)
	{
		onConnectedEvent();
	}
	if (GetLastError() != ERR_EINPROGRESS)
	{
		if (m_pSessionHandler)
			m_pSessionHandler->OnDisConnect();
		return false;
	}
	//add to reactor.
	CocoSocketReactor::getInstance().addSocket(this);
	return true;
}

int CocoSocket::forceClose()
{
	m_bConnected = false;
	//CocoSocketReactor::getInstance().delSocket(this);
	int retval = ::close(m_fd);
	m_fd = -1;		
	return retval;
}

void CocoSocket::shutdown()
{
	::shutdown(m_fd, 0xffffffff);
	m_fd = -1;
	m_bConnected = false;
}

void CocoSocket::write(const char* data, int len)
{
	char pPacket[MAX_MSG_LEN];
	int nPacketLen = MAX_MSG_LEN;
	m_pPacketParser.EncodePacket(data, len, pPacket, nPacketLen);

	m_WriteBuffer.Fill(pPacket, nPacketLen);
}


void CocoSocket::onErrorEvent()
{
}

bool CocoSocket::onReadEvent()
{
	char* base = m_ReadBuffer.GetPtr();
	int used = m_ReadBuffer.GetLength();
	int len = m_ReadBuffer.GetFreeLength();
	int retval = 0;
	if (len > 0)
	{
		retval = ::recv(m_fd, base + used, len, 0);
		if (retval == -1)
		{
			int err = GetLastError();
			if (err == ERR_EAGAIN || err == ERR_EINTR)
			{
				//nothing can read.
				return true;
			}
			else
			{
				onDisconnectedEvent();
				return false;
			}
		}
		else if (retval == 0)
		{
			onDisconnectedEvent();
			return false;
		}
		m_ReadBuffer.Fill(retval);
	}
	//successful and dispatch msg.
	if (retval > 0)
	{
		int ret = onDispatch(base, m_ReadBuffer.GetLength());
		m_ReadBuffer.Shrink(ret);
	}
	return true;
}

bool CocoSocket::onWriteEvent()
{
	char* base = m_WriteBuffer.GetPtr();
	int used = m_WriteBuffer.GetLength();
	int retval = 0;
    if (base == NULL)
        return true;
	if (used > 0)
	{
		
		retval = send(m_fd, base, used, 0);
		if (retval == -1)
		{
			int err = GetLastError();
			if (err == ERR_EAGAIN || err == ERR_EINTR)
			{
				//nothing has been written.
				return true;
			}
			else
			{
				//something wrong.
				onDisconnectedEvent();
				return false;
			}
		}
		else if (retval == 0)
		{
			onDisconnectedEvent();
			return false;
		}

		//write successfully.
		//shrink buffer data.
		m_WriteBuffer.Shrink(retval);
	}
	return true;
}

void CocoSocket::onConnectedEvent()
{
	m_bConnected = true;
	if (m_pSessionHandler)
	{
		m_pSessionHandler->OnConnection();
	}
}

void CocoSocket::onConnectFailedEvent()
{
	if (m_pSessionHandler)
	{
		m_pSessionHandler->OnDisConnect();
	}
	forceClose();
}
void CocoSocket::onMessageEvent(const char* msg, int len)
{
	if (m_pSessionHandler)
	{
		m_pSessionHandler->OnRecv(msg, len);
	}
}
void CocoSocket::onDisconnectedEvent()
{
	if (m_pSessionHandler)
	{
		m_pSessionHandler->OnDisConnection();
	}
}

int CocoSocket::onDispatch(const char* data, int len)
{
	int nShrinkLen = 0;
	char pPacket[MAX_MSG_LEN * 2] = {0};
	int nMaxPacket = 1024 * 12; 		
	int nParserLen = 0; 	
	int nRecvOffset = len;
	char pRecvBuffer[MAX_MSG_LEN] = {0};
	memcpy(pRecvBuffer, data, len);

	while(true)
	{
		//最大包长，实际长度
		int nMaxPacket = MAX_MSG_LEN; 						
		if(nRecvOffset <= 0)
			return nShrinkLen;
		// 把RecvBuffer的数据解析到pPacket中
		// 在DecodePacket中能读取到包的长度
		nParserLen = m_pPacketParser.DecodePacket(pRecvBuffer, nRecvOffset, pPacket, nMaxPacket);
		if(nParserLen > 0)
		{
			nShrinkLen += nParserLen;
			nRecvOffset -= nParserLen; //剩余长度
			if(nMaxPacket > 0 && nMaxPacket <= MAX_MSG_LEN) //取到完整包, 进队列
			{
				if (m_pSessionHandler)
				{
					m_pSessionHandler->OnRecv(pPacket, nMaxPacket);
				}			
			}	
			// 如果还有剩余的长度，证明
			// 有多个包，进入下个循环
			// 把数据前移到开始处
			if(nRecvOffset > 0)
			{
				//memcpy(m_pRecvBuffer, &m_pRecvBuffer[nParserLen], m_nRecvOffset); 
				memmove(pRecvBuffer, &pRecvBuffer[nParserLen], nRecvOffset);				
			}

		}
		else
		{
			break; //无完整包可以处理
		}
	}	

	//
	return nShrinkLen;
}

bool CocoSocket::dnsParse(const char* domain, char*ip)
{
	struct hostent*p;
	if ((p = gethostbyname(domain)) == NULL)
		return false;
	return true;
}

bool CocoSocket::FlushData()
{
	return onWriteEvent();
}

//-----------------implementions of class CocoSocketReactor------------------------------------
void CocoSocketReactor::addSocket(CocoSocket* tor)
{
	m_ConnectorList.push_back(tor);
}

void CocoSocketReactor::delSocket(CocoSocket* tor)
{
	std::list<CocoSocket*>::iterator it = m_ConnectorList.begin();
	for (; it != m_ConnectorList.end(); )
	{
		if (tor == *it)
		{
			m_ConnectorList.erase(it++);
			break;
		}
		else
		{
			 ++it;
		}
	}
}

void CocoSocketReactor::handleEvents()
{
	fd_set m_ReadFDs;
	fd_set m_WriteFDs;
	fd_set m_ErrorFDs;
	//clear all
	FD_ZERO(&m_ReadFDs);
	FD_ZERO(&m_WriteFDs);
	FD_ZERO(&m_ErrorFDs);

	//add all fd
	int maxfd = -1;
	for (std::list<CocoSocket*>::iterator it = m_ConnectorList.begin();
		it != m_ConnectorList.end(); ++it)
	{
		CocoSocket* tor = *it;
		FD_SET(tor->fd(), &m_ReadFDs);
		FD_SET(tor->fd(), &m_WriteFDs);
		FD_SET(tor->fd(), &m_ErrorFDs);
		maxfd = tor->fd() > maxfd ? tor->fd() : maxfd;
	}

	struct timeval t = { 0,1 };
	int retval = ::select(maxfd + 1, &m_ReadFDs, &m_WriteFDs, &m_ErrorFDs, &t);
	//nothing changed.
	if (retval == 0)
	{
		return;
	}

	// 必须要把已经关闭的socket移除掉，否则出错
	// select error
	if (retval < 0)
	{		
		return;
	}

	std::list<CocoSocket*>::iterator it = m_ConnectorList.begin();
	while (it != m_ConnectorList.end())
	{
		bool needRemove = false;
		CocoSocket* tor = *it;
		//write event
		if (FD_ISSET(tor->fd(), &m_WriteFDs))
		{
			tor->m_bWritable = true;
		}

		//read event
		if (FD_ISSET(tor->fd(), &m_ReadFDs))
		{
			tor->m_bReadable = true;
		}

		//error
		if (FD_ISSET(tor->fd(), &m_ErrorFDs))
		{
			if (tor->isConnected() == false)
			{
				tor->onConnectFailedEvent();
			}
			else
			{
				tor->onDisconnectedEvent();
			}
			tor->onErrorEvent();
			needRemove = true;
		}

		if (tor->isConnected() == false)
		{
			if (tor->m_bWritable == true)
			{
#ifdef WIN32
				tor->onConnectedEvent();
#else
				//speical for osx,ios
#ifdef __APPLE__
				char buf[1];
				int retval = ::read(tor->fd(), buf, 0);
				if (retval == 0)
				{
					tor->onConnectedEvent();
				}
				else
				{
					tor->onConnectFailedEvent();
					needRemove = true;
				}
#else
				//speical for android/linux
				int ret;
				int len = 4;
				::getsockopt(tor->fd(), SOL_SOCKET, SO_ERROR, (char*)&ret,&len);
				if (ret == 0 || ret == ERR_EINPROGRESS)
				{
					tor->onConnectedEvent();
				}
				else
				{
					tor->onConnectFailedEvent();
					needRemove = true;
				}
#endif
				/*

				*/
#endif
			}
		}
		else
		{
			if (tor->m_bReadable)
			{
				needRemove = !tor->onReadEvent();
			}
			if (needRemove == false && tor->m_bWritable)
			{
				needRemove = !tor->onWriteEvent();
			}
		}

		tor->m_bWritable = false;
		tor->m_bReadable = false;

		if (needRemove == true)
		{
			it = m_ConnectorList.erase(it);
		}
		else
		{
			++it;
		}
	}
}

void CocoSocketReactor::update(float dt)
{
	for (std::list<CocoSocket*>::iterator it = m_ConnectorList.begin();
		it != m_ConnectorList.end();)
	{
		CocoSocket* tor = *it;
		if (tor->isConnected() == false && tor->m_TimeOut > 0)
		{
			if (tor->m_CurrentTime > tor->m_TimeOut)
			{
				tor->onConnectFailedEvent();
				it = m_ConnectorList.erase(it);
				continue;
			}
			tor->m_CurrentTime += dt;
		}
		else if (tor->NeedDel())
		{
			SAFE_DELETE(*it);
			it = m_ConnectorList.erase(it);
			continue;
		}
		++it;
	}
	handleEvents();
}

CocoSocketReactor::CocoSocketReactor()
{
	
}

CocoSocketReactor& CocoSocketReactor::getInstance()
{
	static CocoSocketReactor obj;
	return obj;
}
