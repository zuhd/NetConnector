#ifndef NETPACKETPARSER_H
#define NETPACKETPARSER_H
#include "ISocketSystem.h"
#include "GlobalDefine.h"
#include "zlib.h"

class CNetPacketParser : public IPacketParser
{
	enum
	{
		KeySize = 16,
	};
public:
	CNetPacketParser(void);
	~CNetPacketParser(void);
	virtual int EncodePacket(const char* pInData, int nInLen, char* pOutData, int& nOutLen);
	virtual int DecodePacket(const char* pInData, int nInLen, char* pOutData, int& nOutLen);
private:
	uchar	m_szSymmKey[16];
	ushort	m_shSeed;
};
#endif