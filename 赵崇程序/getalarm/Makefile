#Makefile
tcpc:TCPClient.c
	@gcc -o tcpc TCPClient.c cjson.c -I/usr/local/include -L/usr/local/lib -lhiredis -levent -lm -lpthread
clean:
	@rm -rf tcpc
