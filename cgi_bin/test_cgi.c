#include <stdio.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

int main()
{
	printf("this is a cgi test\n");
	printf("method is : %s\n", getenv("REQUEST_METHOD"));
	if( strcasecmp(getenv("REQUEST_METHOD"), "GET") == 0 ){
		printf("query string : %s\n", getenv("QUERY_STRING"));
	}else{//else
		char buf[1024];
		memset(buf, '\0', sizeof(buf));
		printf("content length : %s\n", getenv("CONTENT_LENGTH"));
		int length = -1;
		if(getenv("CONTENT_LENGTH") == NULL){
			return 1;
		}
		sscanf(getenv("CONTENT_LENGTH"), "%d", &length);//char* -> int
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
		printf("%s\n", buf);
		printf("haha\n");
	}
}
