#ifndef NETTHREAD_H_
#define NETTHREAD_H_
#include "Thread.h"
class CSession;
class CNetThread : public leogon::CThread
{
public:
	CNetThread(CSession* pSession);
	virtual ~CNetThread();
	virtual void Run();

private:
	CSession* m_pSession;
};
#endif