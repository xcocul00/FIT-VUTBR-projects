/*
 * author: Maros Cocula
 * subject:  IPK
 * project:  proj2 - Prenos suborov
 * file: server.h
 */

 // libraries
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>  // socket
#include <sys/socket.h>	// socket
#include <netinet/in.h> // socket strcuture
#include <arpa/inet.h>
#include <netdb.h>
#include <cstring>
#include <pthread.h>
#include <signal.h>
#include <cerrno>

  bool ctrl_c_came=false;

  typedef struct {
	sockaddr_in client;
	int new_socket;
} struct_THREAD;

//  struktura pre parsovanie hlavicky 
  typedef struct {
  std::string file;
  bool download=false;
  bool upload=false;
  std::string length;
} struct_IPK_PROTOKOL;


// enum pre chybvoe vystupy programu
  enum error {
	error_ok=0,
	error_argv,
	error_argv_p,
	error_socket,
	error_bind,
	error_listen,
	error_accept,
	error_thread,
	error_close_socket,
  error_cannot_rewrite_code,
  error_message_sent,
};

const char *error_message[] =
{
    "",                                                 
    "Zle zadany parameter",
    "Zle zadany parameter -p",
    "Socket sa nepodarilo vytvorit",
    "Socket sa nepodarilo pomenovat",
    "Nepodarilo sa vytvorit prichadzajuce pripojenia",
    "Spojenie nebolo nadviazane",
    "Nepodarilo sa vytvorit vlakno",
    "Socket sa nepodarilo zatvorit",
    "Nemozete prepisat zdrojove kody servera a klienta",
    "Spravu sa nepodarilo poslat"
};