/*
 * author: Maros Cocula
 * subject:  IPK
 * project:  proj1
 * file: webclient.cpp
 */

#include "webclient.h"
// funkcia vypise chybovy kod a spravu, spravy su v subore webclient.h
void error_status(int error)
{
	if (error==0)
		exit(EXIT_SUCCESS);
	else
		std::cerr << "Error " << error << " : " << error_message[error] << std::endl;
	exit(error);
}
// vytvori novy socket API
 int socket_new()
 {
 	int sock=socket(PF_INET, SOCK_STREAM, 0);
 	if(sock<0)
 		error_status(error_socket);
	return sock;
 }
// pociatocna inicializacia URL, ako port sa pouzije 80 ak nie je zadany inac v argumente CLI
void inicialize_url(URL *url)
{
	url->port=80;
	url->host.clear();
	url->objekt.clear();
}
// nahradi medzery v URL
std::string replace_whitespace(std::string string)
{
    while(string.find(" ")!=std::string::npos){
        string.replace(string.find(" "), 1, "%20");
    }
    while(string.find("~")!=std::string::npos){
        string.replace(string.find("~"), 1, "%7E");
    }
    return string;
}
// zparsuje argumenty a naplni strukturu URL 
void parse_argv(std::string argument, URL *url)
 {
 	const char * argv;
 	argv = argument.c_str();
    regex_t regular;
    regmatch_t regular_array[OBJEKT+1]; 

    if(regcomp(&regular, REGEX, REG_EXTENDED) != 0){
        error_status(error_regex);
    }

    if(regexec(&regular,argv,OBJEKT+1,regular_array,0) == 0)
    {
    	// ak sa v poli roztriedenom regexec nachadza nieco v riadku PROTOKOL, vlozi sa to do url->protokol
        if((regular_array[PROTOKOL].rm_eo - regular_array[PROTOKOL].rm_so) != 0){
          url->protokol.append(argv, regular_array[PROTOKOL].rm_so, (regular_array[PROTOKOL].rm_eo - regular_array[PROTOKOL].rm_so));
        }

        // ak sa v poli roztriedenom regexec nachadza nieco v riadku HOST, vlozi sa to do url->host
        if(regular_array[HOST].rm_eo - regular_array[HOST].rm_so != 0){
          url->host.append(argv, regular_array[HOST].rm_so, regular_array[HOST].rm_eo - regular_array[HOST].rm_so);
        }

        // ak sa v poli roztriedenom regexec nachadza nieco v riadku PORT, vlozi sa to do url->port
        if(regular_array[PORT].rm_eo - regular_array[PORT].rm_so != 0){
          std::string tmp_str;
          tmp_str.append(argv,regular_array[PORT].rm_so, regular_array[PORT].rm_eo - regular_array[PORT].rm_so);
          url->port=atoi(tmp_str.c_str());
        }

        // ak sa v poli roztriedenom regexec nachadza nieco v riadku OBJEKT, vlozi sa to do url->objekt
        if(regular_array[OBJEKT].rm_eo - regular_array[OBJEKT].rm_so != 0){
          url->objekt.append(argv, regular_array[OBJEKT].rm_so, regular_array[OBJEKT].rm_eo - regular_array[OBJEKT].rm_so);

        }
    }
    regfree(&regular);
    url->objekt_bez=replace_whitespace(url->objekt);
 }

// nastavi pociatocne nastavenia siete
void set_network(int *socket_use,struct sockaddr_in *socket_addres,URL *url)
 {
 	struct hostent *remoteHost;
    const char* host_name=url->host.c_str();
 	*socket_use=socket_new();
    socket_addres->sin_family = PF_INET;
    socket_addres->sin_port = htons(url->port);
    socket_addres->sin_addr.s_addr = INADDR_ANY;

    remoteHost = gethostbyname(host_name);  // vrati IP adresu
    if(remoteHost==NULL)
    	error_status(error_gethostbyname);

    memcpy( &socket_addres->sin_addr, remoteHost->h_addr, remoteHost->h_length);
    if((connect(*socket_use, (struct sockaddr *)&(*socket_addres), sizeof(*socket_addres)))==(-1)) // pokusi sa nadviazat spojenie
    	error_status(error_conection_cannot_open);
 }
 // vytvori GET poziadavku pre server
 void create_message(std::string *message, URL *url)
 {
 	message->append("GET");
    message->append(" ");
    if(url->objekt_bez=="")
        message->append("/");
    else
        message->append(url->objekt_bez);
    message->append(" HTTP/1.1\r\nHost: ");
    message->append(url->host);
    message->append("\r\nConnection: close\r\n\r\n");
 }
 // precita data s obdrzanej spravy
 void receive_message(int socket_use, std::string *message)
 {
 	char buffer;
 	std::string data;
 		
 	while((read(socket_use,&buffer,1))>0)
    {
        data=data+buffer;
    }

 	message->append(data);

 }
 // vrati hodnotu navratoveho kodu 
 int get_code(std::string message)
 {
 	std::string code;
    std::regex re (REGEX_CODE);
 	std::smatch match;
    std::string tmp_str=message.substr (0,message.find("\r\n"));
 	if(std::regex_search(tmp_str,match,re))
        code=match[1];
  else
       error_status(error_header);
    return atoi(code.c_str());
 }
 // oreze data ak su poslane ako chunky
std::string cute_chunk(std::string data)
{
    int crlf=2; // length of string \r\n
    std::string chunk_lenght;
    int chunk_lenght_int=1;
    std::string data_after="";
    char *p;

    while(chunk_lenght_int!=0){
    chunk_lenght=data.substr(0,data.find("\r\n"));
    chunk_lenght_int=strtol(chunk_lenght.c_str(), &p, 16 );
    data=data.replace(0,data.find("\r\n")+crlf,"");
    data_after.append(data.substr(0,chunk_lenght_int));
    data=data.substr(chunk_lenght_int+crlf,data.length());
    }    

    data_after=data_after.substr(0,data_after.length()-2);

    return data_after;
}
// ziska data, oreze hlavicku zavola funkciu pre chunk ak je potreba (HTTP 1.1)
std::string get_data(std::string message)
{
    std::string data;
    std::string protokol;
    std::string head;
    bool chunk=false;
    protokol = message.substr(0,message.find(" "));
    head=message.substr(0,message.find("\r\n\r\n"));
    if(head.find("chunked") < head.length())
        chunk=true;
    protokol=protokol.substr(protokol.length()-1,protokol.length());
    data=message.substr(message.find("\r\n\r\n")+4,message.length());
    if(atoi(protokol.c_str())==1 && chunk==true)
       {
        data=cute_chunk(data);
        return data;
        }
    else
        return data;
}
// vrati nazov suboru kde sa ma vysledok ulozit
std::string get_name(std::string objekt)
{

    std::string name;
    if(objekt=="")
    {
        name="index.html";
    }
    else
    {
        std::string code;
        std::regex re (REGEX_NAME);
        std::smatch match;
        if(std::regex_search(objekt,match,re))
            name=match[1];
    }
    if (name=="")
        name="index.html";
    return name;
}
// vrati novu URL ziskanu z obdrzanej spravy
std::string get_new_url(std::string message)
{
    std::string url;
    url=message.substr(message.find("Location:"),message.find("Connection:"));
    url=url.substr(10,url.find("\r\n"));
    url=url.substr(0,url.find("\r\n"));
    return url;
}
//*************************************************  main  **********************************************
 int main (int argc, char** argv)
 {
 	// check if argv is OK 
 	if(argc<=1)
 		error_status(error_argv);
//********************************** define needed valuables ********************************************
	
 	int redirect = MAX_redirect;	// max of redirect is 5
 	int socket_use;					// socket
 	int server_code=3;				// status code from server
 	URL url;						// parsed url
 	std::string message;			// message to server
 	std::string incoming_message;	// message from server
 	std::string data;				// incoming data
    std::string file;
    std::string url_start=argv[argc-1];
 	struct sockaddr_in socket_addres;
//****************************************** program start **********************************************

 	do {
 		message.clear();						       // delete old message 
 		incoming_message.clear();
        inicialize_url(&url);                          // inicialize url struct
        parse_argv(url_start,&url);                    // parse argument 
        set_network(&socket_use,&socket_addres,&url);
 		memset(&socket_addres,0,sizeof(socket_addres));
 		create_message(&message,&url);
 		if(write(socket_use, message.c_str(), message.length()) < 0 )
 		{
        	error_status(error_message_sent);
    	}
    	receive_message(socket_use,&incoming_message); // obdrzi spravu zo servru
    	server_code=get_code(incoming_message);
        if(server_code == status_ok)                    // ak je status 200 ulozi sa subor
        {   
            data=get_data(incoming_message);
            file = get_name(url.objekt);
            std::ofstream out(file.c_str());
            out << data << std::endl;
            break;
        }
        else if (server_code==status_moved_permanently || server_code==status_found) // 301,302
        {
            redirect--;
            if(redirect==0)
                error_status(error_redirect);
            else
            {
                url_start=get_new_url(incoming_message);
            }
        }
        else if (server_code<status_internal_server_error && server_code>=status_bad_request) // ostatne errory
            error_status(error_client);
        else if (server_code>=status_internal_server_error)
            error_status(error_server);


 	}while(redirect>0);
    if(close(socket_use)<0) // ukonci spojenie
        error_status(error_close_socket);
 	return (EXIT_SUCCESS);	
 }