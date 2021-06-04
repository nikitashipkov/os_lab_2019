all : client server

client : head head.h
	gcc -o client MultiModulo client.c -lpthread

server : head head.h
	gcc -o server MultiModulo server.c -lpthread -fsanitize=address

head : head.h
	gcc -o MultiModulo -c MultiModulo.c

clean :
	rm -rf client server MultiModulo
