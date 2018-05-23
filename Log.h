/************************************************************************/
/* CopyRight @ 2010 by Anyone. No rights is reserved.
/* @Author: ZUHD
/* @Date: 11/11/2010
/* @Description:   »’÷æ¿‡
/************************************************************************/
#ifndef LOG_H
#define LOG_H
#include <limits.h>
#include <stdio.h>
#ifndef WIN32
#include <android/log.h>
#define  LOG_TAG    "test===="
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)

#define LOGE(...)  __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

#define LOGD(...)  __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#else
#define  LOGI(...)  {}

#define LOGE(...)   {}

#define LOGD(...)   {}
#endif
namespace leogon
{
#define LG_MAX_PATH 256
	class CLog
	{
	public:
		CLog(const char* szFileName);
		~CLog(void);
		void log(const char* fmt, ...);
		void syslog(const char* fmt, ...);
//#if defined DEBUG || defined _DEBUG
//		void debug(const char* fmt, ...);
//#else
		void debug(const char* fmt, ...);
//#endif
	private:
		char m_szFileName[LG_MAX_PATH];
		FILE* m_file;
	};
}
extern leogon::CLog glog;
#endif
