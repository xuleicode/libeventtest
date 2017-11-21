#ifndef _LOG_H_
#define _LOG_H_
#include "stdafx.h"
class Log
{
public:
	static void e(char * msg);//级别为error的日志
	static void d(char * msg);//级别为debug的日志
	static void i(char * msg);//级别为info的日志
	static void w(char * msg);//级别为warm的日志
	static void writeLog(char * level,char * msg);//写入日志
	static const int isLog = 1;//是否写入日志，1为写入
};
#endif