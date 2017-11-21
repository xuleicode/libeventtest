#include "stdafx.h"
#include "dataParser.h"
#include <iostream>
#ifdef WIN32
#include <Windows.h> //Windowsͷ�ļ�
#endif
#include <fstream>
typedef unsigned char byte;
using std::string;
#include <sstream>
#include "include/json/json.h"
#include <iostream>
#ifdef WIN32
#pragma comment(lib, "json_vc71_libmtd.lib")
#endif


Json::Value dataParser::jsonParser(char * req)
{
	Json::Reader reader;//json����
	Json::Value value;//��ʾһ��json��ʽ�Ķ���
	reader.parse(req,value,false);
	return value;
}



int dataParser::makeJson(char * returnKey,int returncode,map<int,map<string,string> >rep,char * itemname,char * repString)
{
	Json::Value root;
	Json::FastWriter writer;
	root[returnKey] = returncode;
	if(rep.empty())//mapΪ�գ���JSONû������
	{
	}
	else if(rep.size() == 1)//mapֻ��һ�����ݣ���map����ֱ�Ӽ���JSON��
	{
		map<int ,map<string,string> >::iterator iters;
		iters = rep.find(0);
		map<string,string> repLine = iters->second;
		map<string,string>::iterator iter;
		for(iter = repLine.begin();iter != repLine.end();++iter)
		{
			char * key =(char *) iter->first.c_str();
			char * values = (char *) iter->second.c_str();
			root[key] = values;
		}
	}
	else//map�������ݣ���map���ݺϳ�һ����ֵ��
	{
		Json::Value arrayObj;
		Json::Value item;
		map<int ,map<string,string> >::iterator iters;
		for(iters = rep.begin();iters != rep.end();++iters)
		{
			map<string,string>repLine= iters->second;
			map<string,string>::iterator iter;
			for(iter = repLine.begin();iter != repLine.end();++iter)
			{
				char * key =(char *) iter->first.c_str();
				char * values = (char *) iter->second.c_str();
				item[key] = values;
			}
			arrayObj.append(item);
		}
		root[itemname] = arrayObj;
	}
	string ret(writer.write(root));
	memcpy(repString,ret.c_str(),ret.size());
	printf("json = %s\n",repString);
	return 0;
}



