#pragma once
enum
{
	as_register_account_req = 8110,
	as_register_account_ack,
	as_verify_account_req,
	as_verify_account_ack,
	as_update_account_req,
	as_update_account_ack,
	as_unid_query_req,
	as_unid_query_ack,
};

enum 
{
	gate_account_login_req = 10000,			// �û������¼
	gate_account_login_ack,					// �û���¼����
	gate_account_login_not,					// �㲥�û���¼
	gate_account_logout_req,				// �û�����ǳ�
	gate_account_logout_ack,				// �û��ǳ�����
	gate_account_logout_not,				// �㲥�û��ǳ�
	gate_account_transdata_req,				// �û�����ת������
	gate_account_transdata_not,				// �㲥�û�����
	gate_account_sessionlist_not,			// ���������ӹ㲥
};

enum 
{
	agent_type_host = 0,						// ���ض�
	agent_type_client,							// ���ض�
};

enum
{
	session_flag_trunk = 0,						// ������
	session_flag_branch,						// ������
};

enum 
{
	device_type_pc = 0,							// PC�豸
	device_type_android,						// ��׿�豸
	device_type_ios,							// IOS�豸
	device_type_browser,						// �����
	device_type_others,							// �����豸
};

enum 
{
	session_type_vedio = 0,						// ��Ƶ
	session_type_audio,
	session_type_desktop,
	session_type_file,
	session_type_others,
};

enum 
{
	socket_type_tcp = 0,
	socket_type_udp,
};

#define MAX_TRANSDATA_LEN (MAX_PACKET_LEN - 1024)
