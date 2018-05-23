#include "./SelectWork.h"
#include "SocketBase.h"
#include <string.h>

CSelectWork::CSelectWork(void)
{
	m_arrEvent = NULL;
	m_nCurSockNum = 0;
	m_nMaxSockNum = 0;
}

CSelectWork::~CSelectWork(void)
{
	if(m_arrEvent != NULL)
	{
		delete[] m_arrEvent;
	}
}
void CSelectWork::PushEvent(leogon::IBaseEvent* pEvent)
{
	m_EventQueue.PushEvent(pEvent);
}
bool CSelectWork::Init(int nMaxNum)
{
	m_nMaxSockNum = nMaxNum;
	m_nCurSockNum = 0;
	FD_ZERO(&m_fdWriteSet);
	FD_ZERO(&m_fdReadSet);
	FD_ZERO(&m_fdExceptSet);
	m_arrEvent = new select_event[m_nMaxSockNum];
	for(int i = 0; i < m_nMaxSockNum; ++i)
	{
		m_arrEvent[i].fd = 0;
		m_arrEvent[i].ptr = 0;
	}
	return true;
}

int CSelectWork::AddSocket(LG_SOCKET fd, int events, void* key)
{
	if(m_nCurSockNum >= m_nMaxSockNum)
		return -1;
	m_arrEvent[m_nCurSockNum].fd = fd;
	m_arrEvent[m_nCurSockNum].events = events;
	m_arrEvent[m_nCurSockNum].ptr = key;
	++m_nCurSockNum;
	return 0;
}
int CSelectWork::DelSocket(LG_SOCKET fd)
{
	if(m_nCurSockNum == 0)
		return -1;
	// �����������ǰ�ƶ�
	for(int i = 0; i < m_nCurSockNum; ++i)
	{
		if(m_arrEvent[i].fd == fd)
		{
			if(m_nCurSockNum - i > 1)
				memcpy(&m_arrEvent[i], &m_arrEvent[i + 1], sizeof(select_event) * (m_nCurSockNum - i - 1));
			m_arrEvent[m_nCurSockNum - 1].fd = 0;
			--m_nCurSockNum;
			return 0;
		}
	}
	return -1;
}
int CSelectWork::ModifySocket(LG_SOCKET fd, int events, void* key)
{
	for(int i = 0; i < m_nCurSockNum; ++i)
	{
		if(m_arrEvent[i].fd == fd)
		{
			m_arrEvent[i].events = events;
			m_arrEvent[i].ptr = key;
			return 0;
		}
	}
	return -1;
}
void CSelectWork::Run()
{
	leogon::IBaseEvent* pEvent = NULL;
	pEvent = m_EventQueue.PopEvent();
	if(pEvent != NULL)
	{
		pEvent->Process();
		pEvent->Release();
	}

	struct timeval tv;
	tv.tv_sec = 0;
	tv.tv_usec = 1;
	FD_ZERO(&m_fdReadSet);
	FD_ZERO(&m_fdWriteSet);
	FD_ZERO(&m_fdExceptSet);
	int nMaxfd = -1;
	int nSelectSize = 0; 
	for(int i = 0; i < m_nCurSockNum; ++i)
	{
		if(m_arrEvent[i].fd != 0)
		{
			ISocketBase* pBase = (ISocketBase*)m_arrEvent[i].ptr;
			if(m_arrEvent[i].events & eReadEvent)
				FD_SET(m_arrEvent[i].fd, &m_fdReadSet);
			if(m_arrEvent[i].events & eWriteEvent)
				FD_SET(m_arrEvent[i].fd, &m_fdWriteSet);
			if(m_arrEvent[i].events & eExceptEvent)
				FD_SET(m_arrEvent[i].fd, &m_fdExceptSet);

			nMaxfd = m_arrEvent[i].fd > nMaxfd ? m_arrEvent[i].fd : nMaxfd;
		}
	}
#ifdef WIN32
	nSelectSize = m_nCurSockNum;
#else
	nSelectSize = nMaxfd + 1;
#endif
	int nRet = select(nSelectSize, &m_fdReadSet, &m_fdWriteSet, &m_fdExceptSet, &tv);
	if(nRet >= 0)
	{
		for(int i = 0; i < m_nCurSockNum; ++i)
		{
			if(m_arrEvent[i].fd != 0)
			{
				glog.debug("%d", m_arrEvent[i].fd);
				ISocketBase* pBase = (ISocketBase*)m_arrEvent[i].ptr;

				if(FD_ISSET(m_arrEvent[i].fd, &m_fdReadSet))
				{
					pBase->OnRecv();
				}
				if(FD_ISSET(m_arrEvent[i].fd, &m_fdWriteSet))
				{
					pBase->OnSend(); 
				}
				if(FD_ISSET(m_arrEvent[i].fd, &m_fdExceptSet))
				{
					pBase->OnDisConnect();
				}

			}
		}
	}
	else
	{
		//glog.debug("CSelectWork::Run(), err:%d", GetLastError());
	}
}
