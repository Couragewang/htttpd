ROOTPATH=$(shell pwd)
BIN_DIR=cgi_bin
OPT=-w
#构建
.PHONY:all
all:httpd demo_client cgi

httpd:httpd.c
	gcc $(OPT) -o $@ $^ -lpthread #-D_DEBUG_
demo_client:demo_client.c
	gcc $(OPT) -o $@ $^ 

.PHONY:cgi
cgi:
	for name in `echo $(BIN_DIR)`;\
	do\
		cd $$name;\
		make;\
		cd -;\
	done
#发布
.PHONY:output
output:
	mkdir -p output/cgi_bin
	cp -rf htdocs output/
	cp demo_client output/
	cp httpd output/
	for name in `echo $(BIN_DIR)`;\
	do\
		cd $$name;\
		make output;\
		cd -;\
	done
#清理
.PHONY:clean
clean:
	rm -rf httpd demo_client;rm -rf output;\
	for name in `echo $(BIN_DIR)`;\
	do\
		cd $$name;\
		make clean;\
		cd -;\
	done
