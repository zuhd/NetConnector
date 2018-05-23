#include "CocoSession.h"
#include <iostream>
using namespace std;

CCocoSession::CCocoSession()
{
	m_bIsConnect = false;
	m_nSID = 0;
}

CCocoSession::~CCocoSession()
{	
	SAFE_DELETE(m_pConnector);
}

void CCocoSession::OnDisConnect()
{
	m_bIsConnect = false;
}

void CCocoSession::OnDisConnection()
{
	m_bIsConnect = false;
}

void CCocoSession::OnConnection()
{
	cout << "On Connection" << endl;
	m_bIsConnect = true;
	Login();
}

void CCocoSession::CloseConnection()
{		
	m_pConnector->forceClose();
	CocoSocketReactor::getInstance().delSocket(m_pConnector);
	SAFE_DELETE(m_pConnector);
}

void CCocoSession::OnRecv(const char* pData, int nLen)
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
	}
}

// 注意：只有自己才会收到ack
void CCocoSession::OnMessage(const msg_gate_account_login_ack& ack)
{
	cout << "recv msg_gate_account_login_ack" << endl;
	cout << ack.cRet << ack.nPID << ack.nSID << endl;
	if (ack.cRet == 0)
	{
		SetSID(ack.nSID);
	}
}

// 注意：只会收到其他人的登录noti
void CCocoSession::OnMessage(const msg_gate_account_login_not& noti)
{
	cout << "recv msg_gate_account_login_not" << endl;
	cout << noti.nPID << noti.nSID << endl; 
}

void CCocoSession::OnMessage(const msg_gate_account_logout_ack& ack)
{
	cout << "recv msg_gate_account_logout_ack" << endl;
	cout << ack.cRet << endl;
}

void CCocoSession::OnMessage(const msg_gate_account_logout_not& noti)
{
	cout << "recv msg_gate_account_logout_not" << endl;
	cout << noti.nPID << noti.nSID << endl; 
}

void CCocoSession::OnMessage(const msg_gate_account_transdata_not& noti)
{
	cout << "recv msg_gate_account_transdata_not" << endl;
	cout << noti.nSrcSID << noti.sDataLen << endl;
}

void CCocoSession::Login()
{
	//用于登录
	msg_gate_account_login_req req;
	req.opcode = gate_account_login_req;
	req.nPID = 2000;
	req.cAgentType = agent_type_client;
	req.cDeviceType = device_type_android;
	req.cSessionFlag = session_flag_trunk;
	SendPacket(req);
}






