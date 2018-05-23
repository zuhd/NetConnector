/************************************************************************/
/* CopyRight @ 2010 by Anyone. No rights is reserved.
/* @Author: ZUHD
/* @Date: 11/11/2010
/* @Description:   安全线程类
/************************************************************************/
#ifndef THREAD_H
#define THREAD_H
#include "System.h"
namespace leogon
{
	class CThread
	{
	public:
		CThread(void);
		virtual ~CThread(void);
		int StartThread();
		void StopThread();
	protected:
		virtual void Run() = 0;
		static p_thread_retval_t P_THREAD_PROC Threadproc(void* pArg);
	protected:
		bool m_bStarted;
		p_thread_t m_pThread;
	};
}
#endif
