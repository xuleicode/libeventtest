#include "stdafx.h"
#include "WorkThread.h"
#include "map"
#include "dataParser.h"
#include "ReadConfig.h"
#include "event.h"
#include "include/mysql/mysql.h"
#ifdef WIN32
#include <io.h>
#endif


void WorkThread::login()
{
	
	char * sql = (char *)malloc(1024);
	memset(sql,0,1024);
	strcpy(sql,"insert into t_product_login (productid,imsi,imei,ua) values ('");
	returnKey = "ResultCode";
	try
	{
		if(requestValue["AppId"].isConvertibleTo(Json::intValue))
		{			
			Log::e("AppId connot parser!");
			returnKey = 0;
			return;
		}
		else
		{
			strcat(sql,requestValue["AppId"].asCString());
		}
	}catch(exception e)
	{
		Log::e((char *)e.what());
		returncode = 0;
		free(sql);
		return;
	}
	strcat(sql,"','");	
	try
	{
		if(requestValue["imsi"].isConvertibleTo(Json::intValue))
		{			
			Log::e("imsi connot parser!");
			returnKey = 0;
			return;
		}
		else
		{

			strcat(sql,requestValue["imsi"].asCString());
		}	
	}catch(exception e)
	{
		Log::e((char *)e.what());
		returncode = 0;
		free(sql);
		return;
	}
	strcat(sql,"','");
	try
	{
		if(requestValue["imei"].isConvertibleTo(Json::intValue))
		{			
			Log::e("imei connot parser!");
			returnKey = 0;
			return;
		}
		else
		{

			strcat(sql,requestValue["imei"].asCString());
		}
		

	}catch(exception e)
	{
		Log::e((char *)e.what());
		returncode = 0;
		free(sql);
		return;
	}
	
	strcat(sql,"','");
	try
	{
		if(requestValue["ua"].isConvertibleTo(Json::intValue))
		{			
			Log::e("ua connot parser!");
			returnKey = 0;
			return;
		}
		else
		{

			strcat(sql,requestValue["ua"].asCString());
		}
		
	}catch(exception e)
	{
		Log::e((char *)e.what());
		returncode = 0;
		free(sql);
		return;
	}
	strcat(sql,"') ON DUPLICATE KEY UPDATE time=NOW()");
	int result = doInsertDataBase(sql);
	if(result)
	{
		returncode = 1;
	}
	else
	{
		returncode = 0;
	}
	
	free(sql);

}



int WorkThread::doInsertDataBase(char *sql)
{
	MYSQL mysql_conn;
	int result = 0;
	Log::d(sql);
	if(mysql_init(&mysql_conn)!=NULL)
	{
		if(mysql_real_connect(&mysql_conn,db_ip,db_name,db_psd,t_name,db_port,NULL,0)!=NULL)
		{

			printf("connect success!!!\n");
			if ( mysql_set_character_set( & mysql_conn, "utf8" ) ) 
			{ 
				Log::e("insert set charset error");
			} 
			mysql_query(&mysql_conn,"set character set utf8");
			result = mysql_query(&mysql_conn,sql);
			if(!result)
			{
				Log::d("Inserted  rows");
			}else
			{
				Log::e("insert failed");
			}

		}else
		{
			Log::e("insert connect failed");
		}
		
	}else
	{
		Log::e("insert initial failed");
	}
	mysql_close(&mysql_conn);
	return result;
}

map<int,map<string,string> > WorkThread::doSelectOne(char * sql)
{
	map<int,map<string,string> > resMap;
	MYSQL mysql_conn;
	MYSQL_RES *mysql_result;
	MYSQL_ROW mysql_row;
	MYSQL_FIELD *my_field; 
	int f1,f2,num_row,num_col;
	printf("start select !");
	Log::d(sql);
	if(mysql_init(&mysql_conn)!=NULL)
	{
		if(mysql_real_connect(&mysql_conn,db_ip,db_name,db_psd,t_name,db_port,NULL,0)!=NULL)
		{
			if ( mysql_set_character_set( & mysql_conn, "utf8" ) ) { 
				Log::e("select set charset error");
			} 
			mysql_query(&mysql_conn,"set character set utf8");
			if(mysql_query(&mysql_conn,sql)==0)
			{
				mysql_result=mysql_store_result(&mysql_conn);
				mysql_close(&mysql_conn);
				num_row=mysql_num_rows(mysql_result);
				num_col=mysql_num_fields(mysql_result);
				printf("num_col=%d!!!\n",num_col);
				string colname[10];
				for(f2=0;f2<num_col;f2++)
				{
					my_field = mysql_fetch_field(mysql_result);
					char* key = (char *)malloc(20480);
					strcpy(key,my_field->name);
					string s_key(key);
					colname[f2] =s_key;
					free(key);
				}
				for(f1=0;f1<num_row;f1++)
				{
					mysql_row=mysql_fetch_row(mysql_result);
					if(mysql_row != NULL)
					{
						map<string,string>rowMap;
						for(f2=0;f2<num_col;f2++)
						{
							string values(mysql_row[f2], strlen(mysql_row[f2]));
							rowMap.insert(map<string,string>::value_type(colname[f2],values));
						}
						resMap.insert(map<int,map<string,string> >::value_type(f1,rowMap));

					}
				}
				
				
				mysql_free_result(mysql_result);	
			}else
			{
				Log::e("query error");
			}
		}else
		{
			Log::e("query connect error");
		}
	}else
	{
		Log::e("query initial error");
	}
	
	//mysql_close(&mysql_conn);

	return resMap;
}

void WorkThread::free_sms()
{
	char * sql = (char *)malloc(1024);
	memset(sql,0,1024);
	strcpy(sql,"select free_sms_port as SmsPort,sms_content as SmsContent  from t_free_sms_config");
	response = doSelectOne(sql);
	returnKey = "ResultCode";
	if(response.empty())
	{
		returncode = 1;
	}
	else
	{
		returncode = 0;
	}
	free(sql);
}

void WorkThread::billig_config()
{
	char * sql = (char *)malloc(1024);
	memset(sql,0,1024);
	try
	{
		sprintf(sql,"select config_content,ConVer from t_billing_config  where AppId ='%s' and ConVer > %d",requestValue["AppId"].asCString(),requestValue["ConVer"].asInt());
		response = doSelectOne(sql);
	}catch(exception e)
	{
		Log::e((char *)e.what());
	}
	
	returnKey = "NeedUpdate";
	if(response.empty())
	{
		returncode = 0;
	}
	else
	{
		returncode = 1;
	}
	free(sql);

}

void WorkThread::product_complete()
{
	returnKey = "ResultCode";
}

void WorkThread::soft_code()
{


	char * sql = (char *)malloc(1024);
	memset(sql,0,1024);
	try
	{
		sprintf(sql,"select NewSoVer,DnlUrl from t_billing_soft_update where NewSoVer > %d",requestValue["SoVer"].asInt());
		response = doSelectOne(sql);
	}catch(exception e)
	{
		Log::e((char *)e.what());
	}
	
	
	returnKey = "NeedUpdate";
	if(response.empty())
	{
		returncode = 0;
	}
	else
	{
		returncode = 1;
	}
	free(sql);
	
}

void WorkThread::swithRequestType()
{
	switch(requestType)
	{
	case  0x0000001:
		{
			login();
			break;
		}
	case 0x0000002:
		{
			billig_config();
			break;
		}
	case 0x0000003:
		{
			product_complete();
			break;
		}
	case 0x0000004:
		{
			soft_code();
			break;
		}
	case 0x0000005:
		{
			soft_code();
			break;
		}
	case 0x0000006:
		{
			sendFile();
			break;
		}

	}

}

void WorkThread::sendFile()
{
	isFile = true;
	filePath = (char*)malloc(1024);
	memset(filePath,0,1024);
	getcwd(filePath,1024);
	strcat(filePath,"/downloadFile/");
	strcat(filePath,requestValue["filename"].asCString());
	if( (access(filePath, 0 )) == -1 )
	{
		memset(filePath,0,1024);
		getcwd(filePath,1024);
		strcat(filePath,"/downloadFile/eror.txt");
	}
	Log::d(filePath);
}




void WorkThread::workProgress(char * requestText)
{

	isFile = false;
	dataParser * dParser = new dataParser();
	requestValue = dParser->jsonParser(requestText); 
	try
	{
		requestType = requestValue["funid"].asInt();
		swithRequestType();
	}catch(exception e)
	{
		Log::e((char *)e.what());
		returnKey = "error";
		returncode = 1;
	}
	
	if(!isFile)
	{
		responseString = (char * )malloc(1024*1024);
		memset(responseString,0,1024*1024);
		dParser->makeJson(returnKey,returncode,response,"SmsCons",responseString);
		//Log::d(responseString);
	}
	
	
	if(dParser != NULL)
	{
		delete dParser;
	}
	
	
}




/*void WorkThread::initDBConfig()
{
	ReadConfig * readConfig = new ReadConfig();
	db_ip = (char *)malloc(64); 
	db_name = (char *)malloc(64); 
	db_psd = (char *)malloc(64); 
	char * str_db_port = (char *)malloc(64); 
	t_name = (char *)malloc(64); 
	readConfig->ReadConfigFile("cls_server", "db_ip", db_ip);
	readConfig->ReadConfigFile("cls_server", "db_name", db_name);
	readConfig->ReadConfigFile("cls_server", "db_psd", db_psd);
	readConfig->ReadConfigFile("cls_server", "db_port", str_db_port);
	db_port = atoi(str_db_port);
	readConfig->ReadConfigFile("cls_server", "t_name", t_name);
	free(str_db_port);
	if(readConfig != NULL)
	{
		delete readConfig;
	}
};*/

