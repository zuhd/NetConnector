#include "Serialize.h"


extern int64 htonll(int64 nValue)
{
	un_value u1, u2;
	u1.nValue = nValue;
	u2.stValue.nValue2 = htonl(u1.stValue.nValue1);
	u2.stValue.nValue1 = htonl(u1.stValue.nValue2);
	return u2.nValue;
}

extern int64 ntohll(int64 nValue)
{
	un_value u1, u2;
	u1.nValue = nValue;
	u2.stValue.nValue2 = ntohl(u1.stValue.nValue1);
	u2.stValue.nValue1 = ntohl(u1.stValue.nValue2);
	return u2.nValue;
}
