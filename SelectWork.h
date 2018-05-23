#ifndef SELECTWORK_H_
#define SELECTWORK_H_
#include "System.h"
#include "GlobalDefine.h"
#include "IBaseEvent.h"
#include "EventQueue.h"

class CSelectWork
{
	struct select_event
	{
		uint events;
		LG_SOCKET fd;
		void* ptr;
	};
public:
	enum
	{
		eReadEvent = (1 << 0),
		eWriteEvent = (1 << 1),
		eExceptEvent = (1 << 2),
	};

public:
	CSelectWork(void);
	~CSelectWork(void);
	void Run();
	bool Init(int nMaxNum = 60);
	int AddSocket(LG_SOCKET fd, int events, void* key);
	int DelSocket(LG_SOCKET fd);
	int ModifySocket(LG_SOCKET fd, int events, void* key);
	void PushEvent(leogon::IBaseEvent* pEvent);

private:
	fd_set m_fdReadSet;
	fd_set m_fdWriteSet;
	fd_set m_fdExceptSet;
	select_event* m_arrEvent;
	int m_nMaxSockNum;
	int m_nCurSockNum;
	leogon::CEventQueue<leogon::IBaseEvent*> m_EventQueue;
};
#endif