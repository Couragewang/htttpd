.PHONY:all
all: test_cgi form_cgi

test_cgi:test_cgi.c
	gcc -o $@ $^
form_cgi:form_cgi.c
	gcc -o $@ $^

.PHONY:clean
clean:
	rm -f test_cgi form_cgi
.PHONY:output
output:
	cp test_cgi ../output/htdocs/cgi_bin/
	cp form_cgi ../output/htdocs/cgi_bin/
	cp shell_cgi ../output/htdocs/cgi_bin/
