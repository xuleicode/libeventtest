#ifndef _WORKTHREAD_H_
#define _WORKTHREAD_H_
#include"stdafx.h"
#include "include/json/json.h"
#include <map>
using namespace std;
class WorkThread
{
public:
	void workProgress(char * request);//处理主程序
	string workProgressRet(char * requestText);//带返回的处理主程序
	void swithRequestType();//根据funid选择处理方法
	void login();//处理登入上报
	void free_sms();//处理运营商短信中心号码
	void billig_config();//config计费指令配置信息
	void product_complete();// 通知一条计费完成
	void soft_code();//计费动态库升级检查
	int doInsertDataBase(char * sql);//数据库插入
	map<int,map<string,string > > doSelectOne(char * sql);//数据库查询
	char * responseString;
	//void initDBConfig();//从config文件中读取数据库配置


	WorkThread(char * db_ip,char *db_name,char *db_psd,int db_port,char * t_name)
	{
		this->db_ip = db_ip;
		this->db_name = db_name;
		this->db_psd = db_psd;
		this->db_port = db_port;
		this->t_name = t_name;
	};
	bool isFile;//是否返回文件
	char * filePath;//返回文件路径
	void sendFile();//处理返回文件
	//void workProgress(char * request);
protected:
	int requestType;//JSON中funid
	Json::Value requestValue;//解析的JSON内容
	map<int,map <string,string > > response;//数据库查询的结果集
	char * returnKey;//JSON返回对应的第一项的key
	int returncode;//JSON第一项对应的value
	char * db_ip;//查询数据库ip
	char * db_name;//查询数据库登陆name
	char * db_psd;//数据库登陆密码
	int db_port;//数据库服务器端口
	char * t_name;//数据库name
	
};
#endif

