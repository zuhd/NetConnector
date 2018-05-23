# NetConnector
## 1，TCP and UDP Support with select module
## 2，windows/linux/android support
## 3，thread safe

# 客户端连接器
## 1，通用的客户端连接器，支持TCP & UDP，select模型
## 2，支持windows/linux/android
## 3，线程安全
## 4，作者微信：zuuuhd

# TCP Sample
```C++
void StartConnect()
{
	if (m_pSocketSystem == NULL)
	{
		return;
	}
	m_pConnector = m_pSocketSystem->CreateConnector(socket_module_select);
	
	if (m_pConnector == NULL)
	{
		return;
	}
	m_pConnector->SetSession(this);
	m_pConnector->Connect(g_strHost, g_dwPort);
}
```

# UDP Sample
```C++
void StartUDP()
{
	if (m_pSocketSystem == NULL)
	{
		return;
	}
	InitUDPRemoteAddr(g_strHost, g_dwPort);
	m_pUDPPeer = m_pSocketSystem->CreateUdpPeer(socket_module_select);

	if (m_pUDPPeer == NULL)
	{
		return;
	}
	m_pUDPPeer->SetSession(this);
	m_pUDPPeer->SetPacketParser(NULL);
	m_pUDPPeer->BindHostAddr("192.168.10.175", 8122);
}
```
