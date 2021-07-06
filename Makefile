CC=gcc
CFLAGS=-g

all: echoserver_parallel echoclient
echoserver_parallel: echoserver_parallel.c
	$(CC) $(CFLAGS) -o echoserver_parallel echoserver_parallel.c
echoclient: echoclient.c
	$(CC) $(CFLAGS) -o echoclient echoclient.c
clean:
	rm echoclient echoserver_parallel 
