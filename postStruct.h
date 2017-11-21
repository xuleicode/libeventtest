

/**
post 消息主体
*/

#ifndef _POSTSTRUCT_H
#define _POSTSTRUCT_H
#include"stdafx.h"

//请求
struct Request
{
	int Protocol_Version;
	int Length;
	int Message_Type;
	int TransactionID;
	int SessionID;
	char * MessageBody;
};

//返回
struct Response
{
	int Protocol_Version;
	int Length;
	int Message_Type;
	int TransactionID;
	unsigned short Return_Code;
	char * MessageBody;
};

#endif