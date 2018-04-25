/*
 * author: Maros Cocula
 * subject:  IPK
 * project:  proj2 - Prenos suborov
 * file: client.cpp
 */

 #include "client.h"

 // funkcia vypise chybovy kod a spravu, spravy su v subore client.h
void error_status(int error)
{
	if (error==0)
		exit(EXIT_SUCCESS);
	else
		std::cerr << "Error " << error << " : " << error_message[error] << std::endl;
	exit(error);
}
// funkcia ktora vymeni medzeru a znak ~ za kodove znaky
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
//funkcia ktora rozparsuje vstupne argumenty, vrati ich v strukture "argument"
void parse_arg (int arg, char** arguments, argument *args)
{
	if(strcmp(arguments[1],"-h") && strcmp(arguments[3],"-h"))
		error_status(error_argv_hp);
	if(strcmp(arguments[1],"-p") && strcmp(arguments[3],"-p"))
		error_status(error_argv_hp);
	if (arg>4)
	{
		if(strcmp(arguments[5],"-d")){
				if(strcmp(arguments[5],"-u"))
					error_status(error_argv_ud);
				else
					args->write=1;
			}
		else
			args->read=1;
		args->file = arguments[6];
		}

	for (int i=1; i<=arg ; i++)
	{
		if(!strcmp(arguments[i],"-h"))
			args->host=arguments[i+1];
		if(!strcmp(arguments[i],"-p"))
			{
				if(atoi(arguments[i+1])!=0)
					args->port=atoi(arguments[i+1]);
				else
					error_status(error_argv_hp);	
			}
	}
	args->file_bez=replace_whitespace(args->file);
}

// vytvori novy socket API
 int socket_new()
 {
 	int sock=socket(AF_INET, SOCK_STREAM, 0);
 	if(sock<0)
 		error_status(error_socket);
	return sock;
 }


// nastavi pociatocne nastavenia siete
void set_network(int socket_use,struct sockaddr_in *socket_addres,argument *args)
 {
 	struct hostent *remoteHost;
    const char* host_name=args->host.data();
    socket_addres->sin_family = AF_INET;
    socket_addres->sin_port = htons(args->port);

    remoteHost = gethostbyname(host_name);  // vrati IP adresu
    if(remoteHost==NULL)
    	error_status(error_gethostbyname);
    memcpy( &socket_addres->sin_addr, remoteHost->h_addr, remoteHost->h_length);
    if(connect(socket_use, (struct sockaddr *)&(*socket_addres), sizeof(*socket_addres))<0) // pokusi sa nadviazat spojenie
    	error_status(error_conection_cannot_open);

 }
 // vytvori message pre download
 std::string create_message(argument args)
 {
 	if(args.read)
 	{
 		return "DOWNLOAD "+args.file_bez+" END OF CONNECTION";
 	}
 	return " ";
 }

// funkcia ktora precita chybovy status zo servra a vrati ho ako chybu
void print_error(std::string code)
{
	code=code.substr(code.find("ERR")+3,code.size()); 
	std::string::size_type size;
	int code_int = std::stoi (code,&size);
	switch (code_int)
	{
		case 1 : error_status(file_not_exist);
		case 2 : error_status(trying_to_replace);
		case 3 : error_status(upload_failed);
		default : error_status(file_not_exist);
	} 
}
// funkcia ktora zabezbecuje download zo servra
 void download (int socket, argument args)
 {	
 	std::string message=create_message(args);
 	std::string income_message="";
 	std::string code;
	bool found_end = false;
	char buffer;
	std::string data;
	std::string income_data;
	std::string size;

 	if(send(socket, message.data(), message.size(),0) < 0 )
 		{
        	error_status(error_message_sent);
    	}
   
    do{
    	if(recv(socket,&buffer,1,0)>0){
    		data=data+buffer;
    	}
    	if(data.find("END OF CONNECTION") != std::string::npos){
    		found_end = true;
    	}

    }while(!found_end);
    income_message.append(data);
    code = income_message.substr(0,income_message.find(" "));
   
    if(code == "OK")
    {
    	size=income_message.substr(income_message.find(" ")+1,income_message.find("END"));
    	size=size.substr(0,size.find(" "));
    	message="OK END OF CONNECTION";
    	if(send(socket, message.data(), message.size(),0) < 0 )
 		{
        	error_status(error_message_sent);
    	}
    	data.clear();
    	int length = stoi(size);
    	do{
    	
    	if(recv(socket,&buffer,1,0)>0){
    		data=data+buffer;
    		std::ofstream out(args.file.data(),std::ios::binary);
        	out << data ;
    	}
    	length--;
    	}while(length!=0);

    }
    else
    	print_error(code);
 }

// funkcia ktora zabezbecuje upload zo servra
  void upload (int socket, argument args)
 {
	std::string message="";
	std::ifstream file;
 	std::string income_message="";
	bool found_end = false;
	char buffer;
	std::string data;

	file.open(args.file, std::ios::binary);
	if(!file.is_open())
	{
		error_status(file_not_exist);
	}
	else
	{		
    		file.seekg (0, file.end);
    		int length = file.tellg();
    		file.seekg (0, file.beg);
			std::stringstream ss;
			ss << length;
			message="UPLOAD "+args.file_bez +" "+ ss.str()+" END OF CONNECTION";
  		if(send(socket, message.data(), message.length(),0) < 0 )
 		{
        	error_status(error_message_sent);
    	} 
		
		do{
    		if(recv(socket,&buffer,1,0)>0){
    			data=data+buffer;
    		}
    		if(data.find("END OF CONNECTION") != std::string::npos){
    			found_end = true;
    		}

    	}while(!found_end);
    	income_message=data.substr(0,data.find("END OF CONNECTION")-1);
    	if(income_message=="OK")
    	{
    			char * buffer = new char [length];
	    		file.read (buffer,length);
	    		file.close();
	    		if(send(socket, buffer, length,0) < 0 )
			 	{
			       	error_status(error_message_sent);
			    }
    	}
    	else
    		print_error(income_message);
	}

 }
// hlavna main funkcia
 int main (int argc, char** argv)
 {
 	// definicia premennych
 	argument args;
 	int socket_use;
 	struct sockaddr_in socket_addres;
 	// kontrola parametrov
 	if(argc != 7)
 		error_status(error_argv);
 	parse_arg (argc-1,argv,&args);

	socket_use=socket_new();
	set_network(socket_use,&socket_addres,&args);
	if (args.read)
	{
		download(socket_use,args);
	}
	else if (args.write)
	{
		upload(socket_use,args);
	}

	if(close(socket_use)<0) // ukonci spojenie
        error_status(error_close_socket);
	error_status(error_ok);
 }