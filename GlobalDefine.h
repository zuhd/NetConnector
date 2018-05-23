/************************************************************************/
/* CopyRight @ 2010 by Anyone. No rights is reserved.
/* @Author: ZUHD
/* @Date: 11/11/2010
/* @Description:   全局变量的定义
/************************************************************************/
#ifndef GLOBALDEFINE_H
#define GLOBALDEFINE_H
enum socket_module
{
	socket_module_epoll = 0,
	socket_module_select,
	socket_module_iocp
};

#define  SINGLETON(type) \
	static inline type* GetInstancePtr() \
	{ \
		static type _instance; \
		return &_instance; \
	}
#define MAX_PACKET_LEN 1024 * 2
//#define MAX_PACKET_LEN 1024 * 64
#define MAX_UDP_PACKET_LEN 1024 * 2

#define SAFE_DELETE(p)       { if(p) { delete (p);     (p)=NULL; } }
#define SAFE_DELETE_ARRAY(p) { if(p) { delete[] (p);   (p)=NULL; } }
#define SAFE_RELEASE(p)      { if(p) { (p)->Release(); (p)=NULL; } }
#define ARRAY(a)	sizeof(a)/sizeof(a[0])

// 服务器最大在线人数
#define MAX_PLAYER_CAPABILITY 1200
// 未加密之前的脚本不能大于48K 
#define MAX_FILESIZE 64 * 1024	
#ifdef WIN32
#define assert_log(exp) \
	if(!(exp))	\
{	\
	glog.log("%s:%d: %s:  Assertion '%s' failed ", __FILE__, __LINE__, __FUNCTION__, #exp);	\
	assert(exp);	\
}
#else
#define assert_log(exp) \
	if(!(exp))	\
{	\
	glog.log("%s:%d: %s:  Assertion '%s' failed ", __FILE__, __LINE__, __FUNCTION__, #exp);	\
	g_pLogger->Log("%s:%d: %s:  Assertion '%s' failed ", __FILE__, __LINE__, __FUNCTION__, #exp);	\
	assert(exp);	\
}
#define INVALID_HANDLE_VALUE -1
#endif
typedef unsigned char uchar;
typedef unsigned short ushort;
typedef unsigned int uint;

typedef long long int64;

#endif

