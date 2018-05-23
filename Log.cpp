#include "Log.h"
#include <stdarg.h>
#include <time.h>
#include <string.h>
#ifndef WIN32
#include <syslog.h>
#include <sys/types.h>
#include <sys/stat.h>

#else

#endif
#define TIME_FORMAT "%b%d %H:%M:%S"
#define TIME_MAXLEN 32
leogon::CLog glog(NULL);
namespace leogon
{
	CLog::CLog(const char* szFileName)
	{
		m_file = NULL;
#ifdef WIN32
		if(NULL == szFileName)
		{
			time_t now;
			struct tm* tmnow;
			time(&now);
			tmnow = localtime(&now);
			strftime(m_szFileName, LG_MAX_PATH, "%m_%d_%H_%M.log", tmnow);
		}
		else
		{
			strncpy(m_szFileName, szFileName, sizeof(m_szFileName));

		}
		//m_file = fopen(m_szFileName, "a");
#else
		umask(0);
#ifdef DEBUG
		m_file = stdout;
#endif
#endif
	}

	CLog::~CLog(void)
	{
		if(m_file)
		{
			fclose(m_file);
		}
	}

	void CLog::syslog(const char* fmt, ...)
	{
#ifndef WIN32
		//va_list args;
		//char szbuffer[512];
		//va_start(args, fmt);
		//vsnprintf(szbuffer, sizeof(szbuffer), fmt, args);
		//va_end(args);
		//::syslog(LOG_INFO|LOG_USER, szbuffer);
		return;
#endif
	}

	void CLog::log(const char* fmt, ...)
	{		
#if defined WIN32 || defined DEBUG || defined _DEBUG		
		va_list args;
		char	time_string[TIME_MAXLEN];
		struct tm*	tmnow;
		time_t		now;
		if(!m_file)
		{
			m_file = fopen(m_szFileName, "a");
		}

		time(&now);
		if(!(tmnow = localtime(&now)))
		{
			strcpy(time_string , "?");
		}
		else
		{
			strftime(time_string, TIME_MAXLEN, TIME_FORMAT, tmnow);
		}
		if(!fmt)
		{
			fprintf(m_file, "%s [error] log::log got NULL fmt\n", time_string);
			fflush(m_file);
			return ;
		}		
		fprintf(m_file,"%s: ", time_string);
		va_start(args, fmt);
		vfprintf(m_file, fmt, args);
		va_end(args);

		fprintf(m_file, "\n");
		fflush(m_file);

#else
		//va_list args;
		//char szbuffer[512];
		//va_start(args, fmt);
		//vsnprintf(szbuffer, sizeof(szbuffer), fmt, args);
		//va_end(args);

	//	syslog(LOG_INFO|LOG_USER, szbuffer);


		return;

#endif

	}

#if defined DEBUG || defined _DEBUG

	void CLog::debug(const char* fmt, ...)
	{
		/*va_list args;
		char	time_string[TIME_MAXLEN];
		struct tm*	tmnow;
		time_t		now;
		if(!m_file) return ;
		time(&now);
		if(!(tmnow = localtime(&now)))
		{
			strcpy(time_string , "?");
		}
		else
		{
			strftime(time_string, TIME_MAXLEN, TIME_FORMAT, tmnow);
		}
		if(!fmt)
		{
			fprintf(m_file, "%s [error] log::log got NULL fmt\n", time_string);
			fflush(m_file);
			return ;
		}
		fprintf(m_file,"%s: ", time_string);
		va_start(args, fmt);
		vfprintf(m_file, fmt, args);
		va_end(args);

		fprintf(m_file, "\n");
		fflush(m_file);*/
	}
#else
	void CLog::debug(const char* fmt, ...)
	{
	}
#endif
}

