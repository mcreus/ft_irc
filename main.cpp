#include <iostream>
#include <iomanip>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>

//socket
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

//custom lib
#include "Server/Server.hpp"

int main(int argc , char **argv)   
{
	if (argc != 3)
		return (0);
	int	max_clients = 30;
	Server	server(max_clients);
	int	opt = 1;
	int	master_socket;
	int	addrlen;
	int	new_socket;
	int	client_socket[max_clients];
	int	activity;
	int	i;
	int	valread;
	int	sd;
	int	max_sd;
	int	port = atoi(argv[1]);
	int	buf = 1024;

	struct sockaddr_in address; 

	std::string message = "ECHO Daemon v1.0 \r\n";	
	char buffer[buf];
	fd_set readfds;

	//(void*)server;
	for (i = 0; i < max_clients; i++)   
	{   
		client_socket[i] = 0;   
	}   
	if((master_socket = socket(AF_INET, SOCK_STREAM, 0)) == 0)   
	{   
		perror("socket failed");   
		exit(EXIT_FAILURE);   
	}    
	if(setsockopt(master_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt)) < 0)
	{
		perror("setsockopt");   
		exit(EXIT_FAILURE);   
	}
	address.sin_family = AF_INET;   
	address.sin_addr.s_addr = INADDR_ANY;   
	address.sin_port = htons( port );
	if (bind(master_socket, (struct sockaddr *)&address, sizeof(address))<0)   
	{   
		perror("bind failed");   
		exit(EXIT_FAILURE);   
	}   
	std::cout << "Listener on port " << port << std::endl;
	if (listen(master_socket, 3) < 0)   
	{   
		perror("listen");   
		exit(EXIT_FAILURE);   
	}
	addrlen = sizeof(address);
	std::cout << "Waiting for connections ...\n";   
	while(1)   
	{   
		FD_ZERO(&readfds);
		FD_SET(master_socket, &readfds);
		max_sd = master_socket; 
		for ( i = 0 ; i < max_clients ; i++)
		{
			sd = client_socket[i];
			if(sd > 0)
				FD_SET(sd ,&readfds);
			if(sd > max_sd)
				max_sd = sd;   
		}
		activity = select(max_sd + 1 , &readfds , NULL , NULL , NULL);
		if ((activity < 0))   
		{   
			std::cout << "select error" << std::endl;   
		}   
		if (FD_ISSET(master_socket, &readfds))   
		{
			if ((new_socket = accept(master_socket,(struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0)   
			{
				perror("accept");   
				exit(EXIT_FAILURE);   
			}
			std::cout << "New connection , socket fd is " << new_socket << " , ip is : " <<  inet_ntoa(address.sin_addr) << ", port : " << ntohs(address.sin_port) << std::endl;
			for (i = 0; i < max_clients; i++)
			{ 
				if(client_socket[i] == 0)
				{   
					client_socket[i] = new_socket;
					fcntl(client_socket[i], F_SETFL, O_NONBLOCK);
					std::cout << "Adding to list of sockets as " << i << std::endl;
					break;
				}
			}
		}
		for (i = 0; i < max_clients; i++)   
		{
			sd = client_socket[i];
			if (FD_ISSET(sd, &readfds))   
			{
				valread = read(sd, buffer, buf);
				if (valread == 0)
				{
					getpeername(sd , (struct sockaddr*)&address , (socklen_t*)&addrlen);   
					std::cout << "Host disconnected , ip " << inet_ntoa(address.sin_addr) << " , port " << ntohs(address.sin_port) << std::endl;    
					shutdown(sd, SHUT_RDWR);
					client_socket[i] = 0;
					max_sd--;
				}
				else
				{
					buffer[valread] = '\0';
					message = "name ";
					message.append(buffer);
					for (i = 0; i < max_sd; i++)
					{
						if (sd != client_socket[i] && client_socket[i] != 0)
							send(client_socket[i], message.c_str(), message.length(), 0);
					}
					buffer[valread] = '\0';
					message = "";
				}   
			}   
		}   
	}   
		 
	return 0;   
}
