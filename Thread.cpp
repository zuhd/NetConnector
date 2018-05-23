#include "Thread.h"
#include "Log.h"
namespace leogon
{
	CThread::CThread(void)
	{
		m_bStarted = false;
	}

	CThread::~CThread(void)
	{
	}
	int CThread::StartThread()
	{
		m_bStarted = true;
		int nret = p_thread_create(&m_pThread, Threadproc, (p_thread_arg_t)this);
		if(nret == 0)
		{
		}
		else
		{
			m_bStarted = false;
			glog.log("create_thread faild!");
		}
		return nret;
	}
	void CThread::StopThread()
	{
		if(m_bStarted)
		{
			m_bStarted = false;
			p_wait_thread_exit(&m_pThread);
		}
	}
	p_thread_retval_t CThread::Threadproc(void* pArg)
	{
		CThread* pp = reinterpret_cast<CThread*>(pArg);
		pp->Run();
		return 0;
	}
}
