/************************************************************************/
/* CopyRight @ 2010 by Anyone. No rights is reserved.
/* @Author: ZUHD
/* @Date: 11/11/2010
/* @Description:   对象池的实现
/************************************************************************/
#ifndef OBJECTPOOL_H
#define OBJECTPOOL_H
#include "./EventQueue.h"
#include "./Log.h"
namespace leogon
{
	template<class T, int MAXNUM, class LOCKER = CEmptyLocker>
	class CObjectPool
	{
	public:
		CObjectPool(void)
		{
			for(int i = 0; i <= 20; ++i)
			{
				m_quFree.PushEvent(new T());
			}
		}
		~CObjectPool()
		{
			T* p = NULL;
			while(!m_quFree.IsEmpty())
			{
				p = m_quFree.PopEvent();
				delete p;
			}
		}
		T* PopObject()
		{
			T* p = NULL;
			if(!m_quFree.IsEmpty())
			{
				p = m_quFree.PopEvent();
			}
			if(p)
				return p;
			return new T();
		}
		void PushObject(T* pObject)
		{
			m_quFree.PushEvent(pObject);
		}
		int GetSize()
		{
			return m_quFree.Size();
		}
	private:
		leogon::CEventQueue<T*, LOCKER> m_quFree;
	};
	//template<class T, int MAXNUM, class LOCKER = CEmptyLocker>
	//class objectpool
	//{
	//	struct objectwrapp
	//	{
	//		T object;
	//		uint index;
	//	};
	//public:
	//	objectpool(void)
	//	{
	//		for(int i = 1; i <= MAXNUM; ++i)
	//		{
	//			m_arryObject[i].index = i;
	//			m_quFree.PushEvent(i);
	//		}
	//	}
	//	~objectpool(void)
	//	{
	//		glog.log("objectpool destroy [free num:%d][%d:byte]", m_quFree.Size(), sizeof(T) * MAXNUM);
	//	}
	//	T* PopObject()
	//	{
	//		int nTry = 0;
	//		while(++nTry < 100)
	//		{
	//			uint index = m_quFree.PopEvent();
	//			//glog.debug("objectpool::PopObject(), leave num[%d]", m_quFree.Size());
	//			if(index <= 0 || index >  MAXNUM)
	//			{
	//				struct timespec tv;	
	//				tv.tv_sec = 0;	
	//				tv.tv_nsec = nTry * 5;	
	//				nanosleep(&tv, NULL);
	//				//struct timeval tv;
	//				//tv.tv_sec = 0;
	//				//tv.tv_usec = 10;
	//				//select(0, 0, 0, 0, &tv);
	//				glog.log("objectpool::PopObject(), index[%d]", index);
	//				glog.log("objectpool::PopObject(), Queue Size[%d]", m_quFree.Size());
	//				//assert(index > 0 && index <=  MAXNUM);
	//				//fprintf(stdout, "foobar");
	//				//usleep(1);
	//				continue;
	//			}
	//			else
	//			{

	//				return (T*)&m_arryObject[index];
	//			}
	//		}

	//		return NULL;

	//	}
	//	void PushObject(T* pObject)
	//	{
	//		//delete pObject;
	//		objectwrapp* p = (objectwrapp*)pObject;
	//		if(p->index <= 0 || p->index > MAXNUM)
	//		{
	//			glog.log("PushObject p->index=[%d]", p->index);
	//		}
	//		assert(p->index > 0 && p->index <= MAXNUM);
	//		m_quFree.PushEvent(p->index);
	//	}
	//	int GetSize()
	//	{
	//		return m_quFree.Size();
	//	}
	//private:
	//	objectwrapp m_arryObject[MAXNUM + 1];
	//	leaf::CBDEventQueue<uint, LOCKER> m_quFree;
	//};
}
#endif
