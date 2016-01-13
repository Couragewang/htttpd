#include <stdio.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>

#define SER_PORT 8080
#define SER_IP   "127.0.0.1"

void* thread_run(void *arg)
{
	char buf[1024];
	char c;
	int count = 0;
	memset(buf, '\0', sizeof(buf));
	int fd = *((int*)arg);
	while(1){
    	ssize_t sz = read(fd, &c, 1);
    	if( sz > 0 ){
    		buf[count++] = c;
    	}else{
			break;
		}
	}
	buf[count]= '\0';
	printf("%s\n", buf);
}

int main()
{
	while(1){
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

	    pthread_t id;
	    pthread_create(&id, NULL, thread_run, (void*)&sock);
		memset(buf, '\0', sizeof(buf));
		while(( count = read(0, &buf[sum], sizeof(buf)-sum-1)) > 0){
			sum += count;
		}
		buf[sum]='\0';
		ret = write(sock, buf, strlen(buf));
		if( ret <= 0 ){
			perror("write");
			return 1;
		}
		pthread_join(id, NULL);
		close(sock);
	}
}

