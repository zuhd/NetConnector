#include "./SocketBase.h"
bool ISocketBase::SetReusePort(LG_SOCKET sock)
{
	int on=1;
	int ret=setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (char *)&on, sizeof(int));
	return ret!=-1;
}
bool ISocketBase::SetNonBlocking(LG_SOCKET sock)
{
#ifdef WIN32
	unsigned long ul = 1;
	int nRet;
	nRet = ioctlsocket(sock, FIONBIO, &ul);
	if(nRet ==  SOCKET_ERROR)
	{
		return false;
	}
	return true;
#else
	int opts;
	opts=fcntl(sock,F_GETFL);
	if(opts<0)
		opts=O_NONBLOCK;
	else
		opts = opts|O_NONBLOCK;
	if(fcntl(sock,F_SETFL,opts)<0)
	{
		return false;
	}
	else
		return true;



#endif
}
