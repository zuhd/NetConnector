/************************************************************************/
/* CopyRight @ 2010 by Anyone. No rights is reserved.
/* @Author: ZUHD
/* @Date: 11/11/2010
/* @Description:   网络消息数据包定义
/************************************************************************/
#ifndef PACKETDEFINE_H
#define PACKETDEFINE_H
#include "Serialize.h"
struct msg_header
{
	short opcode;
};

// 代理服务器专用
struct msg_hash
{
	int nHashID;
};
#ifdef NO_INLINE_SERIAL

#define MSG_DEFINE(message) \
struct message; \
	CInputStream& operator>>(CInputStream& is, message& msg); \
	COutputStream& operator<<(COutputStream& os, const message& msg); \
struct message : public msg_header	

#define MSG_READ(message) \
	CInputStream& operator>>(CInputStream& is, message& msg)

#define MSG_WRITE(message) \
	COutputStream& operator<<(COutputStream& os, const message& msg)

#else

#define MSG_DEFINE(message) \
struct message; \
	inline CInputStream& operator>>(CInputStream& is, message& msg); \
	inline COutputStream& operator<<(COutputStream& os, const message& msg); \
struct message : public msg_header

#define MSG_READ(message) \
	CInputStream& operator>>(CInputStream& is, message& msg)

#define MSG_WRITE(message) \
	COutputStream& operator<<(COutputStream& os, const message& msg)

#endif

#define DISPATCH_MESSAGE(message) \
{ \
	message obj; \
	is >> obj; \
	OnMessage(obj); \
	break;	\
}

#define DISPATCH_MESSAGE_REF(message, parm) \
{ \
	message msg(parm); \
	is >> msg; \
	OnMessage(msg); \
	break;	\
}

enum lg_opcode
{
	lg_net_echo_not = 0,
	lg_net_connect,
	lg_net_closeconnect,
};
MSG_DEFINE(msg_lg_net_echo_not)
{
	unsigned int nTime;
};
MSG_DEFINE(msg_lg_net_connect)
{
	int serverID;
};
MSG_DEFINE(msg_lg_net_closeconnect)
{
};
MSG_READ(msg_lg_net_echo_not)
{
	return is >> msg.opcode >> msg.nTime;
}
MSG_WRITE(msg_lg_net_echo_not)
{
	return os << msg.opcode << msg.nTime;
}

#endif

