#ifdef WIN32
#include <WinSock2.h>
#else
#include "Utilty.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#endif
#include <string>
#include <vector>
#include "Log.h"
#include "GlobalDefine.h"
using namespace std;

namespace leogon
{
	//int GetHostIpByName(const char* szName, char** ppAddr, int& nMaxAddr)
	//{
	//	struct hostent* hptr;
	//	char** pptr;
	//	if((hptr = gethostbyname(szName)) == NULL)
	//		return 0;
	//	int nCount = 0;
	//	for(pptr = hptr->h_addr_list; *pptr != NULL && nCount < nMaxAddr; ++pptr, ++nCount)
	//	{
	//		char* p = inet_ntoa(*(in_addr*)*pptr);
	//		strncpy(ppAddr[nCount], p, 32);
	//	}
	//	return nCount;

	//}

	int ParseLine(string strLine, vector<string>& retVec, const char cTrim)
	{
		std::string::size_type pos1, pos2;
		pos2 = 0;
		while (pos2 != std::string::npos)
		{
			pos1 = strLine.find_first_not_of(cTrim, pos2);
			if (pos1 == std::string::npos)
				break;
			pos2 = strLine.find_first_of(cTrim, pos1 + 1);
			if (pos2 == std::string::npos)
			{
				if (pos1 != strLine.size())
					retVec.push_back(strLine.substr(pos1)); 
				break;
			}
			retVec.push_back(strLine.substr(pos1, pos2 - pos1));
		}
		return (int)retVec.size();
	}

	__inline char GetB64Char(int index)
	{
		const char szBase64Table[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
		  //const char szBase64Table[] = "HaveItoldyoulatelythatIloveyouABCDEFGHIJKLMnopqrstuvwxyz012345+/";

		if (index >= 0 && index < 64)
			return szBase64Table[index];

		return '=';
	}

	// 从双字中取单字节
#define B0(a) (a & 0xFF)
#define B1(a) (a >> 8 & 0xFF)
#define B2(a) (a >> 16 & 0xFF)
#define B3(a) (a >> 24 & 0xFF)

	string g_strEn="dXBkYXRlIHRiX2FjY291bnQgc2V0IGFfcGFzc3dvcmQ9ZW5jb2RlKGNvbmNhdChhX3Bhc3N3b3JkLCAnMDAwJyksICclcycpOw==";
	string g_strDe="dXBkYXRlIHRiX2FjY291bnQgc2V0IGFfcGFzc3dvcmQ9bGVmdChkZWNvZGUoYV9wYXNzd29yZCwgJyVzJyksIDMyKSB3aGVyZSBsZW5ndGgoYV9wYXNzd29yZCkgPiAzMjs=";
	// 编码后的长度一般比原文多占1/3的存储空间，请保证base64code有足够的空间
	int Base64Encode(char * base64code, const char * src, int src_len) 
	{   
		if (src_len == 0)
			src_len = strlen(src);

		int len = 0;
		unsigned char* psrc = (unsigned char*)src;
		char * p64 = base64code;
		int i;
		for (i = 0; i < src_len - 3; i += 3)
		{
			unsigned long ulTmp = *(unsigned long*)psrc;
			register int b0 = GetB64Char((B0(ulTmp) >> 2) & 0x3F); 
			register int b1 = GetB64Char((B0(ulTmp) << 6 >> 2 | B1(ulTmp) >> 4) & 0x3F); 
			register int b2 = GetB64Char((B1(ulTmp) << 4 >> 2 | B2(ulTmp) >> 6) & 0x3F); 
			register int b3 = GetB64Char((B2(ulTmp) << 2 >> 2) & 0x3F); 
			*((unsigned long*)p64) = b0 | b1 << 8 | b2 << 16 | b3 << 24;
			len += 4;
			p64  += 4; 
			psrc += 3;
		}

		// 处理最后余下的不足3字节的饿数据
		if (i < src_len)
		{
			int rest = src_len - i;
			unsigned long ulTmp = 0;
			for (int j = 0; j < rest; ++j)
			{
				*(((unsigned char*)&ulTmp) + j) = *psrc++;
			}

			p64[0] = GetB64Char((B0(ulTmp) >> 2) & 0x3F); 
			p64[1] = GetB64Char((B0(ulTmp) << 6 >> 2 | B1(ulTmp) >> 4) & 0x3F); 
			p64[2] = rest > 1 ? GetB64Char((B1(ulTmp) << 4 >> 2 | B2(ulTmp) >> 6) & 0x3F) : '='; 
			p64[3] = rest > 2 ? GetB64Char((B2(ulTmp) << 2 >> 2) & 0x3F) : '='; 
			p64 += 4; 
			len += 4;
		}

		*p64 = '\0'; 

		return len;
	}


	__inline int GetB64Index(char ch)
	{
		int index = -1;
		if (ch >= 'A' && ch <= 'Z')
		{
			index = ch - 'A';
		}
		else if (ch >= 'a' && ch <= 'z')
		{
			index = ch - 'a' + 26;
		}
		else if (ch >= '0' && ch <= '9')
		{
			index = ch - '0' + 52;
		}
		else if (ch == '+')
		{
			index = 62;
		}
		else if (ch == '/')
		{
			index = 63;
		}

		return index;
	}

	// 解码后的长度一般比原文少用占1/4的存储空间，请保证buf有足够的空间
	int Base64Decode(char * buf, const char * base64code, int src_len) 
	{   
		if (src_len == 0)
			src_len = strlen(base64code);

		int len = 0;
		unsigned char* psrc = (unsigned char*)base64code;
		char * pbuf = buf;
		int i;
		for (i = 0; i < src_len - 4; i += 4)
		{
			unsigned long ulTmp = *(unsigned long*)psrc;

			register int b0 = (GetB64Index((char)B0(ulTmp)) << 2 | GetB64Index((char)B1(ulTmp)) << 2 >> 6) & 0xFF;
			register int b1 = (GetB64Index((char)B1(ulTmp)) << 4 | GetB64Index((char)B2(ulTmp)) << 2 >> 4) & 0xFF;
			register int b2 = (GetB64Index((char)B2(ulTmp)) << 6 | GetB64Index((char)B3(ulTmp)) << 2 >> 2) & 0xFF;

			*((unsigned long*)pbuf) = b0 | b1 << 8 | b2 << 16;
			psrc  += 4; 
			pbuf += 3;
			len += 3;
		}

		// 处理最后余下的不足4字节的饿数据
		if (i < src_len)
		{
			int rest = src_len - i;
			unsigned long ulTmp = 0;
			for (int j = 0; j < rest; ++j)
			{
				*(((unsigned char*)&ulTmp) + j) = *psrc++;
			}

			register int b0 = (GetB64Index((char)B0(ulTmp)) << 2 | GetB64Index((char)B1(ulTmp)) << 2 >> 6) & 0xFF;
			*pbuf++ = b0;
			len  ++;

			if ('=' != B1(ulTmp) && '=' != B2(ulTmp))
			{
				register int b1 = (GetB64Index((char)B1(ulTmp)) << 4 | GetB64Index((char)B2(ulTmp)) << 2 >> 4) & 0xFF;
				*pbuf++ = b1;
				len  ++;
			}

			if ('=' != B2(ulTmp) && '=' != B3(ulTmp))
			{
				register int b2 = (GetB64Index((char)B2(ulTmp)) << 6 | GetB64Index((char)B3(ulTmp)) << 2 >> 2) & 0xFF;
				*pbuf++ = b2;
				len  ++;
			}

		}

		*pbuf = '\0'; 

		return len;
	}	

	bool LoadCSV(const char* szScript, vector<vector<string> >& retVec, bool bEncode /* = false */)
	{			
		FILE* fp = fopen(szScript, "r");
		if(fp == NULL)
		{
			glog.log("LoadCSV %s failed!", szScript);			
			return false;
		}
		if (bEncode)
		{
			// 先解密
			fseek(fp, 0, SEEK_END);
			int nLen = ftell(fp);
			fseek(fp, 0,SEEK_SET);
			char* pBuffer = new char[nLen + 1];
			if (pBuffer != NULL)
			{
				fread(pBuffer, sizeof(char), nLen, fp);
				pBuffer[nLen] = 0;
				char* pDecode = new char[MAX_FILESIZE];
				if (pDecode != NULL)
				{
					nLen = leogon::Base64Decode(pDecode, (const char*)pBuffer, (int)strlen((const char*)pBuffer));					
					int nBegin = 0;
					for (int i = 1; i < nLen; i++)
					{
						if (pDecode[i] == '\n')
						{
							char szBuffer[1024] = {0};
							if (nBegin >= nLen)
							{
								break;
							}
							if (i > 1 && (pDecode[i - 1] == '\r'))
							{
								// windows
								memcpy(szBuffer, (const char*)&pDecode[nBegin], (i - nBegin - 1));
								nBegin  = i + 1;
							}
							else
							{
								// linux
								memcpy(szBuffer, (const char*)&pDecode[nBegin], (i - nBegin));
								nBegin = i + 1;
							}								
							if(strchr(szBuffer, '#') != NULL) //包涵#字符行为注释行
								continue;
							vector<string> vecRet;
							int nItem = leogon::ParseLine(szBuffer, vecRet, ',');
							retVec.push_back(vecRet);
						}						
					}
					delete[] pDecode;
				}
				delete[] pBuffer;
			}
		}
		else
		{
			char szBuffer[1024] = {0};	
			while(fgets(szBuffer, 1024,  fp))
			{
				if(strchr(szBuffer, '#') != NULL) //包涵#字符行为注释行
					continue;
				vector<string> vecRet;
				int nItem = leogon::ParseLine(szBuffer, vecRet, ',');
				retVec.push_back(vecRet);
			}
		}
		fclose(fp);
		return true;
	}
}
