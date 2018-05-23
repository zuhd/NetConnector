
#include "NetEvent.h"
#include <string.h>
#include <string>
#include "NetPacketParser.h"
#include "SelectWork.h"
#include "Define.h"
#include "SelectWork.h"
#include "SocketSystem.h"
#include "Utilty.h"
#include "./SelConnection.h"

using namespace std;

CSelConnection::CSelConnection(CSocketSystem* pSocketSystem)
{
	m_nSendOffset = 0;
	m_nRecvOffset = 0;
	m_fdClient = -1;
	m_bIsConnected = false;
	//	m_SeleWork.Init();
	m_bConnPending = false;
	m_nProxyType = 0;
	m_bIsClient = false;
	m_pThread = INVALID_HANDLE_VALUE;
	m_pSocketSystem = pSocketSystem;
}

CSelConnection::~CSelConnection(void)
{
	m_nSendOffset = 0;
	m_nRecvOffset = 0;
	m_fdClient = -1;
	m_bIsConnected = false;

}
void CSelConnection::SetSockPacketParser(IPacketParser* pParser)
{
	if(pParser == NULL)
	{
		static CNetPacketParser Parse;
		m_pPacketParser = &Parse;
	}
	else
	{
		m_pPacketParser = pParser; 
	}
}
void CSelConnection::ReConnect()
{
	Connect(m_szRemoteAddr, m_nRemotePort, m_nProxyType, m_szProxyAddr, m_nProxyPort, m_szProxyUser, m_szProxyPwd);
}
bool CSelConnection::IsConnected()
{
	return m_bIsConnected;
}
void CSelConnection::GetPeerAddr(struct sockaddr* pRemoteAddr)
{
	socklen_t len = sizeof(sockaddr);
	if(getpeername(m_fdClient, pRemoteAddr, &len))
	{
		glog.debug(strerror(errno));
	}
}

void CSelConnection::Connect(const char* szRemoteAddr, int nPort, int nProxyType, 
							 const char* szProxyAddr, int nProxyPort,
							 const char* szProxyUser, const char* szProxyPwd)
{
	m_pThread = INVALID_HANDLE_VALUE;
	m_bIsClient = true;
	if(m_bIsConnected)
	{
		OnDisConnection();
	}
	if(m_bConnPending)
	{
		return;
	}
	else
	{
		m_bConnPending = true; //正在处理连接中....
	}
	memset(m_szRemoteAddr, 0, sizeof(m_szRemoteAddr));
	memset(m_szProxyAddr, 0, sizeof(m_szProxyAddr));
	memset(m_szProxyUser, 0, sizeof(m_szProxyUser));
	memset(m_szProxyPwd, 0, sizeof(m_szProxyPwd));

	if(nProxyType != HTTP && (isalpha(szRemoteAddr[0]) || isalpha(szRemoteAddr[1])))
	{
		hostent* hptr;
		if((hptr = gethostbyname(szRemoteAddr)) == NULL)
		{
			goto FAILED;
		}
		if(hptr->h_addr == NULL)
		{
			goto FAILED;
		}
		strncpy(m_szRemoteAddr, inet_ntoa(*(in_addr*)hptr->h_addr), sizeof(m_szRemoteAddr));
	}
	else
	{
		strncpy(m_szRemoteAddr, szRemoteAddr, sizeof(m_szRemoteAddr));
	}
	m_nRemotePort = nPort;
	m_nProxyType = nProxyType;
	if(szProxyAddr != NULL)
	{
		strncpy(m_szProxyAddr, szProxyAddr, sizeof(m_szProxyAddr));
		m_nProxyPort = nProxyPort;
	}
	if(szProxyUser)
	{
		strncpy(m_szProxyUser, szProxyUser, sizeof(m_szProxyUser));
	}
	if(szProxyPwd)
	{
		strncpy(m_szProxyPwd, szProxyPwd, sizeof(m_szProxyPwd));
	}

	struct sockaddr_in serveraddr;
	memset(&serveraddr,0, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	m_fdClient = socket(AF_INET, SOCK_STREAM, 0);

	if(nProxyType == NONE)//不使用代理
	{
		ISocketBase::SetNonBlocking(m_fdClient);
		serveraddr.sin_addr.s_addr = inet_addr(szRemoteAddr);
		serveraddr.sin_port = htons(nPort);
		m_pSocketSystem->GetSelectWork()->AddSocket(m_fdClient, CSelectWork::eWriteEvent | CSelectWork::eExceptEvent, this);
		connect(m_fdClient, (sockaddr*)&serveraddr, sizeof(serveraddr));
	}
	else//使用代理
	{
		p_thread_t m_pThread;
		p_thread_create(&m_pThread, Threadproc, (p_thread_arg_t)this);
	}
	return;
//
FAILED:
		OnDisConnect();
}


void CSelConnection::OnDisConnect()
{
	m_bIsConnected = false;
	m_pSocketSystem->GetSelectWork()->DelSocket(m_fdClient);
	m_bConnPending = false; //连接处理完成
	m_pSession->OnDisConnect();
}

void CSelConnection::OnDisConnection()
{
	m_bIsConnected = false;
	m_pSocketSystem->GetSelectWork()->DelSocket(m_fdClient);
	closesocket(m_fdClient);
	m_fdClient = -1;
	if(m_pSession != NULL)
	{
		m_pSession->OnDisConnection();
		m_pSession->Release();
	}
	if(!m_bIsClient && m_pSocketSystem)
	{
		m_pSocketSystem->PutSelConnection(this);
	}
}
void CSelConnection::OnConnection()
{
	m_nSendOffset = 0;
	m_nRecvOffset = 0;

	// 清除队列事件
	for(;;)
	{
		CNetEvent* pEvent = m_quSendBuffer.PopEvent();
		if(pEvent == NULL)
		{
			break;
		}
		else
		{
			pEvent->Release();
		}
	}
	m_bIsConnected = true;
	m_pSocketSystem->GetSelectWork()->ModifySocket(m_fdClient, CSelectWork::eReadEvent, this);
	if(m_pSession != NULL)
	{
		m_pSession->OnConnection(this);
		m_bConnPending = false; //连接处理完成
	}
}
void CSelConnection::_OnConnection(LG_SOCKET fdcli)
{
	m_fdClient = fdcli;
	m_pSocketSystem->GetSelectWork()->AddSocket(m_fdClient, CSelectWork::eReadEvent, this);
	OnConnection();
}
void CSelConnection::CloseConnection()
{
	if(m_pThread != INVALID_HANDLE_VALUE)
		p_wait_thread_exit(&m_pThread);
	OnDisConnection();
}
void CSelConnection::Send(const char* pData, int nLen)
{
	if(m_fdClient == -1)
	{
		glog.debug("INetConnection::Send() socket invalid!");
		return;
	}
	// 如果当前Buffer没有数据，无需排队
	if(m_quSendBuffer.Size() == 0)
	{
		char pPacket[MAX_PACKET_LEN];
		int nPacketLen = MAX_PACKET_LEN;
		m_pPacketParser->EncodePacket(pData, nLen, pPacket, nPacketLen);
		if(m_nSendOffset + nPacketLen + 4 < MAX_OVERLAP_BUFFER)
		{
			memcpy(&m_pSendBuffer[m_nSendOffset], pPacket, nPacketLen);
			m_nSendOffset += nPacketLen;
			m_pSocketSystem->GetSelectWork()->ModifySocket(m_fdClient, CSelectWork::eReadEvent | CSelectWork::eWriteEvent, this);
			return;
		}
	}

	CNetEvent* pEvent = new CNetEvent();
	if(!pEvent)
	{
		return;
	}
	char* pPacket = pEvent->GetData();
	int nPacketLen = MAX_PACKET_LEN;
	m_pPacketParser->EncodePacket(pData, nLen, pPacket, nPacketLen);
	pEvent->SetLength(nPacketLen);
	m_quSendBuffer.PushEvent(pEvent);
	m_pSocketSystem->GetSelectWork()->ModifySocket(m_fdClient, CSelectWork::eReadEvent | CSelectWork::eWriteEvent, this);
	return;	

}
void CSelConnection::OnRecv()
{
	char pPacket[MAX_OVERLAP_BUFFER];
	if (m_nRecvOffset >= MAX_OVERLAP_BUFFER)
	{
		return;
	}

	int nTransfered = recv(m_fdClient, &m_pRecvBuffer[m_nRecvOffset], MAX_OVERLAP_BUFFER - m_nRecvOffset, 0);
	if(nTransfered <= 0)
	{
		if(nTransfered <0)
			glog.debug("CBDNetConnection::OnRecv() recv failed! err:%s", strerror(errno));
		OnDisConnection();
		return;
	}
	m_nRecvOffset += nTransfered;

	while(true)
	{
		int nMaxPacket = MAX_PACKET_LEN; //最大包长，实际长度
		int nParserLen = 0; //到次处理长度
		nParserLen = m_pPacketParser->DecodePacket(m_pRecvBuffer, m_nRecvOffset, pPacket, nMaxPacket);
		if(nParserLen > 0)
		{
			m_nRecvOffset -= nParserLen; //剩余长度
			//memcpy(m_pRecvBuffer, &m_pRecvBuffer[nParserLen], m_nRecvOffset); //前移
			memmove(m_pRecvBuffer, &m_pRecvBuffer[nParserLen], m_nRecvOffset); //前移

			if(nMaxPacket > 0 && nMaxPacket <= MAX_PACKET_LEN) //取到完整包, 进队列
			{
				m_pSession->OnRecv(pPacket, nMaxPacket);
			}
			else
			{
				glog.debug("CBDNetConnection::OnRecv() invalidate packet!! packet len:%d", nMaxPacket);
			}
		}
		else
		{
			break; //无完整包可以处理
		}
	}
}

void CSelConnection::OnSend()
{
	if(!m_bIsConnected) //客户端且没有连接成功前
	{
		sockaddr_in peeraddr;
		socklen_t len = sizeof(sockaddr_in);
		int ret = getpeername(m_fdClient, (sockaddr*)&peeraddr, &len);
		if(ret < 0)
		{
			OnDisConnect(); //连接失败
		}
		else
		{
			OnConnection(); //连接成功
		}
		return;
	}

	if(m_fdClient == -1)
	{
		glog.debug("OnSend failed, socket invalid!");
		return;
	}
	
	if(m_nSendOffset > 0)
	{
		int nTransfered = send(m_fdClient, m_pSendBuffer, m_nSendOffset, 0);
		if(nTransfered == -1)
		{
			return;
		}
		else
		{
			m_nSendOffset -= nTransfered;
			memcpy(m_pSendBuffer, &m_pSendBuffer[nTransfered], m_nSendOffset);
		}
		return;
	}

	CNetEvent* pEvent = m_quSendBuffer.GetFront();
	if(pEvent == NULL)
	{
		m_pSocketSystem->GetSelectWork()->ModifySocket(m_fdClient, CSelectWork::eReadEvent, this);
		return;
	}
	int nTransfered = send(m_fdClient, pEvent->GetData(), pEvent->GetLength(), 0);
	if(nTransfered == -1)
	{
		return;
	}
	else
	{
		if(nTransfered < pEvent->GetLength())
		{
			pEvent->TransferData(nTransfered);
		}
		else
		{
			m_quSendBuffer.PopEvent();
			pEvent->Release();
		}
	}
}

p_thread_retval_t CSelConnection::Threadproc(void* pArg)
{
	CNetEvent* pEvent = new CNetEvent();
	CSelConnection* pCon = (CSelConnection*)pArg;

	struct sockaddr_in serveraddr;
	memset(&serveraddr,0, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;

	serveraddr.sin_addr.s_addr = inet_addr(pCon->m_szProxyAddr);
	serveraddr.sin_port = htons(pCon->m_nProxyPort);
	if(0 != connect(pCon->m_fdClient, (sockaddr*)&serveraddr, sizeof(serveraddr)))
	{
		goto FAILED;
	}
	if(pCon->m_nProxyType == SOCK4)
	{
		char buffer[100] = {0};
		memset(buffer, 0, sizeof(buffer));
		struct sock4req1* req = (sock4req1*)buffer;
		req->VN = 4;
		req->CD = 1;
		req->IPAddr = inet_addr(pCon->m_szRemoteAddr);
		req->Port = htons(pCon->m_nRemotePort);
		if(send(pCon->m_fdClient, (char*)req, 9, 0) < sizeof(req))
		{
			goto FAILED;
		}
		memset(buffer, 0, sizeof(buffer));
		if(recv(pCon->m_fdClient, buffer, sizeof(buffer), 0) < 0)
		{
			goto FAILED;
		}
		struct sock4ans1* ack = (sock4ans1*)buffer;
		if(ack->CD != 90 || ack->VN != 0)
		{
			goto FAILED;
		}
	}
	else if(pCon->m_nProxyType == HTTP)
	{		
		char buffer[512] = {0};
		string strSend;
		string strEnd = "Accept: */*\r\n"
			"Content-Type: text/html\r\n"
			"Proxy-Connection: Keep-Alive\r\n"
			"Content-length: 0\r\n\r\n";
		snprintf(buffer, sizeof(buffer), "CONNECT %s:%d HTTP/1.1\r\n", pCon->m_szRemoteAddr, pCon->m_nRemotePort);

		strSend = buffer;
		snprintf(buffer, sizeof(buffer), "%s:%s",pCon->m_szProxyUser,pCon->m_szProxyPwd);

		char chrBase64[4096];
		leogon::Base64Encode(buffer, chrBase64, (int)strlen(buffer));
		snprintf(buffer, sizeof(buffer), "Proxy-Authorization: Basic %s\r\n", chrBase64);
		strSend += buffer;
		strSend += strEnd;
		if(send(pCon->m_fdClient, strSend.c_str(),	(int)strSend.length(), 0) < 0)
		{
			goto FAILED;
		}
		char recvbuffer[600];
		if(recv(pCon->m_fdClient, recvbuffer, sizeof(recvbuffer), 0) < 0)
		{
			goto FAILED;
		}
		if(strstr(recvbuffer, "HTTP/1.0 200 Connection established") == NULL)
		{
			goto FAILED;
		}
	}
	else if(pCon->m_nProxyType == SOCK5)
	{
		char buffer[600];
		memset(buffer, 0, sizeof(buffer));
		struct sock5req1* req1 = (struct sock5req1*)buffer;
		req1->Ver = 5;
		req1->nMethods = 1;
		req1->Methods[0] = 2;
		req1->Methods[1] = 2;
		if(send(pCon->m_fdClient, (char*)req1, 3, 0) < 0)
		{
			goto FAILED;
		}
		if(recv(pCon->m_fdClient, buffer, sizeof(buffer), 0) < 0)
		{
			goto FAILED;
		}
		struct sock5ans1* ans1 = (sock5ans1*)buffer;
		if(ans1->Ver != 5 || (ans1->Method != 0 && ans1->Method != 2))
		{
			goto FAILED;
		}
		if(ans1->Method == 2)
		{
			int nUserLen = (int)strlen(pCon->m_szProxyUser);
			int nPassLen = (int)strlen(pCon->m_szProxyPwd);
			memset(buffer, 0, sizeof(buffer));
			buffer[0] = 1;
			buffer[1] = nUserLen;
			strcpy(&buffer[2], pCon->m_szProxyUser);

			buffer[2 + nUserLen] = nPassLen;

			strcpy(&buffer[3 + nUserLen], pCon->m_szProxyPwd);
			
			if(send(pCon->m_fdClient, buffer, 3 + nUserLen + nPassLen, 0) < 0)
			{
				goto FAILED;
			}
			memset(buffer, 0, sizeof(buffer));
			if(recv(pCon->m_fdClient, buffer, sizeof(buffer), 0) < 0)
			{
				goto FAILED;
			}
			struct authans* aans = (authans*)buffer;
			if((aans->Ver != 1 && aans->Ver != 5) || aans->Status != 0)
			{
				goto FAILED;
			}
		}
		memset(buffer, 0, sizeof(buffer));
		struct sock5req2* req2 = (sock5req2*)buffer;
		req2->Ver = 5;
		req2->Cmd = 1;
		req2->Rsv = 0;
		req2->Atyp = 1;
		req2->IPAddr = inet_addr(pCon->m_szRemoteAddr);
		req2->Port = htons(pCon->m_nRemotePort);		
		if(send(pCon->m_fdClient, (char*)req2, 10, 0) < 0)
		{
			goto FAILED;
		}
		if(recv(pCon->m_fdClient, buffer, sizeof(buffer), 0) < 0)
		{
			goto FAILED;
		}
		struct sock5ans2* ans2 = (sock5ans2*)buffer;
		if(ans2->Ver != 5 || ans2->Rep != 0)
		{
			goto FAILED;
		}
	}
	else
	{
		goto FAILED;
	}
	ISocketBase::SetNonBlocking(pCon->m_fdClient);
	pCon->GetSocketSystem()->GetSelectWork()->AddSocket(pCon->m_fdClient, CSelectWork::eReadEvent, pCon);	
	pEvent->Set(pCon, NULL, (int)ON_CONNECTION);
	pCon->GetSocketSystem()->GetSelectWork()->PushEvent(pEvent);
	return 0 ;
FAILED:
	pEvent->Set(pCon, NULL, (int)ON_DISCONNECT);
	pCon->GetSocketSystem()->GetSelectWork()->PushEvent(pEvent);
	return 0 ;

}
