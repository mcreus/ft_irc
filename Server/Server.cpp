/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcreus <mcreus@student.42perpignan.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/28 11:55:42 by mcreus            #+#    #+#             */
/*   Updated: 2023/12/12 12:43:34 by mcreus           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

struct sockaddr_in  address;

Server::Server(char **av)
{
    master_socket = socket(AF_INET, SOCK_STREAM, 0);
    max_clients = 20;
    opt = 1;
    port = atoi(av[1]);
    //client_socket.insert(std::pair<int, User*>(master_socket, new User));
}

Server::~Server()
{
    std::map<int, User*>::iterator    it = client_socket.begin();
    std::map<int, User*>::iterator    ite = client_socket.end();
    while (it != ite)
    {
        delete it->second;
    }
}

void    Server::initServer()
{
    if (master_socket == 0)
    {
        std::cerr << "Socket failed" << std::endl;
        exit(EXIT_FAILURE);
    }
    if (setsockopt(master_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt)) < 0)
    {
        std::cerr << "Setsockopt" << std::endl;
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( port );
    if (bind(master_socket, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        std::cerr << "Bind failed" << std::endl;
        exit(EXIT_FAILURE);
    }
    std::cout << "Listener on port " << port << std::endl;
	//try to specify maximum of 3 pending connections for the master socket  
	if (listen(master_socket, 3) < 0)   
	{   
		perror("listen");   
		exit(EXIT_FAILURE);   
	}
    addrlen = sizeof(address);
    std::cout << "Waiting for connections ..." << std::endl;
}

void    Server::initArgs()
{
    std::map<int, User*>::iterator    it = client_socket.begin();
    FD_ZERO(&readfds);   
		//add master socket to set  
	FD_SET(master_socket, &readfds);
		//add child sockets to set  
    //int  client_fd;
    max_fd = master_socket;
	for (; it != client_socket.end(); it++)   
	{   
		FD_SET(it->first ,&readfds);   
			//highest file descriptor number, need it for the select function  
		if(it->first > max_fd)
			max_fd = it->first;   
	}
		//wait for an activity on one of the sockets , timeout is NULL ,  
		//so wait indefinitely  
    //std::cout << addrlen << std::endl;
	activity = select(max_fd + 1 , &readfds , NULL , NULL , NULL);
	if (activity < 0)   
	{   
		std::cout << "select error" << std::endl;   
	}
}

void    Server::newConnection()
{
    std::string message = "Bienvenue sur Corddis";
    new_socket = 0;
    if (FD_ISSET(master_socket, &readfds))   
	{   
        std::cout << master_socket << std::endl;
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
           if (client_socket.find(new_socket) == client_socket.end())  
		    client_socket.insert(std::pair<int, User*>(new_socket, new User()));
        else
            std::cout << "Client is already connected !!!" << std::endl;
	}
}


void    Server::listenSocket()
{
    std::map<int, User*>::iterator    it = client_socket.begin();
    for (; it != client_socket.end(); it++)   
	{
		if (FD_ISSET( it->first , &readfds))
		{
			//Check if it was for closing , and also read the  
				//incoming message  
			if ((valread = read(it->first, buffer, 1024)) == 0)
			{   
				//Somebody disconnected , get his details and print 
				getpeername(it->first , (struct sockaddr*)&address , (socklen_t*)&addrlen);   
				std::cout << "Host disconnected , ip " << inet_ntoa(address.sin_addr) << " , port " << ntohs(address.sin_port) << std::endl;    
				close(it->first);
                //delete it->second;
				//client_socket.erase(it->first);
                if (client_socket.empty())
                    max_fd = master_socket;
                else
                {
                    std::map<int, User*>::iterator    it_new = client_socket.end();
                    it_new--;
                    max_fd = it_new->first;
                }
			}
			else 
			{   
				buffer[valread] = '\0';
                std::map<int, User*>::iterator    it_new = client_socket.begin();
                std::cout << buffer << "UUUUUUUUUUUUUU" << std::endl;
                if (buffer == ("#JOIN /" + channelName))
                {
                    
                }
                else
                    
				for (; it_new != client_socket.end(); it_new++)
				{
					if (it->first != it_new->first)
						send(it_new->first , buffer , strlen(buffer) , 0 );
				}
			}   
		}   
	}
    
}
/*int const   &getMasterSocket() const;
int const   &getaddrlen() const;
int const   &getNewSocket() const;
int const   &getMaxClients() const;
int const   &getClientSocket() const;
int const   &getActivity() const;
int const   &getValread() const;
int const   &getFd() const;
int const   &getMaxFd() const;*/

int const   &Server::getPort() const
{
    return (port);
}