/************************************************************************/
/* CopyRight @ 2010 by Anyone. No rights is reserved.
/* @Author: ZUHD
/* @Date: 11/11/2010
/* @Description:   事件队列，网络事件，DB事件
/************************************************************************/
#ifndef EVENTQUEUE_H
#define EVENTQUEUE_H
#include "IBaseEvent.h"
#include "Locker.h"
#include "GlobalDefine.h"
#include "Log.h"
#include <queue>
namespace leogon
{
	template<class T, class LOCKER = CLocker>
	class CEventQueue
	{
	public:
		CEventQueue(void){}
		~CEventQueue(void){}
		bool PushEvent(T pEvent)
		{
			CLockGuard<LOCKER> locker(m_QueLocker);
			m_EventQue.push(pEvent);
			return true;
		}
		T PopEvent()
		{
			T pEvent = 0;
			if(!m_EventQue.empty())
			{
				CLockGuard<LOCKER> locker(m_QueLocker);
				if(!m_EventQue.empty())
				{
					pEvent = m_EventQue.front();
					m_EventQue.pop();
				}
			}
			return pEvent;
		}
		T GetFront()
		{
			T pEvent = 0;
			if(!m_EventQue.empty())
			{
				CLockGuard<LOCKER> locker(m_QueLocker);
				if(!m_EventQue.empty())
				{
					pEvent = m_EventQue.front();
				}
			}
			return pEvent;
		}
		bool IsEmpty()
		{
			CLockGuard<LOCKER> locker(m_QueLocker);
			return m_EventQue.empty();
		}
		size_t Size()
		{
			CLockGuard<LOCKER> locker(m_QueLocker);
			return m_EventQue.size();
		}
	private:
		std::queue<T> m_EventQue;
		LOCKER m_QueLocker;
	};
}
#endif
