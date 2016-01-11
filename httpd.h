#ifndef _HTTPD_
#define _HTTPD_

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
//#include <sys/sendfile.h>
#include <sys/types.h>//unix
#include <sys/socket.h>//unix
#include <sys/uio.h>//unix
#include <sys/stat.h>
#define u_short unsigned short
#define _BACK_LOG_ 5
#define COMM_SIZE 1024
#define MAIN_PAGE "index.html"

void clear_header(int client);
void echo_file(int client, char *path_name, int file_len);
void exe_cgi();
void return_back_errno();
int get_line(int client, char buf[], int buflen);
void* accept_request(void *arg);
int startup(u_short *port);

#endif
