#include <stdio.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>

#define SER_PORT 8080
#define SER_IP   "127.0.0.1"

int main()
{
	int sock = socket(AF_INET, SOCK_STREAM, 0);
	if( sock < -1 ){
		perror("socket");
		return 1;
	}
	struct sockaddr_in remote;
	remote.sin_family = AF_INET;
	remote.sin_port   = htons(SER_PORT);
	inet_pton(AF_INET, SER_IP, &remote.sin_addr.s_addr);//string -> net int

	socklen_t len = sizeof(struct sockaddr_in);
	int ret = connect(sock, (struct sockaddr*)&remote, len);
	if(ret == -1){
		perror("connect");
		return 1;
	}

	char buf[10240];
	ssize_t count = 0;
	ssize_t sum = 0;
	while(1){
		memset(buf, '\0', sizeof(buf));
		while(( count = read(0, &buf[sum], sizeof(buf)-sum-1)) > 0){
			sum += count;
		}
		buf[sum]='\0';
		write(sock, buf, strlen(buf));
		ssize_t sz = read(sock, buf, sizeof(buf));
		if( sz > 0 ){
			buf[sz] = '\0';
		}
		printf("%s\n", buf);
	}
}




