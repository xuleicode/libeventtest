#ifndef _READCONFIG_H_
#define _READCONFIG_H_
#include "stdafx.h"
class ReadConfig
{
public:
	char * l_trim(char * szOutput, const char *szInput);//删除左边的空格 
	char *r_trim(char *szOutput, const char *szInput);//删除右边的空格
	char * a_trim(char * szOutput, const char * szInput);//删除两边的空格
	int ReadConfigFile( char *AppName, char *KeyName, char *KeyVal );//读取config文件，文件为conf.conf,
};
#endif