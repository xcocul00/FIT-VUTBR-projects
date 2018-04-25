/*
 * author: Maros Cocula
 * subject:  IPK
 * project:  proj2 - Prenos suborov
 * file: server.cpp
 */

  #include "server.h"

 // funkcia vypise chybovy kod a spravu, spravy su v subore server.h
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
 	int sock=socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
 	if(sock<0)
 		error_status(error_socket);
	return sock;
 }
 // nastavi pociatocne nastavenia siete
void set_sin(sockaddr_in *socket_name,int port)
{
	socket_name->sin_family = AF_INET;
    socket_name->sin_port = htons(port);
    socket_name->sin_addr.s_addr = INADDR_ANY;
}
// nastavi pociatocne nastavenia siete
void set_network(int socket_use, sockaddr_in socket_name)
 {

    if (bind(socket_use, (sockaddr *)&socket_name, sizeof(socket_name)) < 0)
		error_status(error_bind);	
	if (listen(socket_use, 100) < 0)
		error_status(error_listen);
 }

 // zachyti stlacenie ctrl+c , server sa ukonci
void func_ctrl_c_came(int signal_came)
{
	ctrl_c_came = true;
	signal_came++;
}

// namiesto kodovych znakov vrati medzery a ~
std::string give_whitespace_back(std::string string)
{
    while(string.find("%20")!=std::string::npos){
        string.replace(string.find("%20"), 3, " ");
    }
    while(string.find("%7E")!=std::string::npos){
        string.replace(string.find("%7E"), 3, "~");
    }
    return string;
}
// parsuje obdrzanu hlavicku od klienta
struct_IPK_PROTOKOL parse_response (std::string message)
{
	struct_IPK_PROTOKOL data;
	if (message.find("DOWNLOAD") != std::string::npos)
		data.download=true;
	if (message.find("UPLOAD") != std::string::npos)
		data.upload=true;
	data.file = message.substr(message.find(" ")+1,message.size());
	data.file = data.file.substr(0,data.file.find(" "));
	data.length=message.substr(0, message.find(" END OF CONNECTION"));
	data.length = data.length.substr(data.length.find(" ")+1,data.length.size());
	data.length = data.length.substr(data.length.find(" ")+1,data.length.size());
	data.file=give_whitespace_back(data.file);
	return data;
}

// funkcia ktora obsluhuje download zo servra
void download (struct_IPK_PROTOKOL response, int socket)
{
	char buffer;
	std::string data;
	std::string income_message="";
	std::string message="";
	std::ifstream file;
	bool found_end = false;
	
	file.open(response.file, std::ios::binary);
	if(!file.is_open())
	{
		message="ERR1 END OF CONNECTION";
		if(send(socket, message.data(), message.size(),0) < 0 )
 		{
        	error_status(error_message_sent);
    	}
	}
	else
	{		

    		file.seekg (0, file.end);
    		int length = file.tellg();
    		file.seekg (0, file.beg);
    		std::stringstream ss;
			ss << length;
    		message="OK "+ss.str()+" END OF CONNECTION";
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
			income_message.append(data);
			data = income_message.substr(0,income_message.find(" "));
			if(data=="OK")
			{
				char * buffer = new char [length];
	    		file.read (buffer,length);
	    		file.close();
	    		if(send(socket, buffer, length,0) < 0 )
			 	{
			       	error_status(error_message_sent);
			    }
			}

	}


} 
// funkcia ktora obsluhuje upload zo servra
void upload (struct_IPK_PROTOKOL response, int socket)
{
	std::string message="";
	std::string data;
	char buffer;
	bool replace = false;
    int length = atoi(response.length.data());
    if(response.file=="server"||response.file=="server.cpp"||response.file=="server.h"
    	||response.file=="client"||response.file=="client.cpp"||response.file=="client.h")
    {
    	message="ERR2 END OF CONNECTION";
    	replace=true;
    	if(send(socket, message.data(), message.size(),0) < 0 )
 		{
        	error_status(error_message_sent);
    	}
    }
    if(!replace){
    	message="OK END OF CONNECTION";
    	if(send(socket, message.data(), message.size(),0) < 0 )
 		{
        	error_status(error_message_sent);
    	}
    	do{
    	
    	if(recv(socket,&buffer,1,0)>0){
    		data=data+buffer;
    		std::ofstream out(response.file.data(),std::ios::binary);
        	out << data ;
    	}
    	length--;
    	}while(length!=0);

		}

}

// funkcia ktora vytvara nove vlakna
void *connect (void* threadid) 
{
	char buffer;
	std::string data;
	struct_THREAD *thread = (struct_THREAD *)threadid;
	std::string income_message="";
	bool found_end = false;
	struct_IPK_PROTOKOL response; 

    do{
    	if(recv(thread->new_socket,&buffer,1,0)>0){
    		data=data+buffer;
    	}
    	if(data.find("END OF CONNECTION") != std::string::npos){
    		found_end = true;
    	}

    }while(!found_end);
	
	income_message.append(data);
 	response = parse_response(income_message);

 	if (response.download)
 	{
 		download(response,thread->new_socket);
 	}
 	if (response.upload)
 	{
 		upload(response,thread->new_socket);
 	}

	close(thread->new_socket);
   	pthread_exit(NULL);
}

// hlavna main funkcia 
 int main (int argc, char** argv)
 {
// definica premennych
 	int port;
 	int socket_use;
 	sockaddr_in socket_name;
// signaly pre ctrl+c
 	struct sigaction action_ctrl_c;
	action_ctrl_c.sa_handler = func_ctrl_c_came;
	sigemptyset (&action_ctrl_c.sa_mask);
	action_ctrl_c.sa_flags = 0;
	sigaction (SIGINT, &action_ctrl_c, NULL);			
	sigaction (SIGTERM, &action_ctrl_c, NULL);
// kontrola parametrov
 	if(argc!=3) 
 		error_status(error_argv);
 	if(strcmp(argv[1],"-p"))
 		error_status(error_argv_p);
 	if(atoi(argv[2])!=0)
 		port = atoi(argv[2]);
 	else
 		error_status(error_argv_p);

 	socket_use=socket_new();
 	set_sin(&socket_name,port);
 	set_network(socket_use,socket_name);

 	do{
 			struct_THREAD *connection = new struct_THREAD;
 			socklen_t length = sizeof(connection->client);
 			int new_connection = accept(socket_use, (sockaddr *)&(connection->client), &length);
 			connection->new_socket = new_connection;
 			if (ctrl_c_came)
 				break;
 			if(new_connection<0)
 				error_status(error_accept); 
 			
 			pthread_t thread;
			if (pthread_create(&thread, NULL, connect, (void *)connection) != 0) 
				error_status(error_thread); 		
			delete [] connection;

 	}while(ctrl_c_came==false);
 	
 	if(close(socket_use)<0) // ukonci spojenie
        error_status(error_close_socket);
    pthread_exit(NULL);
 
 }