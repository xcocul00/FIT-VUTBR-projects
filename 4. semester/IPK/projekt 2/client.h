/*
 * author: Maros Cocula
 * subject:  IPK
 * project:  proj2 - Prenos suborov
 * file: client.h
 */

 // libraries
#include <iostream>
#include <string>
#include <sys/types.h>  // socket
#include <sys/socket.h>	// socket
#include <netinet/in.h> // socket strcuture
#include <arpa/inet.h>
#include <netdb.h>
#include <sstream>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <algorithm>
#include <exception>

// struktura pre parsovanie argumentov CLI
typedef struct Arguments{
	int port;
	std::string host;
	std::string file;
	std::string file_bez;
	bool read=0;
	bool write=0;
}argument;

// enum pre chybvoe vystupy programu
 enum error {
	error_ok=0,
	error_argv,
	error_argv_hp,
	error_argv_ud,
	error_socket,
	error_close_socket,
	error_gethostbyname,
	error_conection_cannot_open,
	error_message_sent,
	file_not_exist,
	trying_to_replace,
	upload_failed,
};

const char *error_message[] =
{
    "",                                                 
    "Zle zadane parametre",
    "Zle zadany parameter -h alebo -p",
    "Zle zadany parameter -u alebo -d",
    "Socket sa nepodarilo vytvorit",
    "Socket sa nepodarilo zatvorit",
    "Adresa hosta nenajdena (gethostbyname)",
    "Spojenie sa nepodarilo nadviazat",
    "Spravu sa nepodarilo poslat",
    "Subor neexistuje",
    "Snazite sa prepisat zdrojove kody",
    "Upload failed"
};