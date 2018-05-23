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
	///�����ٽ���
	void CLocker::Enter()
	{
		p_mutex_lock(&m_locker);
	}
	///�˳��ٽ���
	void CLocker::Leave()
	{
		p_mutex_unlock(&m_locker);
	}
}
