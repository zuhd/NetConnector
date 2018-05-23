/************************************************************************/
/* CopyRight @ 2010 by Anyone. No rights is reserved.
/* @Author: ZUHD
/* @Date: 11/11/2010
/* @Description:   ��ȫ�߳���
/************************************************************************/
#ifndef LOCKER_H
#define LOCKER_H
#include "System.h"
namespace leogon
{
	class CLocker
	{
	public:
		CLocker();
		~CLocker();
		///�����ٽ���
		void Enter();
		///�˳��ٽ���
		void Leave();
		//int TryEnter();
	private:
		p_mutex_t m_locker;
	};

	class CEmptyLocker
	{
	public:
		CEmptyLocker(void){}
		~CEmptyLocker(void){}
		///�����ٽ���
		void Enter(){}
		///�˳��ٽ���
		void Leave(){}

	};

	template<class T>
	class CLockGuard
	{
	public:
		CLockGuard(T& locker) : m_locker(locker)
		{
			m_locker.Enter();
		}
		~CLockGuard()
		{
			m_locker.Leave();
		}
	private:
		T& m_locker;
	};
}

#endif
