mysql_connect:mysql_connect.cpp
	g++ -w -o $@ $^ -I./mysql_lib/include -L./mysql_lib/lib -lmysqlclient -D_DEBUG_  #-static
.PHONY:clean
clean:
	rm -rf mysql_connect

