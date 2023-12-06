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
	 
int main(int argc , char **argv)   
{
	if (argc != 3)
		return (0);
	int	opt = 1;   
	int	master_socket;
	int	addrlen;
	int	new_socket;
	int	max_clients = 30;
	int	client_socket[max_clients];
	int	activity;
	int	i;
	int	valread;
	int	sd;
	int	max_sd;
	int	port = atoi(argv[1]);

	struct sockaddr_in address; 

	std::string message = "ECHO Daemon v1.0 \r\n";	
	char buffer[1024];
	fd_set readfds;

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
	if (bind(master_socket, (struct sockaddr *)&address, sizeof(address)) < 0)   
	{   
		perror("bind failed");   
		exit(EXIT_FAILURE);   
	}   
	std::cout << "Listener on port " << port << std::endl;
	//try to specify maximum of 3 pending connections for the master socket  
	if (listen(master_socket, 3) < 0)   
	{   
		perror("listen");   
		exit(EXIT_FAILURE);   
	}
	//accept the incoming connection  
	addrlen = sizeof(address);   
	std::cout << "Waiting for connections ...\n";   
	while(1)   
	{   
		//clear the socket set  
		FD_ZERO(&readfds);   
		//add master socket to set  
		FD_SET(master_socket, &readfds);   
		max_sd = master_socket;   
		//add child sockets to set  
		for ( i = 0 ; i < max_clients ; i++)   
		{   
			//socket descriptor  
			sd = client_socket[i];
			//if valid socket descriptor then add to read list  
			if(sd > 0)   
				FD_SET(sd ,&readfds);   
			//highest file descriptor number, need it for the select function  
			if(sd > max_sd)
				max_sd = sd;   
		}
		//wait for an activity on one of the sockets , timeout is NULL ,  
		//so wait indefinitely  
		activity = select(max_sd + 1 , &readfds , NULL , NULL , NULL);
		if ((activity < 0))   
		{   
			std::cout << "select error" << std::endl;   
		}   
		//If something happened on the master socket ,  
		//then its an incoming connection  
		if (FD_ISSET(master_socket, &readfds))   
		{   
			if ((new_socket = accept(master_socket,(struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0)   
			{
				perror("accept");   
				exit(EXIT_FAILURE);   
			}   
			//inform user of socket number - used in send and receive commands  
			std::cout << "New connection , socket fd is " << new_socket << " , ip is : " <<  inet_ntoa(address.sin_addr) << ", port : " << ntohs(address.sin_port) << std::endl;   
			//send new connection greeting message
			send(new_socket, message.c_str(), message.length(), 0);
			/*if(send(new_socket, message.c_str(), message.length(), 0) != (long)message.length())
				perror("send");*/
			std::cout << "Welcome message sent successfully\n";   
			//add new socket to array of sockets  
			for (i = 0; i < max_clients; i++)   
			{ 
				if(client_socket[i] == 0)   
				{   
					client_socket[i] = new_socket;
					std::cout << "Adding to list of sockets as " << i << std::endl;
					break;
				}
			}
		}
		//else its some IO operation on some other socket 
		for (i = 0; i < max_clients; i++)   
		{
			sd = client_socket[i];   
			if (FD_ISSET( sd , &readfds))   
			{
				//Check if it was for closing , and also read the  
				//incoming message  
				if ((valread = read(sd, buffer, 1024)) == 0)
				{   
					//Somebody disconnected , get his details and print  
					getpeername(sd , (struct sockaddr*)&address , (socklen_t*)&addrlen);   
					std::cout << "Host disconnected , ip " << inet_ntoa(address.sin_addr) << " , port " << ntohs(address.sin_port) << std::endl;    
					close(sd);   
					client_socket[i] = 0;
					if (max_sd == sd)
                    {
                        int j = (sd - 1);
                        while (client_socket[j] == 0 && j > 0)
                            j--;
                        if (j != 0)
                            max_sd = client_socket[j];
                        else
                            max_sd = master_socket;
                    }
				}
				else 
				{   
					//set the string terminating NULL byte on the end  
					//of the data read
					buffer[valread] = '\0';
					for (i = 0; i < max_clients; i++)
					{
						if (sd != client_socket[i] && client_socket[i] != 0)
							send(client_socket[i] , buffer , strlen(buffer) , 0 );
					}
				}   
			}   
		}   
	}   
		 
	return 0;   
}
