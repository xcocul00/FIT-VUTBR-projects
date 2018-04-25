/*
 * Author : Maros Cocula (xcocul00)
 * Subject : ISA 
 * Project : Programovani sitove sluzby -> FTP client
 * Year : 2016
 * file : fclient.cpp
 */
#include "fclient.h"
#define DEBUG 1
/*
* hlavna funkcia, vola jednotlive funkcie podla parametrov
*/
int main (int argc, char** argv){

  argument args;
  login logs;
  struct sockaddr_in socket_address;
  int socket,socket_other;
  int port,port_pasive,port_active;
  /* CTRL+C*/
  struct sigaction action_ctrl_c;
  action_ctrl_c.sa_handler = func_ctrl_c_came;
  sigemptyset (&action_ctrl_c.sa_mask);
  action_ctrl_c.sa_flags = 0;
  sigaction (SIGINT, &action_ctrl_c, NULL);     
  sigaction (SIGTERM, &action_ctrl_c, NULL);

  parse_argv(argc-1,argv,&args);
  parse_login(&logs,args.login);
  port=parse_port(args.port);
  socket=create_socket();
  set_network(socket,port,&socket_address,args);
  autentificate(socket,logs);

  if(args.p){      // argument -p
    ftp_passive(socket,&args);
    socket_other=create_socket();
    port_pasive=parse_port(args.pasive_port);
    set_network(socket_other,port_pasive,&socket_address,args);
  }
  if(!args.u && !args.d && !args.r){ // ak nie je zadany argument -u | -d | -r
    print_structure(socket,socket_other,args);
  }
  else if (args.a){ // argument -a port
    ftp_active(socket,&args);
    socket_other=create_socket();
    port_active=parse_port(args.active_port);
    set_network(socket_other,port_active,&socket_address,args);

  }
  if(args.P){   // argument -P, zmena cesty
    change_path(socket,args);
  }
  if(args.u){   // argument -u, upload
    upload(socket,socket_other,args);
  }
  if (args.d){  // argument -d, download
    download(socket,socket_other,args);
  }
  if(args.r){   // argument -r, delete
    remove(socket,args);
  }

  if (DEBUG){   // spustenei debugu
    debug_param(args,logs);
  }

  close(socket_other);    // zatvorenie soketu
  if(close(socket)<0){
    error_status(err_socket_close);
  }
  error_status(err_ok);
}
/*
* funkcia ktora zachyti stlacenie ctrl+c
*/
void func_ctrl_c_came(int signal_came){
  if(signal_came!=0)
  error_status(err_sigint);
}
/*
 *  vypis chybovej hlasky a navratovej hodnoty
 */
void error_status(int err){
  if (err == 0){
    exit(EXIT_SUCCESS);
  }
  else
    std::cerr << "Error : " << err_message[err] << std::endl;
  exit(EXIT_FAILURE);
}
/*
 *  vypis pomocnika
 */
void help(){
  std::cout<< "./fclient -s 192.168.1.102 -c credentials.txt [-p]|[-a port_number] [-d|-u|-r filename] [-P path] \n" << 
    "-s server  IP adresa nebo doménové jméno serveru\n" <<
    "-c filename  soubor s uživatelským jménem a heslem\n" <<
    "-a port_number  aktivní mód, datové spojení bude inicializováno z daného portu\n" <<
    "-p pasivní mód\n" <<
    "-u filename soubor, který se uloží na server (pokud je pogram spuštěn s parametrem -P, soubor na server nahrajte do této složky)\n" <<
    "-d filename soubor, který se stáhne ze serveru (pokud je program spuštěn s paramterem -P, soubor na počítači uložte do této složky)\n" <<
    "-r filename soubor, který se má smazat ze serveru (pokud se soubor bude odstraňovat z jiné než z kořenové složky, bude cesta uvedena v parametru -P)\n" <<
    "-P path  cesta k souboru, volitelný přepínač k -u, -d, -r\n";
    error_status(err_ok);

}
/*
 *  roztriedenie parametrov a ulozenie ich do struktury
 */
void parse_argv(int argc, char** arguments, argument *args){
  int c;
  if(argc==1 && !strcmp(arguments[1],"-h")){
    help();

  }
  if(argc >= 12 || argc < 4){
    error_status(err_parser);
  }

    while ((c = getopt(argc+1, arguments, "+s:c:a:pu:d:r:P:")) != -1) {
      switch (c) {
        case 's':
          args->s = true;
          args->server=optarg;
          break;
        case 'c':
          args->c = true;
          args->login=optarg;
          break;
        case 'a':
          args->a = true;
          args->active_port=optarg;
          break;        
        case 'p':
          args->p = true;
          break; 
        case 'u':
          if (args->d || args->r)
            error_status(err_parser);
          args->u = true;
          args->sent=optarg;
          break; 
        case 'd':
          if (args->u || args->r)
            error_status(err_parser);       
          args->d = true;
          args->get=optarg;
          break; 
        case 'r':
          if (args->d || args->u)
            error_status(err_parser);
          args->r = true;
          args->remove=optarg;
          break;
        case 'P':
          args->P = true;
          args->path=optarg;
          if(!(args->d || args->u || args->r)){
            error_status(err_parser);
          }
          break;
        case '?':
            error_status(err_parser);
    }
    opterr=0;
   }
   if(!args->c || !args->s){
    error_status(err_parser);
   }
   if(args->a && args->p){
    error_status(err_parser);
   }
   if(!args->a && !args->p){
    args->p=true;
   }

   args->port="21";
}
/*
 *  roztriedenie suboru s loginom a heslom
 */
void parse_login(login *logs, std::string args){
  
  std::ifstream file;
  std::string data="";
  std::stringstream string;
  std::string temporary;

  file.open(args,std::ios::binary);
  if(!file.is_open()){
    error_status(err_login);
  }
  else{
    string << file.rdbuf();
    data=string.str();
    temporary=data.substr(data.find(" ")+1,data.size());
    temporary=temporary.substr(0,temporary.find("login")-1);
    logs->username=temporary.data();
    temporary=data.substr(data.find("login: ")+7,data.size());
    temporary=temporary.substr(0,temporary.size()-1);
    logs->password=temporary.data();

  }
}
/*
 *  konverzia portu zo stringu na int
 */
int parse_port(std::string port){
  int value;
  if(atoi(port.data())== 0){  
    error_status(err_port);
  }
  else
    value=atoi(port.data());
  return value; 
}
/*
 *  vytvorenie soketu
 */
int create_socket(){
  int sct=socket(AF_INET,SOCK_STREAM,0);
  if(sct<0){
    error_status(err_socket);
  }
  return sct;
}
/*
 *  nastavenie parametrov spojenia a pokus o nadviazanie spojenia
 */
void set_network(int socket,int port,struct sockaddr_in *socket_address,argument args){
  struct hostent *remoteHost;
  const char* host_name=args.server.data();
  socket_address->sin_family=AF_INET;
  socket_address->sin_port=htons(port);
  remoteHost=gethostbyname(host_name);
  if(remoteHost==NULL){
    error_status(err_ip_address);
  }
  memcpy(&socket_address->sin_addr,remoteHost->h_addr,remoteHost->h_length);
  if(connect(socket,(struct sockaddr *)&(*socket_address),sizeof(*socket_address))<0){
    error_status(err_connect);
  }

}
/*
 *  prihlasenie sa  k ftp serveru
 */
void autentificate(int socket, login logs){
  
  std::string message;
  int status;
  
  status=receive(socket);
  if(status != 220){
    error_status(err_server_not_ready);
  }  
  message="USER "+logs.username+"\r\n";
  if(send(socket,message.data(),message.size(),0)<0){
    error_status(err_read);
  }
  status=receive(socket);
  if(status != 331){
    error_status(err_server_not_ready);
  }
  
  message="PASS "+logs.password+"\r\n";
  if(send(socket,message.data(),message.size(),0)<0){
    error_status(err_read);
  }
  status=receive(socket);
  if (status == 530){
    error_status(err_user);
  }
  else if(status != 230){
    error_status(err_server_not_ready);
  } 
}
/*
 *  ukoncenie spojenie so serverom
 */
void quit(int socket){
  std::string message="QUIT\r\n";
  if(send(socket,message.data(),message.size(),0)<0){
    error_status(err_read);
  }
  int status=receive(socket);
  if(status!=221){
    error_status(err_quit);
  }
}
/*
 *  zaslanie spravy PASV a ziskanie IP adresy a portu zo spravy
 */
void ftp_passive(int socket,argument *args){
  std::string message="PASV\r\n";
  std::string data="";
  std::string addres="",port="";
  char buffer,tmp;
  int rcv,status,int_port;
  if(send(socket,message.data(),message.size(),0)<0){
    error_status(err_read);
  }
  do{
    tmp=buffer;
    rcv=recv(socket,&buffer,1,0);
    if(rcv>0){
      data+=buffer;
    }
    else if(rcv<0){
      error_status(err_read);
    }
  }while(buffer!='\n' && tmp!='\r');
  
  if(DEBUG){
  std::cout<<data;
  }

  status=atoi(data.substr(0,data.find(" ")).data());
  if(status!=227){
    error_status(err_quit);
  }

  data=data.substr(data.find("(")+1,data.size());
  data=data.substr(0,data.find(")"));
  for(int i=0; i<6;i++){
    if(i<4){
      addres+=data.substr(0,data.find(","));
      data=data.substr(data.find(",")+1,data.size());
      if(i!=3){
        addres+=".";
      }
    }
    else if (i==4){
      port=data.substr(0,data.find(","))+";";
      data=data.substr(data.find(",")+1,data.size());
    }
    else if (i==5){
      port+=data;
    }
  }
  int_port=atoi(port.substr(0,port.find(";")).data())*256+atoi(port.substr(port.find(";")+1,port.size()).data());
  args->server=addres;
  std::stringstream ss;
  ss << int_port;
  args->pasive_port=ss.str();

}
/*
 *  zaslanie spravy PORT a ziskanie IP adresy a portu zo spravy
 */
void ftp_active(int socket,argument *args){
    struct ifaddrs *ifap, *ifa;
    struct sockaddr_in *sa;
    char *addr;
    std::string ifconfig="";
    std::string message;
    std::string a1,a2,a3,a4;
    std::stringstream ss,ss2;
    int int_port=atoi(args->active_port.data());
    int p1=int_port/256;
    int p2=int_port%256;
    getifaddrs (&ifap);
    for (ifa = ifap; ifa; ifa = ifa->ifa_next) {
        if (ifa->ifa_addr->sa_family==AF_INET) {
            sa = (struct sockaddr_in *) ifa->ifa_addr;
            addr = inet_ntoa(sa->sin_addr);
            ifconfig=ifconfig+ ifa->ifa_name + " addr:" + addr +"\n";
        }
    }
    if(ifconfig.substr(0,ifconfig.find(" "))=="lo") {
      ifconfig=ifconfig.substr(ifconfig.find("\n")+1,ifconfig.size());
    }
    if(ifconfig.size()==0){
      a1="127";
      a2="0";
      a3="0";
      a4="1";
    }
    else{
      ifconfig=ifconfig.substr(ifconfig.find(":")+1,ifconfig.size());
      a1=ifconfig.substr(0,ifconfig.find("."));
      ifconfig=ifconfig.substr(ifconfig.find(".")+1,ifconfig.size());
      a2=ifconfig.substr(0,ifconfig.find("."));
      ifconfig=ifconfig.substr(ifconfig.find(".")+1,ifconfig.size());
      a3=ifconfig.substr(0,ifconfig.find("."));
      ifconfig=ifconfig.substr(ifconfig.find(".")+1,ifconfig.size());
      a4=ifconfig.substr(0,ifconfig.find("\n"));
    }
    ss<<p1;
    ss2<<p2;
    message="PORT "+a1+","+a2+","+a3+","+a4+","+ss.str()+","+ss2.str()+"\r\n";
    if(send(socket,message.data(),message.size(),0)<0){
      error_status(err_read);
      }
    int status=receive(socket);
    if(status!=200){
      error_status(err_port_active);
    }
}
/*
 *  zaslanie spravy CWD, zmena cesty 
 */
void change_path(int socket, argument args){
  std::string message="CWD "+args.path+"\r\n";
  if(send(socket,message.data(),message.size(),0)<0){
    error_status(err_read);
  }
  int status=receive(socket);
  if(status!=250){
    error_status(err_direct);
  }
}
/*
 *  zaslanie spravy STOR, zaslanie subora na FTP server
 */
void upload (int socket,int socket_pasive, argument args){
  std::string message="STOR "+args.sent+"\r\n";
  std::ifstream file;
  file.open(args.sent, std::ios::binary);
  if(!file.is_open())
      {
        error_status(err_file_not_exist);
      }

  if(send(socket,message.data(),message.size(),0)<0){
    error_status(err_read);
  }
  int status=receive(socket);

  if(status==150){  
        file.seekg (0, file.end);
        int length = file.tellg();
        file.seekg (0, file.beg);
        char * buffer = new char [length];
        file.read (buffer,length);
        file.close();
        if(send(socket_pasive, buffer, length,0) < 0 ){
            error_status(err_read);
         }
  }
  else
    error_status(err_server_not_ready);
}
/*
 *  zaslanie spravy RETR, stiahnutie subora z FTP servera
 */
void download(int socket,int socket_pasive, argument args){
    std::string message="RETR "+args.get+"\r\n";
    std::string data = "";
    char buffer,tmp;
    int rcv;
    bool empty=false;
    std::ifstream file;

    if(send(socket,message.data(),message.size(),0)<0){
    error_status(err_read);
    }
    do{
        tmp=buffer;
        rcv=recv(socket,&buffer,1,0);
        if(rcv>0){
          data+=buffer;
        }
        else if(rcv<0){
          error_status(err_read);
        }
    }while(buffer!='\n' && tmp != '\r');
    if(DEBUG){
      std::cout<<data;
    }
    int status= atoi(data.substr(0,data.find(" ")).data());
    data=data.substr(data.find("(")+1,data.size());
    data=data.substr(0,data.find(" "));
    int length=atoi(data.data());
    if(length == 0){

      std::ofstream out(args.get.data(),std::ios::binary);
      empty=true;
    }
    
    data.clear();

    if(status==150 && !empty){
    
      do{

          rcv=recv(socket_pasive,&buffer,1,0);
            if(rcv>0){
            data=data+buffer;
            std::ofstream out(args.get.data(),std::ios::binary);
            out << data ;
            }
      length--;
      }while(length!=0);

    }
    else if(!empty){
      error_status(err_file_not_exist);
    }
    
}
/*
 *  zaslanie spravy DELE, vymazanie subora z FTP servera
 */
void remove(int socket, argument args){
  std::string message="DELE "+args.remove+"\r\n";
  if(send(socket,message.data(),message.size(),0)<0){
    error_status(err_read);
  }
  int status=receive(socket);
  if(status!=250){
    error_status(err_file_not_exist);
  }
}
/*
 *  ziskanie spravy zo servera a spracovanie jej
 */
int receive (int socket){
  std::string data="";
  char buffer,tmp;
  int rcv;
  do{
    tmp=buffer;
    rcv=recv(socket,&buffer,1,0);
    if(rcv>0){
      data+=buffer;
    }
    else if(rcv<0){
      error_status(err_read);
    }
  }while(buffer!='\n' && tmp!='\r');
  if(DEBUG){
  std::cout<<data;
  }
  data=data.substr(0,data.find(" "));
  return atoi(data.data()); 
}
/*
 *  ziskanie poctu riadkov z vysledku spravy LIST
 */
int count_row (std::string data){
  int c=0;
  for(int i=0; i<(int) data.size();i++){
    if(data[i]=='\n')
      c++;
  }
  return c;
}
/*
 *  ziskanie potrebnych informacii zo spravy LIST (file|directory a meno)
 */
std::string cut_table(int row_cnt, std::string data){
  std::string tmp = "";
  std::string table = "";
  for(int i=0; i<row_cnt;i++){
      tmp =data.substr(0,data.find("\r\n"));
      table= table + tmp.substr(tmp.find_last_of(" ")+1,tmp.size()) + " " +tmp.substr(tmp.find("    ")+4,1)+ "\n";
      data=data.substr(data.find("\r\n")+1,data.size());
    }
  return table;
}
/*
 *  ziskanie dat spravou LIST zo servera
 */
std::string recv_table(std::string name,int socket, int socket_pasive){
  char buffer;
  int rcv;
  std::string data = "";
  std::string message="LIST "+name+"\r\n";
  if(send(socket,message.data(),message.size(),0)<0){
    error_status(err_read);
  }
  int status=receive(socket);
  if(status!=150){
    error_status(err_get_struct);
  }
  do{
        rcv=recv(socket_pasive,&buffer,1,0);
        if(rcv>0){
          data+=buffer;
        }
        else if(rcv<0){
          error_status(err_read);
        }
    }while(rcv!=0);
  status=receive(socket);
  if(status!=226){
    error_status(err_get_struct);
  }
  return data;

}
/*
 *  rekurzivne volanie pre ziskanie dat zo spravy LIST pre kazdy adresar
 */
std::string recurse(std::string table, int row_cnt, int socket, std::string name, argument args){
  std::string tmp = "";
  std::string row = "";
  std::string data = "";
  std::string name2 = "";
  std::string tab = "";
  argument sec = args;
  struct sockaddr_in address;
  int socket_other;
  int cnt=0;
  for(int i=0; i<row_cnt; i++){
      row=table.substr(0,table.find("\n"));
      tmp=row.substr(row.find(" ")+1,row.find("\n"));
      if(atoi(tmp.data())==2){
        name2=row.substr(0,row.find(" "));
        tab=tab+name2+"\n";
        ftp_passive(socket,&sec);
        socket_other=create_socket();
        int port_pasive=parse_port(sec.pasive_port);
        set_network(socket_other,port_pasive,&address,sec);
        data=recv_table(name2,socket,socket_other);
        cnt=count_row(data);
        data=cut_table(cnt,data); 
        tab=tab+recurse(data,cnt,socket,name2,sec);
      }
      else{
        if(!name.compare("")){
        tab=tab+name+row.substr(0,row.find(" "))+"\n";
      }else
        tab=tab+name+"/"+row.substr(0,row.find(" "))+"\n";
      }

      table=table.substr(table.find("\n")+1,table.size());

    }
    close(socket_other);
    return tab;

}
/*
 *  vypis struktury adresarov na servery
 */
void print_structure(int socket,int socket_pasive, argument args){
  int row_cnt;
  std::string data = "";
  std::string table = "";
  std::string name = "";
  data=recv_table(name,socket,socket_pasive);

    row_cnt=count_row(data);
    table = cut_table(row_cnt,data);
    table=recurse(table, row_cnt, socket, name, args);
    std::cout<<table;
}

/*
 *  funkcia pre debug nastavi sa v define DEBUG ako 1
 */
void debug_param(argument args, login logs){
  std::cout << "********************************\n" <<"DEBUG\n" << "********************************\n";
  std::cout << "server :" << args.server << "\n";
  std::cout << "login_file :" << args.login << "\n";
  std::cout << "port :" << args.port << "\n";
  std::cout << "active_port :" << args.active_port << "\n";
  std::cout << "pasive_port :" << args.pasive_port << "\n";
  std::cout << "sent :" << args.sent << "\n";
  std::cout << "get :" << args.get << "\n";
  std::cout << "remove :" << args.remove << "\n";
  std::cout << "path :" << args.path << "\n";
  if(args.p){
    std::cout << "PASIVE"<< "\n";
  }
  std::cout << "username:"<<logs.username << "\n"<< "password:" << logs.password << "\n";
  std::cout << "********************************\n" <<"END OF DEBUG\n" << "********************************\n";
}