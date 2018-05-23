/************************************************************************/
/* CopyRight @ 2010 by Anyone. No rights is reserved.
/* @Author: ZUHD
/* @Date: 11/12/2010
/* @Description:   网络模块定义
/************************************************************************/
#ifndef ISOCKETSYSTEM_H
#define ISOCKETSYSTEM_H

class IPacketParser
{
public:
	virtual ~IPacketParser(){}
	/*
	 *	@Param: NULL
	 *	@Return: 打包后数据实际长度
	 *	@Description: [in]最大数据存放空间，[out]打包后数据实际长度
	 */
	virtual int EncodePacket(const char* pInData, int nInLen, char* pOutData, int& nOutLen) = 0;
	/*
	 *	@Param: NULL
	 *	@Return: 返回本次解包操作实际消耗的网络数据大小, 0通常表示还不能凑成完成包
	 *	@Description: [in]数据最大空间,[out]解包后数据实际长度,0表示无有效数据包
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
	 *	@Description: 释放资源
	 */
	virtual void Release() = 0;	
	/*
	 *	@Param: NULL
	 *	@Return: NULL
	 *	@Description: 加载
	 */
	virtual int Run(int nLimit = 1) = 0;		
	/*
	 *	@Param: NULL
	 *	@Return: NULL
	 *	@Description: 创建监听器
	 */
	virtual IListener* CreateListener() = 0;
	/*
	 *	@Param: NULL
	 *	@Return: NULL
	 *	@Description: 创建连接器
	 */
	virtual IConnector* CreateConnector(int nType = 0) = 0;
	/*
	 *	@Param: nType 0, UDP消息不进主队列, 1 UDP消息进主队列处理 
	 *	@Return: NULL
	 *	@Description: 创建UDP通迅
	 */
	virtual IUdpPeer* CreateUdpPeer(int nType = 0) = 0;
};

#endif
