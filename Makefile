httpd:httpd.c
	gcc -w -o $@ $^ -lpthread #-D_DEBUG_

.PHONY:clean
clean:
	rm -rf httpd
