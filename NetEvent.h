#ifndef NETEVENT_H
#define NETEVENT_H
#include "IBaseEvent.h"
#include "GlobalDefine.h"
#include "System.h"
#include <string.h>

class ISocketBase;
class CNetEvent : public leogon::IBaseEvent
{
private:
	~CNetEvent(void);
public:
	CNetEvent();
	void Set(ISocketBase* pConnection, char* pPacket, int nPacketLen);

	virtual void Process();
	virtual void Release();

	char* GetData(){ return m_pPacketBuffer; }
	int GetLength(){ return m_nPacketLen; }
	void SetLength(int nLen) { m_nPacketLen = nLen; }
	void TransferData(int nTransferNum)
	{
		m_nPacketLen -= nTransferNum;
		memcpy(m_pPacketBuffer, &m_pPacketBuffer[nTransferNum], m_nPacketLen);
	}
private:
	char m_pPacketBuffer[MAX_PACKET_LEN];
	int m_nPacketLen;
	ISocketBase* m_pConnection;
};

class CNetUDPEvent : public leogon::IBaseEvent
{
private:
	~CNetUDPEvent(void){}
public:
	CNetUDPEvent(ISocketBase* pConnection, char* pPacket, int nPacketLen, struct sockaddr* peeraddr);
	virtual void Process();
	virtual void Release();
private:
	char m_pPacketBuffer[MAX_PACKET_LEN];
	int m_nPacketLen;
	ISocketBase* m_pConnection ;
	struct sockaddr m_stPeerAddr;
};
#endif
