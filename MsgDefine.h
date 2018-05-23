#pragma once
#include "PacketDefine.h"
MSG_DEFINE(msg_as_register_account_req)
{
	string strAccount; //�û��ʺ�
	string strPassword;	//�û�����
	int	nAreaID;		//����id
	string strEthMac;	//ע��ͻ��˵�mac��ַ

};
MSG_DEFINE(msg_as_register_account_ack)
{
	string	strAccount; //ע����ʺ�
	char	i8ret;		//0ע��ɹ�, -1δ֪����, -2�û����Ѿ�����, , -5��ֹ�ظ�ע��, -6��ǰ�����������pid
	string	strTicket; //ͨ��֤
	uint	nPID;
	int		nNextRegTime;	//���´ο�ע��ʱ��

};
MSG_DEFINE(msg_as_verify_account_req)
{
	char cMode;			//0�˺���֤ 1PID��֤ 2������֤ 3�ֻ���֤ -1ģ����֤
	uint nPID;			//PID
	string strAccount;  //�û��ʺ�
	string strEmail;	//����
	string strMobile;	//�ֻ�
	string strPassword; //�û�����
};
MSG_DEFINE(msg_as_verify_account_ack)
{	
	char	i8ret;		//0��֤ͨ����-1δ֪����, -2�������, -3���˺Ų�����,-4�����PIDδ����
	string	strTicket;  //ͨ��֤, ʧ��Ϊ��
	uint	nPID;		//PID
	string strAccount;	//�˺�
	string strEmail;	//����
	string strMobile;	//�ֻ�
};
MSG_DEFINE(msg_as_update_account_req)
{
	string strAccount;		//�û��ʺ�
	string strOldPassword;  //�û�ԭ����
	string strNewPassword;  //�û�������
};
MSG_DEFINE(msg_as_update_account_ack)
{
	string strAccount;	//��֤���ʺ�
	char i8ret;			//0��������ɹ���-1δ֪����, -2ԭ�������,-3�û��������ڣ�
};
MSG_DEFINE(msg_as_unid_query_req)
{
	uint	nPID;			//nPID��Ϊ0�� strAccount��Ϊ�գ�
	string	strAccount;	
};
MSG_DEFINE(msg_as_unid_query_ack)
{
	char	cRet;			//0��ѯ�ɹ�, -1��������, -2�û�id���ʺŲ�����
	uint	nPID;			//�û�Ψһid
	string	strAccount;		//�û��ʺ�
	int		nAreaID;		//�û�ע������id
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
	uint	nPID;			// �û�ΨһID
	char	cAgentType;		// �ο�ö��
	char	cDeviceType;	// �ο�ö��
	char	cSessionFlag;	// ���ӱ��
	int		nVersion;		// �汾��

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
	char	cRet;			// ��¼��� 0�ɹ� ��0ʧ��
	uint	nPID;			// �û�ΨһID
	uint	nSID;			// �ɹ����SessionID
};
MSG_DEFINE(msg_gate_account_login_not)
{
	uint	nPID;			// ��¼��PID
	uint	nSID;			// SessionID
	char	cAgentType;		// �ο�ö��
	char	cDeviceType;	// �ο�ö��
	char	cSessionFlag;	// ���ӱ��
	int		nVersion;		// �汾��

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
	uint	nPID;			// �û�ΨһID
	uint	nSID;			// SessionID
};
MSG_DEFINE(msg_gate_account_logout_ack)
{
	char	cRet;			// �ǳ����
};
MSG_DEFINE(msg_gate_account_logout_not)
{
	uint	nPID;			// �ǳ���PID
	uint	nSID;			// SessionID
};
MSG_DEFINE(msg_gate_account_transdata_req)
{
	uint			nSrcPID;			// ��ԴPID
	uint			nSrcSID;			// ��ԴSessionID	
	vector<uint>	vecDstSID;			// ��Ҫת����Ŀ��SessionID
	uint			nSerialNO;			// �������к�
	uint			nTotalSize;			// �����ܳ���
	ushort			sDataLen;			// ��ʵ���ݵĳ��� ���5K
	char			arrData[MAX_TRANSDATA_LEN];	// ��ʵ����
};

MSG_DEFINE(msg_gate_account_transdata_not)
{
	uint			nSrcPID;			// ��ԴPID
	uint			nSrcSID;			// ��ԴSessionID	
	uint			nSerialNO;			// �������к�
	uint			nTotalSize;			// �����ܳ���
	ushort			sDataLen;			// ��ʵ���ݵĳ��� ���2K
	char			arrData[MAX_TRANSDATA_LEN];	// ��ʵ����
};

MSG_DEFINE(msg_gate_account_transdata_complete)
{
	uint	        nSrcPID;			// ��ԴPID
	uint			nSrcSID;			// ��ԴSessionID	
	uint			nSerialNO;			// �������к�
	char*           pBuffer;            // ����������
	int             nBufferLen;
};

MSG_DEFINE(st_session)
{
	uint	nPID;			// ��¼��PID
	uint	nSID;			// SessionID
	char	cAgentType;		// �ο�ö��
	char	cDeviceType;	// �ο�ö��
	char	cSessionFlag;	// ���ӱ��
	int		nVersion;		// �汾��

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