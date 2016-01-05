#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#define u_short unsigned short
#define _BACK_LOG_ 5

void* accept_request(void *arg)
{
	int fd = (int)arg;
	return NULL;
}

int startup(u_short *port)
{
	int sock = socket(AF_INET, SOCK_STREAM, 0);
	if( sock < 0 ){
		perror("socket");
		return -1;
	}
	struct sockaddr_in local;
	local.sin_family = AF_INET;
	local.sin_port   = htons(*port);
	local.sin_addr.s_addr   = htonl(INADDR_ANY);
	if( bind(sock, (struct sockaddr *)&local, sizeof(local)) < 0 ){
		perror("bind");
		return -2;
	}
	if( listen(sock, _BACK_LOG_) < 0){
		perror("listen");
		return -3;
	}
	return sock;
}

int main()
{
	u_short port  = 8080;
	int server_sock = startup(&port);
	printf("httpd running... port is : %d\n", server_sock);

	struct sockaddr_in client;
	socklen_t cli_len = 0;
	while(1){
		cli_len = sizeof(client);
		int new_fd = accept(server_sock, (struct sockaddr *)&client, &cli_len);
		if( new_fd < -1 ){
			perror("accept");
			break;
		}
		pthread_t id;
		if( pthread_create(&id, NULL, accept_request, new_fd) != 0){
			perror("pthread_create");
		}
	}
	close(server_sock);
	return 0;
}
