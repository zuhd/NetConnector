/************************************************************************/
/* CopyRight @ 2010 by Anyone. No rights is reserved.
/* @Author: ZUHD
/* @Date: 11/11/2010
/* @Description:   一些常用的重要的函数定义
/************************************************************************/
#ifndef UTILITY_H
#define UTILITY_H
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#ifdef WIN32
#include <iostream>
//#include <windows.h>
#include "WinInclude.h" 
#include <WinBase.h>
#include <ctime>
#include<time.h>
#else
#include <sys/time.h>
#endif
#include <string>
#include <vector>
using namespace std;

namespace leogon
{
	
	extern string g_strEn;
	extern string g_strDe;

	//int GetHostIpByName(const char* szName, char** ppAddr, int& nMaxAddr);
	inline void Hex2String(const char* hex, int hexlen, char* str)
	{
		int i, o;
		for(i = 0, o = 0; i < hexlen; ++i, o += 2)
		{
			sprintf((char*)&str[o], "%02X", hex[i]);
		}
		//str[o + 1] = '\0';
	}
	inline void String2Hex(const char* str, char* hex)
	{
		char* stop = 0;
		for(int i = 0, o = 0; i < (int)strlen(str); i += 2, ++o)
		{
			char c[3];
			memcpy(&c, &str[i], 2);
			c[2] = '\0';
			hex[o] = (char)strtol(c, &stop, 16); 
		}
	}

	inline unsigned int GetCurTime()
	{
#ifdef WIN32
		return GetTickCount();
#else
		struct timeval tv;
		gettimeofday(&tv, 0);
		return tv.tv_sec * 1000 + tv.tv_usec / 1000;
#endif
	}
	
	int ParseLine(string strLine, vector<string>& retVec, const char cTrim = ',');	

	// 编码后的长度一般比原文多占1/3的存储空间，请保证base64code有足够的空间
	int Base64Encode(char * base64code, const char * src, int src_len = 0); 
	int Base64Decode(char * buf, const char * base64code, int src_len = 0);	

	bool LoadCSV(const char* szScript, vector<vector<string> >& retVec, bool bEncode = false);
}

#endif
