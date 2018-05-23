/************************************************************************/
/* CopyRight @ 2010 by Anyone. No rights is reserved.
/* @Author: ZUHD
/* @Date: 11/11/2010
/* @Description:   跨平台的函数或变量定义 
/************************************************************************/
#ifndef SYSTEM_H
#define SYSTEM_H

typedef void *			p_thread_arg_t;
#ifdef WIN32
//#include <windows.h>
#include "WinInclude.h" 
#include <process.h>
#define P_THREAD_PROC		WINAPI
#define P_EXPORT_DLL		__declspec(dllexport)
#define p_thread_detach(thread)	(0)
#define p_thread_exit(retval)	ExitThread(retval)
#define p_thread_self()		GetCurrentThread()
#define p_thread_cancel(thread)	TerminateThread(thread, -1)
//#define p_thread_wait(thread) WaitForSingleObject(thread, INFINITE)
typedef HANDLE			p_thread_t;
typedef unsigned int	p_thread_retval_t;
typedef p_thread_retval_t  (P_THREAD_PROC * p_thread_proc)(p_thread_arg_t);

#define p_mutex_init(mutex)	InitializeCriticalSection(mutex)
#define p_mutex_lock(mutex)	EnterCriticalSection(mutex)
#define p_mutex_trylock(mutex)	TryEnterCriticalSection(mutex)
#define p_mutex_unlock(mutex)	LeaveCriticalSection(mutex)
#define p_mutex_destroy(mutex)	DeleteCriticalSection(mutex)
typedef CRITICAL_SECTION	p_mutex_t;


#define DYNLIB_HANDLE HINSTANCE
#define DYNLIB_LOAD( a ) LoadLibraryA( a )
#define DYNLIB_GETSYM( a, b ) GetProcAddress( a, b )
#define DYNLIB_UNLOAD( a ) (bool)!FreeLibrary( a )
#define p_sleep( a ) Sleep( a )

//#define vsnprintf _vsnprintf
#define snprintf _snprintf
typedef SOCKET LG_SOCKET;
#else /* WIN32 */

//#ifdef HAVE_PTHREAD_H
#include <pthread.h>
#include <unistd.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
//#endif

#define P_THREAD_PROC
#define p_thread_detach(thread)	 pthread_detach(thread)
#define p_thread_exit(retval)	pthreadn_exit(retval)
#define p_thread_self()		pthread_self()
#define p_thread_cancel(thread)	pthread_cancel(thread)
//#define p_thread_wait(thread, ) pthread_join(thread, INFINITE)
#define closesocket( fd ) close( fd )
#define p_sleep( a ) usleep( 1000 * a )

typedef pthread_t		p_thread_t;
typedef void *			p_thread_retval_t;
typedef p_thread_retval_t  (P_THREAD_PROC * p_thread_proc)(p_thread_arg_t);

#ifndef HAVE_PTHREAD_MUTEX_RECURSIVE_NP
# ifdef PTHREAD_MUTEX_RECURSIVE
#   define PTHREAD_MUTEX_RECURSIVE_NP PTHREAD_MUTEX_RECURSIVE
# else
#   define PTHREAD_MUTEX_RECURSIVE_NP 0
# endif
#endif

#ifdef HAVE_PTHREAD_MUTEXATTR_SETKIND_NP
/* ft, no defination found in pthread.h! */
extern int pthread_mutexattr_setkind_np(pthread_mutexattr_t *__attr, int __kind);
#  define p_mutexattr_settype(attr, kind)	pthread_mutexattr_setkind_np(attr, kind)
#else
#  ifdef HAVE_PTHREAD_MUTEXATTR_SETTYPE
#    define p_mutexattr_settype(attr, kind)	pthread_mutexattr_settype(attr, kind)
#  else
#    define p_mutexattr_settype(attr, kind) (0)
#  endif
#endif


/*
 * ??? shall I usr pthread_mutexattr_setkind_np ???
 * pthread_mutexattr_setkind_np(&_mutex_attr, PTHREAD_MUTEX_RECURSIVE_NP);
 */

#define p_mutex_init(mutex)\
	do {	\
		pthread_mutexattr_t     _mutex_attr;	\
		pthread_mutexattr_init(&_mutex_attr);	\
		p_mutexattr_settype(&_mutex_attr, PTHREAD_MUTEX_RECURSIVE_NP);	\
		pthread_mutex_init(mutex, &_mutex_attr);	\
		pthread_mutexattr_destroy(&_mutex_attr);	\
	} while(0);



#define p_mutex_lock(mutex)	pthread_mutex_lock(mutex)
#define p_mutex_trylock(mutex)	pthread_mutex_trylock(mutex)
#define p_mutex_unlock(mutex)	\
	do {	\
		pthread_mutex_unlock(mutex);	\
	}while(0);
#define p_mutex_destroy(mutex)	pthread_mutex_destroy(mutex)
typedef pthread_mutex_t		p_mutex_t;


#define DYNLIB_HANDLE void*
#define DYNLIB_LOAD( a ) dlopen( a, RTLD_NOW )
#define DYNLIB_GETSYM( a, b ) dlsym( a, b )
#define DYNLIB_UNLOAD( a ) dlclose( a )
typedef int LG_SOCKET;
#define P_EXPORT_DLL

#endif /* WIN32 */

extern int p_thread_create(p_thread_t * thread, p_thread_proc start_routine, p_thread_arg_t arg);
extern int p_wait_thread_exit(p_thread_t * thread, int timeout = 0xFFFFFFFF);
#endif
