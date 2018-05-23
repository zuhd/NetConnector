#include "./SocketSystem.h"
#include "./NetConnector.h"
#include "./SelectWork.h"
#include "./UdpPeer.h"

CSocketSystem::CSocketSystem(void)
{
	static CSocketPrepare prepare;
	m_pSelectWork = new CSelectWork();
	if (m_pSelectWork != NULL)
	{
		m_pSelectWork->Init();
	}
}

CSocketSystem::~CSocketSystem(void)
{
}

void CSocketSystem::Release()
{	
	SAFE_DELETE(m_pSelectWork);
	delete this;
}

int CSocketSystem::Run(int nLimit)
{
	if (m_pSelectWork != NULL)
	{
		m_pSelectWork->Run();
	}
	return 0;
}

IListener* CSocketSystem::CreateListener()
{
	return NULL;
}

IConnector* CSocketSystem::CreateConnector(int nType)
{
	return new CNetConnector(nType, this);
}

IUdpPeer* CSocketSystem::CreateUdpPeer(int nType)
{
	return new CUdpPeer(nType, this);
}