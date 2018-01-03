#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <fcntl.h> //文件控制定义
#include <termios.h>//终端控制定义
#include <arpa/inet.h>
#include <sys/socket.h>
#include <pthread.h>
#include <signal.h>
#include <hiredis/hiredis.h>
#include <hiredis/async.h>
#include <hiredis/adapters/libevent.h>
#include "cJSON.h"

#define SERVPORT 27888
#define MAXDATASIZE 100
#define UDPPORT 28003

int serial_fd = 0;

pthread_t thread_redis;

void subCallback(redisAsyncContext *c, void *r, void *priv) 
{  
	redisReply *reply = r;  
	if (reply == NULL) return;
	if ( reply->type == REDIS_REPLY_ARRAY && reply->elements == 3 )
	{  
		if ( strcmp( reply->element[0]->str, "subscribe" ) != 0 )
		{ 
			char *p = strstr(reply->element[2]->str,"alarm_report");
			if(NULL == p) return; 

			printf( "Received[%s] channel %s: %s\n",
			(char*)priv,
			reply->element[1]->str,
			reply->element[2]->str);
			char *cc = strstr(reply->element[2]->str, "}");
			char *strJSON = cc + 1;
			int fd_sock;
			struct sockaddr_in server_addr;
			bzero(&server_addr, sizeof(server_addr));
			server_addr.sin_family = AF_INET;
			server_addr.sin_port = htons(UDPPORT);
			server_addr.sin_addr.s_addr = inet_addr("192.168.1.43");
			fd_sock = socket(PF_INET, SOCK_DGRAM, 0);
			sendto(fd_sock, strJSON, strlen(strJSON), 0, (struct sockaddr *)&server_addr, sizeof(server_addr));
			close(fd_sock);
		}
	}
}

void connectCallback(const redisAsyncContext *c, int status) {  
    if (status != REDIS_OK) {  
        printf("Error: %s\n", c->errstr);  
        return;  
    }  
    printf("Connected...\n");  
}  
  
void disconnectCallback(const redisAsyncContext *c, int status) {  
    if (status != REDIS_OK) {  
        printf("Error: %s\n", c->errstr);  
        return;  
    }  
    printf("Disconnected...\n");  
}  
void *redisthread(void *args)
{
	signal(SIGPIPE, SIG_IGN);  
	struct event_base *base = event_base_new();  

	redisAsyncContext *c = redisAsyncConnect("127.0.0.1", 6379);  
	if (c->err) 
	{  
		/* Let *c leak for now... */  
		printf("Error: %s\n", c->errstr);  
		return;  
	}  
  
	redisLibeventAttach(c,base);  
	redisAsyncSetConnectCallback(c,connectCallback);  
	redisAsyncSetDisconnectCallback(c,disconnectCallback);  
	redisAsyncCommand(c, subCallback, (char*) "sub", "SUBSCRIBE ui_com");  
	printf("callback all done!\n");
	event_base_dispatch(base);  
	printf("dispatch done!\n");
}

int main()
{
	printf("Start!\n");

	memset(&thread_redis,0,sizeof(thread_redis));
	pthread_create(&thread_redis,NULL,redisthread,NULL);

	pthread_join(thread_redis,NULL);

	printf("End!\n");

	return(0);
}


