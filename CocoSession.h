#pragma once
#include "CocoSocket.h"
#include "System.h"
#include "GlobalDefine.h"
#include "ISocketSystem.h"
#include "MsgHeader.h"
#include "MsgDefine.h"
class CCocoSession : public ISocketHandler
{
public:
//	SINGLETON(CAccountSession)
	CCocoSession();
	~CCocoSession();

	virtual void OnConnection();
	virtual void OnDisConnect();
	virtual void OnDisConnection();
	virtual void OnRecv(const char* pData, int nLen);	
	void CloseConnection();
	void Login();

	uint GetSID()			{ return m_nSID; }
	void SetSID(uint nSID)	{ m_nSID = nSID; }
protected:
	void OnMessage(const msg_gate_account_login_ack& ack);
	void OnMessage(const msg_gate_account_login_not& noti);
	void OnMessage(const msg_gate_account_logout_ack& ack);
	void OnMessage(const msg_gate_account_logout_not& noti);
	void OnMessage(const msg_gate_account_transdata_not& noti);
public:
	template<class T>
	void SendPacket(const T& t)
	{
		COutputStream os;
		os << t;
		if(m_pConnector && m_pConnector->isConnected()) 
		{
			m_pConnector->write(os.GetData(), os.GetLength());
		}
	}
public:
	CocoSocket* m_pConnector;
	bool m_bIsConnect;					// 是否连接
	uint m_nSID;						// 当前连接ID
};