/*
 * author: Maros Cocula
 * subject:  IPK
 * project:  proj1
 * file: webclient.h
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
#include <regex.h> // regex
#include <regex>

//Constants
#define MAX_redirect 5
#define POZITION 2

// Regex
#define REGEX "^(https?:\\/\\/)?([a-zA-Z0-9\\._%-]+)(:?)([0-9]+)?(\\/?.*)?$"
#define REGEX_CODE "^HTTP/1\\.[0-1] ([0-9][0-9][0-9]).*$"
#define REGEX_NAME "^.*/(.*)$"


typedef struct Url_type{
	int port;
	std::string objekt;
	std::string objekt_bez;
	std::string host;
	std::string protokol;
}URL;

enum regular_array_colum{
    PROTOKOL = 1,
    HOST,
    PORT = 4,
    OBJEKT
};
enum web_stat_code {
	status_continue = 100,
	status_switching_protokol,
	status_ok = 200,
	status_created,
	status_accepted,
	status_non_authoritative_information,
	status_no_content,
	status_reset_content,
	status_partial_content,
	status_multiple_choices = 300,
	status_moved_permanently,
	status_found,
	status_see_other,
	status_not_modified,
	status_use_proxy,
	status_unused,
	status_temporary_redirect,
	status_bad_request = 400,
	status_unauthorized,
	status_payment_required,
	status_forbidden,
	status_not_found,
	status_method_not_allowed,
	status_not_acceptable,
	status_proxy_authentication_required,
	status_request_timeout,
	status_conflict,
	status_gone,
	status_lenght_required,
	status_precondition_failed,
	status_request_entity_too_large,
	status_request_uri,
	status_internal_server_error = 500,
	status_not_implemented,
	status_bad_gateway,
	status_service_unavailable,
	status_gateway_timeout,
	status_HTTP_not_supported
};

enum error {
	error_ok=0,
	error_argv,
	error_socket,
	error_regex,
	error_gethostbyname,
	error_conection_cannot_open,
	error_message_sent,
	error_header,
	error_redirect,
	error_client,
	error_server,
	error_close_socket
};

const char *error_message[] =
{
    "",                                                 
    "Nezadali ste argument do prikazovej riadky",     
    "Socket sa nepodarilo vytvorit",
    "Chyba v regularnom vyraze" ,
    "IP adresa sa nepodarila ziskat" ,
    "Spojenie sa nepodarilo nadviazat" ,
    "Spravu sa nepodarilo odoslat" ,
    "Hlavicku suboru sa nepodarilo precitat", 
    "Minul sa maximalny pocet presmerovani" ,
    "Client Error 4xx" ,
    "Server Error 5xx",  
    "Socket cannot close"           
};