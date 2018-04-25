# Author : Maros Cocula (xcocul00)
# Subject : ISA 
# Project : Programovani sitove sluzby -> FTP client
# Year : 2016
# file : Makefile

NAME=fclient
CC=g++
CFLAGS=-std=c++11 -Wextra -Wall -pedantic -g -pthread -Wl,-rpath=/usr/local/lib/gcc49/

$(NAME): fclient.cpp
	$(CC) $(CFLAGS) fclient.cpp -o fclient

clean : 
	-rm -f *.o *.out fclient

