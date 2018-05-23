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
	gate_account_login_req = 10000,			// 用户请求登录
	gate_account_login_ack,					// 用户登录反馈
	gate_account_login_not,					// 广播用户登录
	gate_account_logout_req,				// 用户请求登出
	gate_account_logout_ack,				// 用户登出反馈
	gate_account_logout_not,				// 广播用户登出
	gate_account_transdata_req,				// 用户请求转发数据
	gate_account_transdata_not,				// 广播用户数据
	gate_account_sessionlist_not,			// 服务器连接广播
};

enum 
{
	agent_type_host = 0,						// 主控端
	agent_type_client,							// 被控端
};

enum
{
	session_flag_trunk = 0,						// 主连接
	session_flag_branch,						// 子连接
};

enum 
{
	device_type_pc = 0,							// PC设备
	device_type_android,						// 安卓设备
	device_type_ios,							// IOS设备
	device_type_browser,						// 浏览器
	device_type_others,							// 其他设备
};

enum 
{
	session_type_vedio = 0,						// 视频
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
