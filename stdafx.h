// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
//

#pragma once



#include <stdio.h>
#include <stdlib.h>
#ifdef WIN32
#include "targetver.h"
#include <tchar.h>
#include <direct.h>
#else
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#endif // _DEBUG
#include <string>
#include <string.h>
#include <malloc.h>
using namespace std;
#include<map>
#include <time.h> 
#include "Log.h"


// TODO: 在此处引用程序需要的其他头文件
