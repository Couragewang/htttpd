ROOTPATH=$(shell pwd)
BIN_DIR=cgi_bin

.PHONY:all
all:httpd demo_client cgi

httpd:httpd.c
	gcc  -o $@ $^ -lpthread #-D_DEBUG_
demo_client:demo_client.c
	gcc  -o $@ $^ 

.PHONY:cgi
cgi:
	for name in `echo $(BIN_DIR)`
	do
		cd $(name)
		make output;
		cd -
	done

.PHONY:clean
clean:
	rm -rf httpd demo_client
