#include "mysql_connect.h"

static void print_log(std::string msg)
{
	std::cout<<msg<<std::endl;
}

db_connect::db_connect(const std::string &_username, const std::string &_passwd, const std::string &_db)
{
	connect_mysql = mysql_init(NULL);
	this->db_ip = DEFAULT_IP;
	this->port  = DEFAULT_PORT;
	this->username = _username;
	this->passwd   = _passwd;
	this->db       = _db;
	this->res      = NULL;
}

db_connect::~db_connect()
{
	mysql_down();
}

//链接mysql
bool db_connect::mysql_connect()
{
	//发起连接
	//if(mysql_real_connect(&connect_mysql, db_ip.c_str(), username.c_str(), passwd.c_str(), db.c_str(), port, NULL, 0)){
	//if(1){
	if(mysql_real_connect(connect_mysql, db_ip.c_str(), username.c_str(), passwd.c_str(), db.c_str(), port, NULL, 0)){
		print_log("connect db success");
		return true;
	}else{
		print_log("connect db failed");
		return false;
	}
}

//执行sql指令
bool db_connect::mysql_operator(std::string &sql)
{
	//发起操作
	int ret = mysql_query(connect_mysql, sql.c_str());
	if( ret == 0 ){//success
		print_log("mysql query success");
		return true;
	}else{
		print_log("mysql query failed");
		return false;
	}
}

//定制化查询操作
//显示查询结果， 测试接口
bool db_connect::mysql_show_table()
{
	char table[64][64];
	//该函数内部会进行malloc，所以需要显示的释放资源
	res = mysql_store_result(connect_mysql);
	MYSQL_FIELD *fd;
	MYSQL_ROW sql_row;
	if(res){
		//获取结果行号
		std::cout<<"res rows is : "<<mysql_num_rows(res)<<std::endl;
		int i = 0;
		//获取列名
		for(; fd = mysql_fetch_field(res); i++){
			bzero(table[i], sizeof(table[i]));
			strcpy(table[i], fd->name);
		}
		//获取列号
		int j = mysql_num_fields(res);
		for( i=0; i<j; i++ ){
			std::cout<<table[i]<<"\t"; //显示所有列名
		}
		std::cout<<std::endl;
		//获取具体内容数据, 按行进行获取
		while(sql_row = mysql_fetch_row(res)){
			for(i=0; i<j; i++){
				std::cout<<sql_row[i]<<"\t";
			}
			std::cout<<std::endl;
		}
		return true;
	}
	return false;
}

bool db_connect::mysql_down()
{
	if( NULL != res ){
		mysql_free_result(res);
		mysql_close(connect_mysql);
	}
	return true;
}


#ifdef _DEBUG_

int main()
{
	db_connect db_test(USERNAME, PASSWD, DB);
//	std::cout<<mysql_get_client_info()<<std::endl;
	db_test.mysql_connect();
	std::string sql = "select * from students";
	db_test.mysql_operator(sql);
	db_test.mysql_show_table();
	return 0;
}

#endif
