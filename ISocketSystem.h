/************************************************************************/
/* CopyRight @ 2010 by Anyone. No rights is reserved.
/* @Author: ZUHD
/* @Date: 11/12/2010
/* @Description:   ����ģ�鶨��
/************************************************************************/
#ifndef ISOCKETSYSTEM_H
#define ISOCKETSYSTEM_H

class IPacketParser
{
public:
	virtual ~IPacketParser(){}
	/*
	 *	@Param: NULL
	 *	@Return: ���������ʵ�ʳ���
	 *	@Description: [in]������ݴ�ſռ䣬[out]���������ʵ�ʳ���
	 */
	virtual int EncodePacket(const char* pInData, int nInLen, char* pOutData, int& nOutLen) = 0;
	/*
	 *	@Param: NULL
	 *	@Return: ���ر��ν������ʵ�����ĵ��������ݴ�С, 0ͨ����ʾ�����ܴճ���ɰ�
	 *	@Description: [in]�������ռ�,[out]���������ʵ�ʳ���,0��ʾ����Ч���ݰ�
	 */
	virtual int DecodePacket(const char* pInData, int nInLen, char* pOutData, int& nMaxLen) = 0;	
};


class ISession;
class IConnection
{
public:
	virtual ~IConnection(){}
	virtual void ReConnect() = 0;
	virtual bool IsConnected() = 0;
	virtual void CloseConnection() = 0;
	virtual void SendTo(struct sockaddr* pRemoteAddr, const char* pData, int nLen) = 0;
	virtual void Send(const char* pData, int nLen) = 0;	
	virtual void GetPeerAddr(struct sockaddr* pRemoteAddr) = 0;
	virtual void SetSession(ISession* pSession) = 0;	
};


class ISession
{
public:
	virtual ~ISession(){}
	virtual void Release() = 0;	
	virtual void OnConnection(IConnection* pConnection) = 0;
	virtual void OnDisConnect() = 0;
	virtual void OnDisConnection() = 0;
	virtual void OnRecv(const char* pData, int nLen) = 0;
	virtual void OnRecvFrom(struct sockaddr* pRemoteAddr, const char* pData, int nLen) = 0;	
};


class ISessionManager
{
public:
	virtual ~ISessionManager(){}
	virtual bool OnPreAccept(struct sockaddr* pRemoteAddr) = 0;
	virtual ISession* OnCreateSession() = 0;	
};


class IListener
{
public:
	virtual ~IListener(){}
	virtual void Release() = 0;	
	virtual bool StartListen(const char* szListenAddr, int nPort) = 0;
	virtual void StopListen() = 0;
	virtual void SetPacketParser(IPacketParser* pParser) = 0;
	virtual void SetSessionManager(ISessionManager* pSessionManager) = 0;	
};


class IConnector
{
public:	
	virtual ~IConnector(){}
	virtual void Release() = 0;
	virtual void Connect(const char* szRemoteAddr, int nPort, int nProxyType = 0, 
		const char* szProxyAddr = 0, int nProxyPort = 0,
		const char* szProxyUser = 0, const char* szProxyPwd = 0) = 0;
	virtual void SetSession(ISession* pSession) = 0;
	virtual void SetPacketParser(IPacketParser* pParser) = 0;	
};

class IUdpPeer
{
public:
	virtual ~IUdpPeer(){}
	virtual void Release() = 0;
	virtual bool BindHostAddr(const char* szHostAddr, int nPort) = 0;	
	virtual void SetSession(ISession* pSession) = 0;
	virtual void SetPacketParser(IPacketParser* pParser) = 0;	
};

class ISocketSystem
{
public:	
	/*
	 *	@Param: NULL
	 *	@Return: NULL
	 *	@Description: 
	 */
	virtual ~ISocketSystem(){}
	/*
	 *	@Param: NULL
	 *	@Return: NULL
	 *	@Description: �ͷ���Դ
	 */
	virtual void Release() = 0;	
	/*
	 *	@Param: NULL
	 *	@Return: NULL
	 *	@Description: ����
	 */
	virtual int Run(int nLimit = 1) = 0;		
	/*
	 *	@Param: NULL
	 *	@Return: NULL
	 *	@Description: ����������
	 */
	virtual IListener* CreateListener() = 0;
	/*
	 *	@Param: NULL
	 *	@Return: NULL
	 *	@Description: ����������
	 */
	virtual IConnector* CreateConnector(int nType = 0) = 0;
	/*
	 *	@Param: nType 0, UDP��Ϣ����������, 1 UDP��Ϣ�������д��� 
	 *	@Return: NULL
	 *	@Description: ����UDPͨѸ
	 */
	virtual IUdpPeer* CreateUdpPeer(int nType = 0) = 0;
};

#endif
