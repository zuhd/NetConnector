#pragma once
#include "PacketDefine.h"
MSG_DEFINE(msg_as_register_account_req)
{
	string strAccount; //用户帐号
	string strPassword;	//用户密码
	int	nAreaID;		//区域id
	string strEthMac;	//注册客户端的mac地址

};
MSG_DEFINE(msg_as_register_account_ack)
{
	string	strAccount; //注册的帐号
	char	i8ret;		//0注册成功, -1未知错误, -2用户名已经存在, , -5禁止重复注册, -6当前服务器的最大pid
	string	strTicket; //通行证
	uint	nPID;
	int		nNextRegTime;	//距下次可注册时间

};
MSG_DEFINE(msg_as_verify_account_req)
{
	char cMode;			//0账号验证 1PID验证 2邮箱验证 3手机验证 -1模糊验证
	uint nPID;			//PID
	string strAccount;  //用户帐号
	string strEmail;	//邮箱
	string strMobile;	//手机
	string strPassword; //用户密码
};
MSG_DEFINE(msg_as_verify_account_ack)
{	
	char	i8ret;		//0验证通过，-1未知错误, -2密码错误, -3用账号不存在,-4输入的PID未激活
	string	strTicket;  //通行证, 失败为空
	uint	nPID;		//PID
	string strAccount;	//账号
	string strEmail;	//邮箱
	string strMobile;	//手机
};
MSG_DEFINE(msg_as_update_account_req)
{
	string strAccount;		//用户帐号
	string strOldPassword;  //用户原密码
	string strNewPassword;  //用户新密码
};
MSG_DEFINE(msg_as_update_account_ack)
{
	string strAccount;	//验证的帐号
	char i8ret;			//0更改密码成功，-1未知错误, -2原密码错误,-3用户名不存在，
};
MSG_DEFINE(msg_as_unid_query_req)
{
	uint	nPID;			//nPID不为0或 strAccount不为空，
	string	strAccount;	
};
MSG_DEFINE(msg_as_unid_query_ack)
{
	char	cRet;			//0查询成功, -1服务器记, -2用户id或帐号不存在
	uint	nPID;			//用户唯一id
	string	strAccount;		//用户帐号
	int		nAreaID;		//用户注册区域id
};



//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
MSG_READ(msg_as_register_account_req)
{
	return is >> msg.opcode >> msg.strAccount >> msg.strPassword >> msg.nAreaID >> msg.strEthMac;
}
MSG_WRITE(msg_as_register_account_req)
{
	return os << msg.opcode << msg.strAccount << msg.strPassword << msg.nAreaID << msg.strEthMac;
}
MSG_READ(msg_as_register_account_ack)
{
	return is >> msg.opcode >> msg.strAccount >> msg.i8ret >> msg.strTicket >> msg.nPID >> msg.nNextRegTime;
}
MSG_WRITE(msg_as_register_account_ack)
{
	return os << msg.opcode << msg.strAccount << msg.i8ret << msg.strTicket << msg.nPID << msg.nNextRegTime;
}
MSG_READ(msg_as_verify_account_req)
{
	return is >> msg.opcode >> msg.cMode >> msg.nPID >> msg.strAccount >> msg.strEmail >> msg.strMobile >> msg.strPassword;
}
MSG_WRITE(msg_as_verify_account_req)
{
	return os << msg.opcode << msg.cMode << msg.nPID << msg.strAccount << msg.strEmail << msg.strMobile << msg.strPassword;
}
MSG_READ(msg_as_verify_account_ack)
{
	return is >> msg.opcode >> msg.i8ret >> msg.strTicket >> msg.nPID >> msg.strAccount >> msg.strEmail >> msg.strMobile;
}
MSG_WRITE(msg_as_verify_account_ack)
{
	return os << msg.opcode << msg.i8ret << msg.strTicket << msg.nPID << msg.strAccount << msg.strEmail << msg.strMobile;
}
MSG_READ(msg_as_update_account_req)
{
	return is >> msg.opcode >> msg.strAccount >> msg.strOldPassword >> msg.strNewPassword;
}
MSG_WRITE(msg_as_update_account_req)
{
	return os << msg.opcode << msg.strAccount << msg.strOldPassword << msg.strNewPassword;
}
MSG_READ(msg_as_update_account_ack)
{
	return is >> msg.opcode >> msg.strAccount >> msg.i8ret;
}
MSG_WRITE(msg_as_update_account_ack)
{
	return os << msg.opcode << msg.strAccount << msg.i8ret;
}
MSG_READ(msg_as_unid_query_req)
{
	return is >> msg.opcode >> msg.nPID >> msg.strAccount;
}
MSG_WRITE(msg_as_unid_query_req)
{
	return os << msg.opcode << msg.nPID << msg.strAccount;
}
MSG_READ(msg_as_unid_query_ack)
{
	return is >> msg.opcode >> msg.cRet >> msg.nPID >> msg.strAccount >> msg.nAreaID;
}
MSG_WRITE(msg_as_unid_query_ack)
{
	return os << msg.opcode << msg.cRet << msg.nPID << msg.strAccount << msg.nAreaID;
}
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//GateServer
//////////////////////////////////////////////////////////////////////////
MSG_DEFINE(msg_gate_account_login_req)
{
	uint	nPID;			// 用户唯一ID
	char	cAgentType;		// 参考枚举
	char	cDeviceType;	// 参考枚举
	char	cSessionFlag;	// 连接标记
	int		nVersion;		// 版本号

	msg_gate_account_login_req()
	{
		nPID = 0;
		cAgentType = -1;
		cDeviceType = -1;
		cSessionFlag = -1;
		nVersion = 0;
	}
};
MSG_DEFINE(msg_gate_account_login_ack)
{
	char	cRet;			// 登录结果 0成功 非0失败
	uint	nPID;			// 用户唯一ID
	uint	nSID;			// 成功后的SessionID
};
MSG_DEFINE(msg_gate_account_login_not)
{
	uint	nPID;			// 登录的PID
	uint	nSID;			// SessionID
	char	cAgentType;		// 参考枚举
	char	cDeviceType;	// 参考枚举
	char	cSessionFlag;	// 连接标记
	int		nVersion;		// 版本号

	msg_gate_account_login_not()
	{
		nPID = 0;
		nSID = 0;
		cAgentType = -1;
		cDeviceType = -1;
		cSessionFlag = -1;
		nVersion = 0;
	}
};
MSG_DEFINE(msg_gate_account_logout_req)
{
	uint	nPID;			// 用户唯一ID
	uint	nSID;			// SessionID
};
MSG_DEFINE(msg_gate_account_logout_ack)
{
	char	cRet;			// 登出结果
};
MSG_DEFINE(msg_gate_account_logout_not)
{
	uint	nPID;			// 登出的PID
	uint	nSID;			// SessionID
};
MSG_DEFINE(msg_gate_account_transdata_req)
{
	uint			nSrcPID;			// 来源PID
	uint			nSrcSID;			// 来源SessionID	
	vector<uint>	vecDstSID;			// 需要转发的目的SessionID
	uint			nSerialNO;			// 包的序列号
	uint			nTotalSize;			// 包的总长度
	ushort			sDataLen;			// 真实数据的长度 最大5K
	char			arrData[MAX_TRANSDATA_LEN];	// 真实数据
};

MSG_DEFINE(msg_gate_account_transdata_not)
{
	uint			nSrcPID;			// 来源PID
	uint			nSrcSID;			// 来源SessionID	
	uint			nSerialNO;			// 包的序列号
	uint			nTotalSize;			// 包的总长度
	ushort			sDataLen;			// 真实数据的长度 最大2K
	char			arrData[MAX_TRANSDATA_LEN];	// 真实数据
};

MSG_DEFINE(msg_gate_account_transdata_complete)
{
	uint	        nSrcPID;			// 来源PID
	uint			nSrcSID;			// 来源SessionID	
	uint			nSerialNO;			// 包的序列号
	char*           pBuffer;            // 完整的数据
	int             nBufferLen;
};

MSG_DEFINE(st_session)
{
	uint	nPID;			// 登录的PID
	uint	nSID;			// SessionID
	char	cAgentType;		// 参考枚举
	char	cDeviceType;	// 参考枚举
	char	cSessionFlag;	// 连接标记
	int		nVersion;		// 版本号

	st_session()
	{
		nPID = 0;
		nSID = 0;
		cAgentType = -1;
		cDeviceType = -1;
		cSessionFlag = -1;
		nVersion = 0;
	}
};

MSG_DEFINE(msg_gate_account_sessionlist_not)
{
	vector<st_session>	vecSession;
};

MSG_READ(msg_gate_account_login_req)
{
	return is >> msg.opcode >> msg.nPID >> msg.cAgentType >> msg.cDeviceType >> msg.cSessionFlag >> msg.nVersion;
}
MSG_WRITE(msg_gate_account_login_req)
{
	return os << msg.opcode << msg.nPID << msg.cAgentType << msg.cDeviceType << msg.cSessionFlag << msg.nVersion;
}

MSG_READ(msg_gate_account_login_ack)
{
	return is >> msg.opcode >> msg.cRet >> msg.nPID >> msg.nSID;
}
MSG_WRITE(msg_gate_account_login_ack)
{
	return os << msg.opcode << msg.cRet << msg.nPID << msg.nSID;
}

MSG_READ(msg_gate_account_login_not)
{
	return is >> msg.opcode >> msg.nPID >> msg.nSID >> msg.cAgentType >> msg.cDeviceType >> msg.cSessionFlag >> msg.nVersion;
}
MSG_WRITE(msg_gate_account_login_not)
{
	return os << msg.opcode << msg.nPID << msg.nSID << msg.cAgentType << msg.cDeviceType << msg.cSessionFlag << msg.nVersion;
}

MSG_READ(msg_gate_account_logout_req)
{
	return is >> msg.opcode >> msg.nPID >> msg.nSID;
}
MSG_WRITE(msg_gate_account_logout_req)
{
	return os << msg.opcode << msg.nPID << msg.nSID;
}

MSG_READ(msg_gate_account_logout_ack)
{
	return is >> msg.opcode >> msg.cRet;
}
MSG_WRITE(msg_gate_account_logout_ack)
{
	return os << msg.opcode << msg.cRet;
}

MSG_READ(msg_gate_account_logout_not)
{
	return is >> msg.opcode >> msg.nPID >> msg.nSID;
}
MSG_WRITE(msg_gate_account_logout_not)
{
	return os << msg.opcode << msg.nPID << msg.nSID;
}

MSG_READ(msg_gate_account_transdata_req)
{
	is >> msg.opcode >> msg.nSrcPID >> msg.nSrcSID >> msg.vecDstSID >> msg.nSerialNO >> msg.nTotalSize >> msg.sDataLen;
	is.ReadData(msg.arrData, msg.sDataLen);
	return is;
}
MSG_WRITE(msg_gate_account_transdata_req)
{
	os << msg.opcode << msg.nSrcPID << msg.nSrcSID << msg.vecDstSID << msg.nSerialNO << msg.nTotalSize << msg.sDataLen;
	os.WriteData(msg.arrData, msg.sDataLen);
	return os;	
}

MSG_READ(msg_gate_account_transdata_not)
{
	is >> msg.opcode >> msg.nSrcPID >> msg.nSrcSID >> msg.nSerialNO >> msg.nTotalSize >> msg.sDataLen;
	is.ReadData(msg.arrData, msg.sDataLen);
	return is;
}
MSG_WRITE(msg_gate_account_transdata_not)
{
	os << msg.opcode << msg.nSrcPID << msg.nSrcSID << msg.nSerialNO << msg.nTotalSize << msg.sDataLen;
	os.WriteData(msg.arrData, msg.sDataLen);
	return os;	
}

MSG_READ(st_session)
{
	return is >> msg.nPID >> msg.nSID >> msg.cAgentType >> msg.cDeviceType >> msg.cSessionFlag >> msg.nVersion;
}
MSG_WRITE(st_session)
{
	return os << msg.nPID << msg.nSID << msg.cAgentType << msg.cDeviceType << msg.cSessionFlag << msg.nVersion;
}

MSG_READ(msg_gate_account_sessionlist_not)
{
	return is >> msg.opcode >> msg.vecSession;
}
MSG_WRITE(msg_gate_account_sessionlist_not)
{
	return os << msg.opcode << msg.vecSession;
}