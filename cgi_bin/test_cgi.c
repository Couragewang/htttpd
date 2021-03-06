#include <stdio.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

int main()
{
	printf("\n");//消息头和消息体中间有空行，需要注意

	char method[64];
	char query_string[512];
	char content_length[64];
	char buf[1024];
	int length = -1;

	memset(method, '\0', sizeof(method));
	memset(query_string, '\0', sizeof(query_string));
	memset(content_length, '\0', sizeof(content_length));
	memset(buf, '\0', sizeof(buf));

	strcpy(method, getenv("REQUEST_METHOD"));
	if( strcasecmp(method, "GET") == 0 ){
		strcpy(query_string, getenv("QUERY_STRING"));
	}else{//else
		memset(buf, '\0', sizeof(buf));
		strcpy(content_length ,getenv("CONTENT_LENGTH"));
		sscanf(content_length, "%d", &length);//char* -> int
	}
	if(length > 0){
		int sum = 0;
		char c;
		while(sum < length){
			ssize_t sz = read(0, &c, 1);
			if(sz > 0){
				buf[sum]=c;
				sum++;
			}
		}
		buf[sum]='\0';
	}
	printf("<p>:)</p>\n");
}
