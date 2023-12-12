#include <iostream>
#include <iomanip>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <stdio.h>

//socket
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "./Server/Server.hpp"
	 
int main(int argc , char **argv)   
{
	if (argc != 3)
		return (0);
	Server  server(argv);
	server.initServer();
	while(1)   
	{
		server.initArgs();
		server.newConnection();
		server.listenSocket();        
	}   
		 
	return 0;   
}
