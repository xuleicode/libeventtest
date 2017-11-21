#ifndef _WORKTHREAD_H_
#define _WORKTHREAD_H_
#include"stdafx.h"
#include "include/json/json.h"
#include <map>
using namespace std;
class WorkThread
{
public:
	void workProgress(char * request);//����������
	string workProgressRet(char * requestText);//�����صĴ���������
	void swithRequestType();//����funidѡ������
	void login();//��������ϱ�
	void free_sms();//������Ӫ�̶������ĺ���
	void billig_config();//config�Ʒ�ָ��������Ϣ
	void product_complete();// ֪ͨһ���Ʒ����
	void soft_code();//�ƷѶ�̬���������
	int doInsertDataBase(char * sql);//���ݿ����
	map<int,map<string,string > > doSelectOne(char * sql);//���ݿ��ѯ
	char * responseString;
	//void initDBConfig();//��config�ļ��ж�ȡ���ݿ�����


	WorkThread(char * db_ip,char *db_name,char *db_psd,int db_port,char * t_name)
	{
		this->db_ip = db_ip;
		this->db_name = db_name;
		this->db_psd = db_psd;
		this->db_port = db_port;
		this->t_name = t_name;
	};
	bool isFile;//�Ƿ񷵻��ļ�
	char * filePath;//�����ļ�·��
	void sendFile();//�������ļ�
	//void workProgress(char * request);
protected:
	int requestType;//JSON��funid
	Json::Value requestValue;//������JSON����
	map<int,map <string,string > > response;//���ݿ��ѯ�Ľ����
	char * returnKey;//JSON���ض�Ӧ�ĵ�һ���key
	int returncode;//JSON��һ���Ӧ��value
	char * db_ip;//��ѯ���ݿ�ip
	char * db_name;//��ѯ���ݿ��½name
	char * db_psd;//���ݿ��½����
	int db_port;//���ݿ�������˿�
	char * t_name;//���ݿ�name
	
};
#endif

