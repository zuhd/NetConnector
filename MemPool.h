/************************************************************************/
/* CopyRight @ 2010 by Anyone. No rights is reserved.
/* @Author: ZUHD
/* @Date: 11/11/2010
/* @Description:   �ڴ����
/************************************************************************/
#ifndef MEMPOOL_H
#define MEMPOOL_H
#include <exception>
#include "Locker.h"
#include <new>
namespace leogon
{
//	template<class T, int nMaxObject = 1000>
//	class CMemPool
//	{
//	public:
//		CMemPool(void)
//		{
//			_bActive = true;
//		}
//		virtual ~CMemPool(void)
//		{
//			_bActive = false;
//		}
//		bool IsActive()
//        {
//            return _bActive; 
//        }
////#ifndef _DEBUG
//		static void* operator new(size_t size)
//		{
//			if(size == 0)
//			{
//				size = 1;
//			}
//			if(size != sizeof(T))
//			{
//				return ::operator new(size);
//			}
//			T* p = NULL;
//
//
//			PoolLocker.Enter();
//			if(list_head == list_end)
//			{
//				getmemorypool();
//			}
//			p = list_head;
//			list_head = list_head->next;
//			PoolLocker.Leave();
//			return p;
//		}
//		static void operator delete(void* memory, size_t size)
//		{
//			if(size != sizeof(T))
//			{
//				return ::operator delete(memory);
//			}
//			T* t = (T*)memory;
//
//			PoolLocker.Enter();
//            list_end->next = t;
//            list_end = t;
//            list_end->next = 0;
//			PoolLocker.Leave();
//
//
//		}
////#endif
//		static void getmemorypool()
//		{
//			T* p = (T*)malloc(sizeof(T) * nMaxObject);
//			if(p == 0)
//			{
//				throw std::bad_alloc();
//			}
//			for(int i = 0; i < nMaxObject - 1; ++i)
//			{
//				p[i].next = &p[i + 1];
//			}
//			p[nMaxObject - 1].next = 0;
//            if(list_head == 0)
//            {
//                list_head = &p[0];
//            }
//            else
//            {
//                list_head->next = &p[0];
//            }
//            list_end = &p[nMaxObject - 1];
//		}
//	private:
//		T*				next;
//		bool				_bActive;
//		static T*		list_head;
//        static T*       list_end;
//		static CLocker	PoolLocker;
//	};
//	template<class T, int nMaxObject>
//		T* CMemPool<T, nMaxObject>::list_head = 0;
//    template<class T, int nMaxObject>
//        T* CMemPool<T, nMaxObject>::list_end = 0;
//	template<class T, int nMaxObject>
//		CLocker CMemPool<T, nMaxObject>::PoolLocker;

	// ��������  #define  __USE_CUSTOM_MEMPOOL__
	// ���û�����������, ��Ƕ��������еĶ��嶼��Ч
	// �����ڴ�ķ���, �������ڶ��ؼ̳е���
	template<class PARENTCLASS>
	class CMemPool
	{
	private:
		struct __BufferInfo
		{
			void *pBuffer;			 //��ǰBuffer���׵�ַ
			__BufferInfo* pNext;     //��һ__BufferInfor��λ��
		};

		int m_nBytesParent;			//������ֽ���
		int m_nCountOnce;			//һ�η���Ķ������
		void *m_pFreeNode;			//�����ڴ���ָ��, ָ����һ�����е��ڴ��
		__BufferInfo *m_pBuffInfoList;  //ÿ�η�����ڴ涼��¼�ڴ�, �Ա��ͷ�ʱ�п���delete    

	public:

		CMemPool(int nCountOnce)  //ÿһ�η���ĸ���
		{
			m_nBytesParent = sizeof(PARENTCLASS);			
			if (m_nBytesParent < 4) 
			{
				m_nBytesParent = 4;
			}

			m_nCountOnce = nCountOnce;
			m_pFreeNode = NULL;
			m_pBuffInfoList = NULL;
		}

		~CMemPool()
		{
			while(m_pBuffInfoList)
			{
				__BufferInfo *pDeleteBuffer = m_pBuffInfoList;
				m_pBuffInfoList = m_pBuffInfoList->pNext;
				delete[] pDeleteBuffer->pBuffer;
				delete pDeleteBuffer;
			}
		}

		void *Alloc()
		{
			if (m_pFreeNode == NULL)
			{
				//�����ڴ�
				__BufferInfo *pNewBuffer = new __BufferInfo;
				pNewBuffer->pBuffer = new char[m_nBytesParent * m_nCountOnce];
				pNewBuffer->pNext = m_pBuffInfoList;
				m_pBuffInfoList = pNewBuffer;

				PARENTCLASS *pTemp = (PARENTCLASS *)pNewBuffer->pBuffer;
				for (int i = 0; i < m_nCountOnce; ++i)
				{
					if (i != m_nCountOnce - 1)
					{
						*(unsigned long*)(pTemp + i) = (unsigned long)(pTemp + i + 1);
					}
					else
					{
						*(unsigned long*)(pTemp + i) = NULL;
					}
				}
				m_pFreeNode = pNewBuffer->pBuffer;
			}


			void *bReturn = m_pFreeNode;
			m_pFreeNode = (void*)(*(unsigned long*)m_pFreeNode);  //ָ����һ��Ԫ
			return bReturn;
		}

		void Release(void *pDelete)
		{
			if (pDelete)
			{
				*(unsigned long*)pDelete = (unsigned long)m_pFreeNode;
				m_pFreeNode = pDelete;
			}		
		}

		int GetTotalSize()
		{
			int nSize = 0;
			__BufferInfo *pTemp = m_pBuffInfoList;
			while(pTemp)
			{
				nSize++;
				pTemp = pTemp->pNext;
			}

			return nSize * m_nBytesParent * m_nCountOnce;
		} 
	};

#ifdef __USE_CUSTOM_MEMPOOL__

#define  ALLOCFROM_CUSTOM_MEMPOOL_DEF(CLASS) public:\
	static CMemPool<CLASS> s_MemPool;\
	void *operator new(size_t size){return s_MemPool.Alloc();};\
	void operator delete(void *pMem){s_MemPool.Release(pMem);};\

#define  ALLOCFROM_CUSTOM_MEMPOOL_IMP(CLASS, INIT_SIZE)  CMemPool<CLASS> CLASS::s_MemPool(INIT_SIZE);

#else
#define  ALLOCFROM_CUSTOM_MEMPOOL_DEF(CLASS)              ;
#define  ALLOCFROM_CUSTOM_MEMPOOL_IMP(CLASS, INIT_SIZE)    ;

#endif
}
#endif
