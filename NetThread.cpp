#include "NetThread.h"
#include "SocketSystem.h"
#include "GlobalDefine.h"
#include "Utilty.h"
#include "Session.h"

CNetThread::CNetThread(CSession* pSession)
{
	m_pSession = pSession;
}

CNetThread::~CNetThread()
{

}

void CNetThread::Run()
{
	uint uiLastTime = leogon::GetCurTime();
	uint uiCurTime;
	int uiElapse = 0;
	
	while(true)
	{
		uiCurTime = leogon::GetCurTime();		
		uiElapse = uiCurTime - uiLastTime;

		bool bIdle = true;
		
		if (m_pSession != NULL)
		{
			ISocketSystem* pSocketSystem = m_pSession->GetSocketSystem();
			if (pSocketSystem != NULL)
			{
				if (m_pSession->GetSocketSystem()->Run())
				{
					bIdle = false;
				}
			}
			
			if (uiElapse >= 3 * 60 * 1000)
			{
				m_pSession->HeartBit();
				uiLastTime = uiCurTime;
			}
		}
		
		if (bIdle)
			p_sleep(50);
	}
}

