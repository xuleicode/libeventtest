#ifndef _READCONFIG_H_
#define _READCONFIG_H_
#include "stdafx.h"
class ReadConfig
{
public:
	char * l_trim(char * szOutput, const char *szInput);//ɾ����ߵĿո� 
	char *r_trim(char *szOutput, const char *szInput);//ɾ���ұߵĿո�
	char * a_trim(char * szOutput, const char * szInput);//ɾ�����ߵĿո�
	int ReadConfigFile( char *AppName, char *KeyName, char *KeyVal );//��ȡconfig�ļ����ļ�Ϊconf.conf,
};
#endif