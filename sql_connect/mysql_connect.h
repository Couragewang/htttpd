#ifndef _MYSQL_CONNECT_CPP_
#define _MYSQL_CONNECT_CPP_

#include <iostream>
#include <mysql.h>
#include <cstring>

//#define DEFAULT_IP "127.0.0.1"//默认本地
#define DEFAULT_IP "192.168.0.151"//设置的远端mysql存储服务器
#define DEFAULT_PORT 3306 //mysql默认绑定端口
#define USERNAME "dandan" //新建用户
#define PASSWD   "123456" //用户密码
#define DB       "remote_db" //访问的远端数据库


class db_connect{
	private:
		MYSQL *connect_mysql;
		std::string db_ip;
		std::string username;
		std::string passwd;
		std::string db;
		unsigned int port;
		MYSQL_RES *res;
	public:
		db_connect(const std::string &_username,\
				   const std::string &_passwd,\
				   const std::string &_db);
		~db_connect();
		bool mysql_connect();
		bool mysql_operator(std::string &sql);
		bool mysql_down();
		bool mysql_show_table();
};

#endif

