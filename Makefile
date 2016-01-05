httpd:httpd.c
	gcc -o $@ $^

.PHONY:clean
clean:
	rm -rf httpd
