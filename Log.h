#ifndef _LOG_H_
#define _LOG_H_
#include "stdafx.h"
class Log
{
public:
	static void e(char * msg);//����Ϊerror����־
	static void d(char * msg);//����Ϊdebug����־
	static void i(char * msg);//����Ϊinfo����־
	static void w(char * msg);//����Ϊwarm����־
	static void writeLog(char * level,char * msg);//д����־
	static const int isLog = 1;//�Ƿ�д����־��1Ϊд��
};
#endif