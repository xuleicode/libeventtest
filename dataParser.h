#ifndef _DATAPARSER_H_
#define _DATAPARSER_H_

#include "stdafx.h"
#include "include/json/json.h"
#include <map>
using namespace std;

class dataParser
{
public:
	Json::Value jsonParser(char * req);//����JSONΪJson::Value
	int makeJson(char * returnKey,int returncode,map<int,map<string,string> >rep,char * itemname,char* repString);//�������ص����ɷ���JSON
};
#endif