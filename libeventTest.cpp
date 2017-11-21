// libeventTest.cpp : 定义控制台应用程序的入口点。
//


#include "stdafx.h"
#include <event2/event.h>
#include <event2/buffer.h>
#include <event2/http.h>
#include <pthread.h>
#include "ReadConfig.h"
#ifdef WIN32
#include <Winsock2.h>
#else
#include <fcntl.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#endif // WIN32
#include <stdlib.h>
#include <stdio.h>
#include "dataParser.h"
#include "WorkThread.h"
#define MAXLEN 1024 

#ifdef WIN32
int init_win_socket()
{
	WSADATA wsaData;
	if(WSAStartup(MAKEWORD(2,2) , &wsaData) != 0) 
	{
		return -1;
	}
	return 0;
}
#endif
int httpserver_bindsocket(int port, int backlog);//绑定端口
int httpserver_start(int port, int nthreads, int backlog);//server 开始
void* httpserver_Dispatch(void *arg);//等待事件发生
void httpserver_GenericHandler(struct evhttp_request *req, void *arg);//事件发生句柄
void httpserver_ProcessRequest(struct evhttp_request *req);//处理事件
void initDBConfig();//初始化数据库参数
char * db_ip;//查询数据库ip
char * db_name;//查询数据库登陆name
char * db_psd;//数据库登陆密码
int db_port;//数据库服务器端口
char * t_name;//数据库name

int httpserver_bindsocket(int port, int backlog) {
#ifdef WIN32
	init_win_socket();
#endif
	int r;
	int nfd;
	nfd = socket(AF_INET, SOCK_STREAM, 0);
	if (nfd < 0) return -1;
	int one = 1;
	r = setsockopt(nfd, SOL_SOCKET, SO_REUSEADDR, (char *)&one, sizeof(int));//设置端口复用

	struct sockaddr_in addr;
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	//addr.sin_addr.s_addr = INADDR_ANY;
	addr.sin_port = htons(port);

	r = bind(nfd, (struct sockaddr*)&addr, sizeof(addr));
	if (r < 0) return -1;
	r = listen(nfd, backlog);
	if (r < 0) return -1;

	//设置非阻塞
#ifdef WIN32
	unsigned long flags = 1;
	if (ioctlsocket(nfd,FIONBIO,&flags) < 0)
		return -1;
#else
	int flags;
	if ((flags = fcntl(nfd, F_GETFL, 0)) < 0
		|| fcntl(nfd, F_SETFL, flags | O_NONBLOCK) < 0)
		return -1;
#endif
	return nfd;
}

int httpserver_start(int port,const int nthreads, int backlog) {
	int r, i;
	int nfd = httpserver_bindsocket(port, backlog);
	if (nfd < 0) 
		return -1;
	pthread_t ths[500];//处理线程
	for (i = 0; i < nthreads; i++) {
		struct event_base *base = event_base_new();
		if (base == NULL) 
			return -1;
		struct evhttp *httpd = evhttp_new(base);
		if (httpd == NULL) 
			return -1;
		r = evhttp_accept_socket(httpd, nfd);
		if (r != 0) 
			return -1;
		evhttp_set_gencb(httpd, httpserver_GenericHandler, NULL);
		r = pthread_create(&ths[i], NULL, httpserver_Dispatch, base);
		if (r != 0) 
			return -1;
	}
	for (i = 0; i < nthreads; i++) {
		pthread_join(ths[i], NULL);
	}
	return 0;
}

void* httpserver_Dispatch(void *arg) {
	event_base_dispatch((struct event_base*)arg);
	return NULL;
}

void httpserver_GenericHandler(struct evhttp_request *req, void *arg) {
	httpserver_ProcessRequest(req);
}

void httpserver_ProcessRequest(struct evhttp_request *req) {
	struct evbuffer *buf = evbuffer_new();
	if (buf == NULL) 
		return;
	size_t len1;
	char * requst;

	buf = evhttp_request_get_input_buffer(req);
	len1 = evbuffer_get_length(buf);
	requst = (char*)malloc(len1 + 1);
	memset(requst,0,len1 + 1);
	memcpy (requst, evbuffer_pullup(buf, -1), len1);
	Log::d(requst);

	dataParser * dParser = new dataParser();
	WorkThread * work = new WorkThread(db_ip,db_name,db_psd,db_port,t_name);
	Log::d("start handler");
	work->workProgress(requst);
	struct evbuffer *rep = evbuffer_new();
	if(!rep)
	{
		puts("failed to create response buffer \n");
		Log::e("failed to create response buffer");
		return;
	}
	evhttp_add_header(evhttp_request_get_output_headers(req), "Content-Type", "text/plain;charset=utf-8");
	if(work->isFile)
	{
		FILE *sofile;
		sofile = fopen(work->filePath,"rb");
		unsigned char buf[MAXLEN]; 
		int rc;
		int count = 0;
		while( (rc = fread(buf,sizeof(unsigned char), MAXLEN,sofile)) != 0 )
		{
			evbuffer_add(rep,buf,rc);
			count += rc;
		}
		fclose(sofile);
		free(work->filePath);
	}
	else
	{
		evbuffer_add_printf(rep, "%s\n",work->responseString);
		free(work->responseString);
	}
	
	
	evhttp_send_reply(req, HTTP_OK, "OK", rep);
	if(work != NULL)
	{
		delete work;
	}

	evbuffer_free(rep);
	free(requst);
}
void initDBConfig()
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
};
int main(void) {
	
	initDBConfig();
	
#ifdef WIN32
	httpserver_start(22, 500, 10240);
	WSACleanup();
#else
	httpserver_start(8060, 10, 10240);
#endif
}
