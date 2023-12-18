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
    {
        std::cerr << "Error you program must be launch with <name of program> <port> <password>";
		return (0);
    }
	
	Server  server(argv);
	server.initServer();
	server.initMapCommand();
	server.initArgs();
		 
	return 0;   
}
