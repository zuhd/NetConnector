/************************************************************************/
/* CopyRight @ 2010 by Anyone. No rights is reserved.
/* @Author: ZUHD
/* @Date: 11/11/2010
/* @Description:   π§≥ß¿‡
/************************************************************************/
#ifndef FACTORY_H
#define FACTORY_H
#include "System.h"
#ifdef WIN32
#else
#include <dlfcn.h>
#include "Log.h"
#endif
#include <stdio.h>
namespace leogon
{
	class CFactory
	{
	public:
		CFactory(const char* szModuleFile)
		{
			if(szModuleFile != NULL)
			{
				init(szModuleFile);
			}
		}
		bool init(const char* szModuleFile)
		{
			m_pHandler = 0;
			m_pHandler = DYNLIB_LOAD(szModuleFile);
			if(m_pHandler == 0)
			{
#ifndef WIN32
//				glog.log("dlopen failed! : %s", dlerror());
#endif
				return false;
			}
			return true;
		}
		~CFactory(void)
		{
			if(m_pHandler!= 0)
			{
				DYNLIB_UNLOAD(m_pHandler);
			}
		}
		template<class T>
			T* createmodule(const char* szCreateFunction)
		{
			typedef	T* (*PFN)();
			PFN pfn = 0;
			T* pInterface = 0;
			if(m_pHandler != 0)
			{
				pfn = (PFN)DYNLIB_GETSYM(m_pHandler, szCreateFunction);
				if(pfn != 0)
				{
					pInterface = pfn();
				}
				else
				{
#ifndef WIN32
					glog.log("dlsym failed! :%s", dlerror());
#endif
				}
			}
			return pInterface;
		}
	private:
		DYNLIB_HANDLE m_pHandler;

	};
}
#endif

