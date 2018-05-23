#include "Session.h"
#include <iostream>
#include <algorithm>
using namespace std;

CSession::CSession()
{
	m_bIsConnect = false;
	m_bIsLogin   = false;
	m_nSID = 0;
	m_pConnection = NULL;
	m_cAgentType = -1;
	m_cSessionFlag = -1;
	m_cDeviceType = -1;
	m_nSerialNO = 0;
	m_setDstSID.clear();
	memset(&m_stRemoteAddr, 0, sizeof(m_stRemoteAddr));
	m_cSocketType = socket_type_tcp;
}

CSession::~CSession()
{	
}

void CSession::OnDisConnect()
{
	m_bIsConnect = false;
	m_bIsLogin = false;
	// 客户端主动连接失败回调
}

void CSession::OnDisConnection()
{
	m_bIsConnect = false;
	m_bIsLogin = false;
	// 服务器主动关闭连接回调
}

void CSession::OnConnection(IConnection* pConnection)
{
	cout << "On Connection" << endl;
	m_pConnection = pConnection;
	m_bIsConnect = true;
	// 客户端主动连接成功回调
	Login();
}

// 客户端主动关闭连接，释放资源 
void CSession::CloseConnection()
{		
	m_bIsConnect = false;
	m_bIsLogin = false;
	m_pConnection->CloseConnection();
}

// 连接成功后，用用户名登录到服务器
void CSession::Login()
{
	//用于登录
	msg_gate_account_login_req req;
	req.opcode = gate_account_login_req;
	req.nPID = GetPID();
	req.cAgentType = GetAgentType();
	req.cDeviceType = GetDeviceType();
	req.cSessionFlag = GetSessionFlag();
	SendPacket(req);
}

void CSession::OnRecv(const char* pData, int nLen)
{
	CInputStream is(pData, nLen);
	short opcode;
	is >> opcode;
	is.Reset();
	switch(opcode)
	{
		case gate_account_login_ack:		DISPATCH_MESSAGE(msg_gate_account_login_ack);		break;
		case gate_account_login_not:		DISPATCH_MESSAGE(msg_gate_account_login_not);		break;
		case gate_account_logout_ack:		DISPATCH_MESSAGE(msg_gate_account_logout_ack);		break;
		case gate_account_logout_not:		DISPATCH_MESSAGE(msg_gate_account_logout_not);		break;
		case gate_account_transdata_not:	DISPATCH_MESSAGE(msg_gate_account_transdata_not);	break;
		case gate_account_sessionlist_not:	DISPATCH_MESSAGE(msg_gate_account_sessionlist_not); break;
	}
}


void CSession::OnMessage(const msg_gate_account_login_not& noti)
{
	cout << "recv msg_gate_account_login_not" << endl;
	cout << noti.nPID << noti.nSID << endl;
	// 这里只处理和自己相同PID的
	if (GetPID() != noti.nPID)
	{
		return;
	}

	// 如果本端是主连接
	if (GetSessionFlag() == session_flag_trunk)
	{
		// 对方也是主连接
		if (noti.cSessionFlag == session_flag_trunk)
		{
			if (GetAgentType() != noti.cAgentType)
			{
				m_setDstSID.insert(noti.nSID);
			}
		}
	}
}

void CSession::OnMessage(const msg_gate_account_logout_not& noti)
{
	cout << "recv msg_gate_account_logout_not" << endl;
	cout << noti.nPID << noti.nSID << endl;
	if (GetPID() != noti.nPID)
	{
		return;
	}
	set<uint>::iterator it = std::find(m_setDstSID.begin(), m_setDstSID.end(), noti.nSID);
	if (it != m_setDstSID.end())
	{
		m_setDstSID.erase(it);
	}
	// 如果是子连接断开
	if (m_setDstSID.empty() && GetSessionFlag() == session_flag_branch)
	{
		// 主动关闭
		CloseConnection();
	}
}

void CSession::OnMessage(const msg_gate_account_transdata_not& noti)
{
	//cout << "recv msg_gate_account_transdata_not" << endl;
	//cout << "SrcSID = " << noti.nSrcSID << " DataLen = "<< noti.sDataLen << endl;
	int nTotalSize = noti.nTotalSize;
	if (nTotalSize <= 0)
	{
		return;
	}
	m_oDataBuffer.Write((char*)noti.arrData, noti.sDataLen);

	if (m_oDataBuffer.GetBufferLen() >= nTotalSize)
	{
		// 数据完整了
		msg_gate_account_transdata_complete msg;
		msg.nSrcPID = noti.nSrcPID;
		msg.nSrcSID = noti.nSrcSID;
		msg.nSerialNO = noti.nSerialNO;
		msg.pBuffer = m_oDataBuffer.GetBuffer(0);
		msg.nBufferLen = m_oDataBuffer.GetBufferLen();
		OnRecvComplete (msg);
		m_oDataBuffer.ClearBuffer();
	}
	else
	{
		glog.log("On Recv TransData, SerialNO=%d", noti.nSerialNO);
	}
}


// 注意：只有自己才会收到ack
void CSession::OnMessage(const msg_gate_account_login_ack& ack)
{
	cout << "recv msg_gate_account_login_ack" << endl;
	cout << ack.cRet << ack.nPID << ack.nSID << endl;
	if (ack.cRet == 0)
	{
		m_bIsLogin = true;
		SetSID(ack.nSID);
	}
}

//登出了，只有自己才会收到
void CSession::OnMessage(const msg_gate_account_logout_ack& ack)
{
	cout << "recv msg_gate_account_logout_ack" << endl;
	cout << ack.cRet << endl;
	if (ack.cRet == 0)
	{
		m_bIsLogin = false;
	}
}

//登录后获取所有之前登录的ID列表
void CSession::OnMessage(const msg_gate_account_sessionlist_not& noti)
{
	cout << "recv msg_gate_account_sessionlist_not" << endl;
	cout << "list size = " << (int)noti.vecSession.size() << endl;


	// 如果本端是主连接
	if (GetSessionFlag() == session_flag_trunk)
	{
		// 对方也是主连接
		for (int i = 0; i < (int)noti.vecSession.size(); i++)
		{
			const st_session& session = noti.vecSession[i];
			if (GetPID() != session.nPID)
			{
				continue;
			}

			if (session.cSessionFlag != session_flag_trunk)
			{
				continue;
			}

			if (GetSessionFlag() == session.cSessionFlag)
			{
				continue;
			}

			m_setDstSID.insert(session.nSID);
		}
	}
}

void CSession::SendData(const char* pData, int nSize)
{
	int			nRet = 0;
	const char	*pbuf = (char *)pData;
	int			size = 0;
	int			nSend = 0;
	int			nSendRetry = 15;
	int			nMaxSize = MAX_TRANSDATA_LEN;

	msg_gate_account_transdata_req req;
	req.opcode = gate_account_transdata_req;
	//req.nSerialNO = m_nSerialNO++;
	req.nTotalSize = nSize;
	req.vecDstSID.resize(m_setDstSID.size());
	std::copy(m_setDstSID.begin(), m_setDstSID.end(), req.vecDstSID.begin());
	req.nSrcPID = GetPID();
	req.nSrcSID = GetSID();
	// 依次发送
	for (size = nSize; size >= nMaxSize; size -= nMaxSize)
	{
		req.nSerialNO = m_nSerialNO++;
		req.sDataLen = nMaxSize;
		memcpy(req.arrData, pbuf, nMaxSize);	
		pbuf += nMaxSize;
		SendPacket(req);
	}
	// 发送最后的部分
	if (size > 0)
	{
		req.nSerialNO = m_nSerialNO++;
		req.sDataLen = size;
		memcpy(req.arrData, pbuf, size);
		SendPacket(req);
	}
}

void CSession::SendData(uint nSID, const char* pData, int nSize)
{
	int			nRet = 0;
	const char	*pbuf = (char *)pData;
	int			size = 0;
	int			nSend = 0;
	int			nSendRetry = 15;
	int			nMaxSize = MAX_TRANSDATA_LEN;

	msg_gate_account_transdata_req req;
	req.opcode = gate_account_transdata_req;
	//req.nSerialNO = m_nSerialNO++;
	req.nTotalSize = nSize;
	req.vecDstSID.push_back(nSID);
	req.nSrcPID = GetPID();
	req.nSrcSID = GetSID();
	// 依次发送
	for (size = nSize; size >= nMaxSize; size -= nMaxSize)
	{
		req.nSerialNO = m_nSerialNO++;
		req.sDataLen = nMaxSize;
		memcpy(req.arrData, pbuf, nMaxSize);	
		pbuf += nMaxSize;
		SendPacket(req);
	}
	// 发送最后的部分
	if (size > 0)
	{
		req.nSerialNO = m_nSerialNO++;
		req.sDataLen = size;
		memcpy(req.arrData, pbuf, size);
		SendPacket(req);
	}
}

void CSession::OnRecvComplete(msg_gate_account_transdata_complete& msg)
{

}

void CSession::HeartBit()
{
	// 发送心跳包
	msg_lg_net_echo_not noti;
	noti.opcode = lg_net_echo_not;
	noti.nTime = (uint)time(0);
	SendPacket(noti);
}

void CSession::OnRecvFrom(struct sockaddr* remoteaddr, const char* pData, int nLen)
{
	OnRecv(pData, nLen);
}

void CSession::InitUDPRemoteAddr(const char* pAddr, int nPort)
{
	m_stRemoteAddr.sin_family = AF_INET;
	m_stRemoteAddr.sin_addr.s_addr = inet_addr(pAddr);
	m_stRemoteAddr.sin_port = htons(nPort);
	m_cSocketType = socket_type_udp;
}
