

/**
post ��Ϣ����
*/

#ifndef _POSTSTRUCT_H
#define _POSTSTRUCT_H
#include"stdafx.h"

//����
struct Request
{
	int Protocol_Version;
	int Length;
	int Message_Type;
	int TransactionID;
	int SessionID;
	char * MessageBody;
};

//����
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