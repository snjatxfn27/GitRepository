#Makefile
getsnap:getsnap.c
	@gcc -o getsnap getsnap.c cjson.c -I/usr/local/include -L/usr/local/lib -lhiredis -levent -lm -lpthread
clean:
	@rm -rf getsnap
