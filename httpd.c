#include "httpd.h"

void clear_header(int client)
{
}

void echo_file(int client, char *path_name, int file_len)
{
	clear_header(client);
	int fd = open(path_name, O_RDONLY, 0);
	if( fd < 0 ){
		return_back_errno();
	}else{
		sendfile(fd, client, 0, &file_len, NULL, 0);//unix,os x
		//sendfile(client, fd, NULL, file_len);//Linux
	}
}

void exe_cgi()
{
}

void return_back_errno()
{
	return;
}

int get_line(int client, char buf[], int buflen)
{
	if( !buf || buflen < 0 ){
		return -1;
	}
	int i = 0;
	char c = '\0';
	int n = 0;
	while( (i < buflen-1) && (c != '\n') ){
		n = recv(client, &c, 1, 0);
		if( n > 0 ){
			if( c == '\r' ){//将所有的\r\n(windows) 和 \r(mac os) 统一转化成\n
				n = recv(client, &c, 1, MSG_PEEK); //对下一个字符制作检测，在内核接受缓冲区中，并不清除该字符
				if( n > 0 && c == '\n' ){
					recv(client, &c, 1, 0);//读取该字符
				}else{
					c = '\n';
				}
			}
			buf[i++] = c;
		}else{
			c = '\n';
		}
	}
	buf[i] = '\0';
	return i;
}

void* accept_request(void *arg)
{
	int fd = (int)arg;
	char buf[COMM_SIZE];
	char method[COMM_SIZE/5];
	char url[COMM_SIZE/2];
	char version[COMM_SIZE/5];
	char path[COMM_SIZE/2];
	int cgi = 0;
	char *query_string = NULL;
	memset(buf, '\0', sizeof(buf));
	memset(method, '\0', sizeof(method));
	memset(url, '\0', sizeof(url));
	memset(version, '\0', sizeof(version));
	memset(path, '\0', sizeof(path));

#ifdef _DEBUG_
	while(get_line(fd, buf, sizeof(buf)) > 0){
		printf("%s", buf);
		fflush(stdout);
		memset(buf, '\0', sizeof(buf));
	}
#else
	if(get_line(fd, buf, sizeof(buf)-1) < 0){
		return_back_errno();
		return NULL;
	}
	//get method
	int i = 0;
	int j = 0;
	while( !isspace(buf[i]) && j < sizeof(method)-1 && i < sizeof(buf)){
		method[j] = buf[i];
		j++, i++;
	}
	if(strcasecmp("POST", method) != 0 && strcasecmp("GET", method) != 0){
		return_back_errno();
		return NULL;
	}
	if( strcasecmp("POST", method) == 0 ){
		cgi = 1;
	}

	//skip backspace
	while( isspace(buf[i]) ){
		i++;
	}
	//get url
	j = 0;
	while( !isspace(buf[i]) && j < sizeof(url)-1 && i < sizeof(buf) ){
		url[j] = buf[i];
		j++, i++;
	}
	if( strcasecmp("GET", method) == 0 ){
		query_string = url;
		while( *query_string != '?' && *query_string != '\0'){
			query_string++;
		}
		if( *query_string ==  '?' ){
			*query_string = '\0';//url = 资源路径 ＋ 所请求资源参数
			query_string++;
			cgi = 1;//既然要传参数，说明肯定不是简单的请求html文档，是需要交互的cgi
		}//至此， url表明资源路径， query_string表明所请求资源参数
	}

	sprintf(path,"htdocs%s", url);
	if(path[strlen(path)-1] == '/' ){//目录
		strcat(path, MAIN_PAGE);
	}

	struct stat st;
	if( stat(path, &st) < 0 ){//文件不存在
		clear_header(fd);
		return_back_errno();
	}else{//文件存在，并且成功获取了对应的st信息
		if(S_ISDIR(st.st_mode)){//指定文件是目录
			strcat(path, "/index.html");
		}else if((st.st_mode & S_IXUSR) ||\
				 (st.st_mode & S_IXGRP) ||\
				 (st.st_mode & S_IXOTH)){//指定文件具有可执行权限
			cgi = 1;
		}
		if(!cgi){//不是cgi,仅仅是一个普通的html请求，回发指定文件即可。
			echo_file(fd, path, st.st_size);
		}else{//执行CGI
			exe_cgi();
		}
	}
	close(fd);//http协议是一个无状态协议，所以执行完成之后，需要关闭链接
//	//skip backspace
//	while( isspace(buf[i]) ){
//		i++;
//	}
//	//get version
//	j = 0;
//	while(buf[i] != '\n' && j < sizeof(version)){
//		version[j] = buf[i];
//		i++, j++;
//	}

#endif
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

	int flag = 1;
	setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, flag, sizeof(flag));

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
	printf("httpd running... sock is : %d\n", server_sock);

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
		if( pthread_create(&id, NULL, accept_request, (void*)new_fd) != 0){
			perror("pthread_create");
		}
	}
	close(server_sock);
	return 0;
}
