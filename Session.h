#ifndef SESSION_H_
#define SESSION_H_
#include <set>
#include "System.h"
#include "GlobalDefine.h"
#include "ISocketSystem.h"
#include "MsgHeader.h"
#include "MsgDefine.h"
#include "Buffer.h"

using namespace std;
class CSession : public ISession
{
public:
	CSession();
	virtual ~CSession();
	void InitUDPRemoteAddr(const char* pAddr, int nPort);

	virtual void OnConnection(IConnection* pConnection);
	virtual void OnDisConnect();
	virtual void OnRecv(const char* pData, int nLen);
	virtual void Release() {}
	virtual void OnRecvFrom(struct sockaddr* remoteaddr, const char* pData, int nLen);
	virtual ISocketSystem* GetSocketSystem() { return NULL; }
	bool IsConnected(){ return m_pConnection == NULL ? false : m_pConnection->IsConnected(); }
	
	template<class T>
	void SendPacket(const T& packet)
	{
		COutputStream os;
		os << packet;
		Send(os.GetData(), os.GetLength());
	}

	void Send(const char* pData, int nLength)
	{
		if (m_cSocketType == socket_type_tcp)
		{
			__SendTCP(pData, nLength);
		}
		else
		{
			__SendUDP(pData, nLength);
		}
	}

	void CloseConnection();
	void SendData(const char* pData, int nSize);
	void SendData(uint nSID, const char* pData, int nSize);

	void Login();
	void HeartBit();

	uint GetSID() { return m_nSID; }
	void SetSID(uint nSID) { m_nSID = nSID; }

	uint GetPID() { return m_nPID; }
	void SetPID(uint nPID) { m_nPID = nPID; }

	char GetAgentType()	{ return m_cAgentType; }
	void SetAgentType(char cType) { m_cAgentType = cType; }

	char GetSessionFlag() { return m_cSessionFlag; }
	void SetSessionFlag(char cFlag) { m_cSessionFlag = cFlag; }

	char GetDeviceType() { return m_cDeviceType; }
	void SetDeviceType(char cType) { m_cDeviceType = cType; }

private:
	void __SendTCP(const char* pData, int nLength)
	{
		if(m_pConnection)
			m_pConnection->Send(pData, nLength);
	}

	void __SendUDP(const char* pData, int nLength)
	{
		if(m_pConnection)
			m_pConnection->SendTo((struct sockaddr*)&m_stRemoteAddr, pData, nLength);
	}
public:
	virtual void OnDisConnection();
	virtual void OnRecvComplete (msg_gate_account_transdata_complete& msg);
	virtual void OnMessage(const msg_gate_account_login_ack& ack);
	virtual void OnMessage(const msg_gate_account_login_not& noti);
	virtual void OnMessage(const msg_gate_account_logout_ack& ack);
	virtual void OnMessage(const msg_gate_account_logout_not& noti);
	virtual void OnMessage(const msg_gate_account_transdata_not& noti);
	virtual void OnMessage(const msg_gate_account_sessionlist_not& noti);

public:
	IConnection* m_pConnection;
	bool m_bIsConnect;					// 是否连接
	bool m_bIsLogin;					// 是否登录成功
	CBuffer m_oDataBuffer;				// 缓冲区
	set<uint> m_setDstSID;				// 目标SID
	uint m_nSID;						// 本Session的SID
	uint m_nPID;                        // 唯一的帐号ID 
	char m_cAgentType;					// 控制类型
	char m_cSessionFlag;				// 连接标记
	char m_cDeviceType;					// 设备类型
	uint m_nSerialNO;					// 发送包的序列号
	struct sockaddr_in m_stRemoteAddr;	// 对方IP地址
	char m_cSocketType;					// TCP OR UDP
};
#endif