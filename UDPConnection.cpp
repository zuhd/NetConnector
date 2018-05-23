#include "UDPConnection.h"
#include "SelectWork.h"
#include "SocketSystem.h"

CUDPConnection::CUDPConnection(CSocketSystem* pSocketSystem):
	m_pPacketParser(NULL)
{
	m_pSocketSystem = pSocketSystem;
}

CUDPConnection::~CUDPConnection(void)
{
}
void CUDPConnection::CloseConnection()
{
}
void CUDPConnection::SendTo(struct sockaddr* pRemoteAddr, const char* pData, int nLen)
{
	int nOutLen = nLen;
	char* pOutData = (char*)pData;	
	int nMaxLen = 0;
	char pBuffer[MAX_PACKET_LEN] = {0};
	if (m_pPacketParser != NULL)
	{		
		m_pPacketParser->EncodePacket(pData, nLen, pBuffer, nMaxLen);
		pOutData = pBuffer;
		nOutLen = nMaxLen;
	}

	if(0 > sendto(m_fdClient, pOutData, nOutLen, 0, pRemoteAddr, sizeof(sockaddr)))
	{
		glog.debug("CUDPConnection::SendTo, err= %d", strerror(errno));
	}
}
void CUDPConnection::SetSockPacketParser(IPacketParser* pParser)
{
	m_pPacketParser = pParser;
}
bool CUDPConnection::BindHostAddr(const char* szHostAddr, int nPort)
{
	struct sockaddr_in hostaddr;
	memset(&hostaddr, 0, sizeof(hostaddr));
	hostaddr.sin_family = AF_INET;
	hostaddr.sin_addr.s_addr = inet_addr(szHostAddr);
	hostaddr.sin_port = htons(nPort);
	m_fdClient = socket(AF_INET, SOCK_DGRAM, 0);
	SetNonBlocking(m_fdClient);
	SetReusePort(m_fdClient);
	if(0 != bind(m_fdClient, (struct sockaddr*)&hostaddr, sizeof(hostaddr)))
	{
		goto FAILED;
	}
	m_pSocketSystem->GetSelectWork()->AddSocket(m_fdClient, CSelectWork::eReadEvent, (void*)this);
	return true;
FAILED:
	glog.debug("CUDPConnection::BindHostAddr() err:%s",  strerror(errno));
	return false;
}

void CUDPConnection::OnRecv()
{
	memset(&m_stRemoteAddr, 0, sizeof(m_stRemoteAddr));
	int len = sizeof(m_stRemoteAddr);
	int nTransfer = recvfrom(m_fdClient, m_pRecvBuffer, MAX_PACKET_LEN, 0, &m_stRemoteAddr, &len);
	if(nTransfer < 0)
	{
		glog.debug("CUDPConnection::OnRecv, err= %d", strerror(errno));
	}	
	int nOutLen = nTransfer;
	char* pOutData = m_pRecvBuffer;
	int nMaxLen = 0;
	char pBuffer[MAX_PACKET_LEN] = {0};
	if (m_pPacketParser != NULL)
	{
		m_pPacketParser->DecodePacket(m_pRecvBuffer, nTransfer, pBuffer, nMaxLen);
		pOutData = pBuffer;
		nOutLen = nMaxLen;
	}
	m_pSession->OnRecvFrom(&m_stRemoteAddr, pOutData, nOutLen);

}
