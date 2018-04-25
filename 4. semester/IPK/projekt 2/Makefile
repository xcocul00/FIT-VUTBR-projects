 # author: Maros Cocula
 # subject:  IPK
 # project:  proj2
 # file: Makefile
 


NAME=all
 
CC=g++
CFLAGS=-std=c++11 -Wextra -Wall -pedantic -g -pthread 

all: server client

client: client.cpp
	$(CC) $(CFLAGS) client.cpp -o client
	
server: server.cpp
	$(CC) $(CFLAGS) server.cpp -o server
	


clean:
	rm -f *.o *.out client server