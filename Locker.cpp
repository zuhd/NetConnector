#include "Locker.h"
namespace leogon
{
	CLocker::CLocker()
	{
		p_mutex_init(&m_locker);
	}
	CLocker::~CLocker()
	{
		p_mutex_destroy(&m_locker);
	}
	///进入临界区
	void CLocker::Enter()
	{
		p_mutex_lock(&m_locker);
	}
	///退出临界区
	void CLocker::Leave()
	{
		p_mutex_unlock(&m_locker);
	}
}
