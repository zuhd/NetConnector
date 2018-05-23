#include "System.h"

extern int p_thread_create(p_thread_t * thread, p_thread_proc start_routine, p_thread_arg_t arg)
{
#ifdef WIN32
	unsigned int dwThreadId;

	*thread = (HANDLE)_beginthreadex(NULL, 0, start_routine, arg, 0, &dwThreadId);
	if (!*thread) return -1; 
	else return 0;
#else
	return pthread_create(thread, NULL, start_routine, arg);
#endif
}
extern int p_wait_thread_exit(p_thread_t * thread, int timeout)
{
#ifdef WIN32
	if( NULL != *thread )
	{
		WaitForSingleObject(*thread, timeout);
		CloseHandle(*thread);
		*thread = NULL;
	}
	return 0;
#else
	pthread_join(*thread, 0);
	return 0;
#endif
}
