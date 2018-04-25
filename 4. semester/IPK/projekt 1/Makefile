
 # author: Maros Cocula
 # subject:  IPK
 # project:  proj1
 # file: Makefile
 


NAME=webclient

CC=g++
CFLAGS=-std=c++11 -pedantic -Wextra -Wall -Wl,-rpath=/usr/local/lib/gcc49/



$(NAME): $(NAME).cpp
	$(CC) $(CFLAGS) $(NAME).cpp -o $(NAME)

clean:
	rm -f *.o *.out webclient

