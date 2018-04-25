/*
 * Author : Maros Cocula (xcocul00)
 * Subject : ISA 
 * Project : Programovani sitove sluzby -> FTP client
 * Year : 2016
 * file : fclient.h
 */

#include <stdlib.h>
#include <stdio.h>
#include <getopt.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>
#include <algorithm>
#include <ifaddrs.h>
 #include <signal.h>



typedef struct Arguments{
	bool s = false;
	bool c = false;
	bool a = false;
	bool p = false;
	bool u = false;
	bool d = false;
	bool r = false;
	bool P = false;
	std::string server;
	std::string login;
	std::string port;
	std::string pasive_port;
	std::string active_port;
	std::string sent;
	std::string get;
	std::string remove;
	std::string path;
}argument;

typedef struct Login{
	std::string username;
	std::string password;
}login;

enum errors{
	err_ok=0,
	err_parser,
	err_socket,
	err_socket_close,
	err_login,
	err_port,
	err_ip_address,
	err_connect,
	err_read,
	err_server_not_ready,
	err_user,
	err_quit,
	err_file_not_exist,
	err_direct,
	err_port_active,
	err_sigint,
	err_get_struct
};

const char* err_message[] = {
	"",
	"Bad params",
	"Cannot create socket",
	"Cannot close socket",
	"Cannot open user identification file",
	"Cannot convert port to integer",
	"Invalid adress of host in parameter -s",
	"Cannot connect to server",
	"Connection failed",
	"Server is not ready for comunications",
	"Login incorrect",
	"Cannot quit connection with server",
	"File does not exist",
	"Cannot change directory",
	"Bad active port parameter",
	"SigInt signal came",
	"Cannot get directory information"
};



/* PROTOTYPS */
void error_status(int err);
void help();
void parse_argv(int argc, char** arguments, argument *args);
int create_socket();
void parse_login(login *logs, std::string args);
int parse_port(std::string port);
void set_network(int socket,int port,struct sockaddr_in *socket_address,argument args);
void autentificate(int socket, login logs);
int receive (int socket);

void quit(int socket);
void ftp_passive(int socket,argument *args);
void ftp_active(int socket,argument *args);
void upload (int socket,int socket_pasive, argument args);
void download(int socket,int socket_pasive, argument args);
void remove(int socket, argument args);
void print_structure(int socket,int socket_pasive, argument args);
void change_path(int socket, argument args);

void func_ctrl_c_came(int signal_came);

int count_row (std::string data);
std::string recurse(std::string table, int row_cnt, int socket,std::string name, argument args);
std::string cut_table(int row_cnt, std::string data);
std::string recv_table(std::string name,int socket, int socket_pasive);


void debug_param(argument args, login logs);