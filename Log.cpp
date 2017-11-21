#include "stdafx.h"
#ifdef WIN32
#include <io.h>
#endif



void Log::writeLog(char * level,char * msg)
{
	if(isLog == 1)
	{
		char * filePath = (char *)malloc(1024);
		memset(filePath,0,1024);
#ifdef WIN32
		//strcpy(filePath,"D:/libeventTest/log");
		getcwd(filePath,1024);
		strcat(filePath,"/log");
		if(access(filePath,0) == -1)
		{
			mkdir(filePath);
		}
#else
		getcwd(filePath,1024);
		strcat(filePath,"/log");
		if(access(filePath,0) == -1)
		{
			mkdir(filePath,0777);
		}
#endif
		time_t t = time(0); 
		char * fileName = (char *)malloc(200); 
		memset(fileName,0,200);
		char * date = (char *)malloc(32);
		memset(date,0,32);
		strftime(date, 32, "%Y%m%d",localtime(&t) ); 
		sprintf(fileName,"/%slog.log",date);
		strcat(filePath,fileName);
		FILE *fLog;
		fLog = fopen(filePath,"at+");
		char * logText = (char *)malloc(1024);
		memset(logText,0,1024);
		time_t t1 = time(0);
		char * ti = (char *)malloc(20);
		memset(ti,0,20);
		strftime( ti, 20, "%X",localtime(&t) ); 
		sprintf(logText," %s[%s] : %s\n",ti,level,msg);
		fputs(logText,fLog);
		fclose(fLog);
		free(filePath);
		free(fileName);
		free(logText);
		free(date);
		free(ti);
	}
	
}

void Log::d(char * msg)
{
	writeLog("debug",msg);
}

void Log::e(char * msg)
{
	writeLog("error",msg);
}

void Log::i(char * msg)
{
	writeLog("info",msg);
}

void Log::w(char * msg)
{
	writeLog("warn",msg);
}