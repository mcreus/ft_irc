/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcreus <mcreus@student.42perpignan.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/04 09:22:29 by mcreus            #+#    #+#             */
/*   Updated: 2023/12/04 09:22:32 by mcreus           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <iomanip>
#include <cstring>
#include <cstdlib>
#include <unistd.h>//socket
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>int	main (int ac, char **av)
{
	if (ac != 3)
		std::cerr << "Error: Your program must be writed as : <program_name><port><password>" << std::endl;
	else
	{
		int client;
		int server;
		int	port = atoi(av[1]);
		bool    isExit = false;
		bool	passwordValid = false;
		int     bufSize = 1024;
		int	bytesRecv;
		char    buffer[bufSize];
		std::string	password = av[2];		struct sockaddr_in  server_addr;
		socklen_t    size;
		
		//init socket		buffer[0] = 0;
		client = socket(AF_INET, SOCK_STREAM, 0);		if (client < 0)
		{
		    std::cout << "Error establishing connection." << std::endl;
		    exit (1);
		}
		std::cout << "server socket connection created..." << std::endl;
		server_addr.sin_family = AF_INET;
		server_addr.sin_addr.s_addr = htons(static_cast<unsigned short>(INADDR_ANY));
		server_addr.sin_port = htons(port);
		
		//binding socket
		
		if (bind(client, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0)
		{
		    std::cout << "Error binding socket..." << std::endl;
		    exit (1);
		}
		size = sizeof(server_addr);
		std::cout << size << std::endl;
		std::cout << "looking for clients..." << std::endl;
		
		// listening socket		listen(client, 5);
		server = accept(client, (struct sockaddr*)&server_addr, &size);
		for (int j = 0; j < 2; j++)
		{
			recv(server, buffer, bufSize, 0);
			std::cout << "[INFO]: " << buffer << std::endl;
			std::string	str = buffer;
			std::string	pass;
			std::size_t found = str.find("PASS :");
			if (found != std::string::npos)
			{
				pass = str.substr(6, str.length() - 7);
				if (pass == password)
					passwordValid = true;
			}
			for (int i = 0; i < bufSize; i++)
				buffer[i] = 0;
		}
		if (!passwordValid)
		{
			std::cout << "Error invalid password" << std::endl;
			//send(client, "blabla", 7, 0);
			recv(client, buffer, bufSize, 0);
		   	exit (1);
		}		if (server < 0)
		{
		    std::cout << "Error accepting client..." << std::endl;
		    exit (1);
		}
		std::cout << "Connected with client..." << std::endl;
		//send(server, "blabla", 7, 0);
		std::cout << "Enter # to end the connection" << std::endl;
		while (!isExit)
		{
			bytesRecv = recv(server, buffer, bufSize, 0);
			if (bytesRecv == 0)
			{
				std::cout << "The client disconnected.\n";
				isExit = true;
			}
			if (bytesRecv > 0)
			{
				std::cout << "Received: " << buffer;
				for (int i = 0; i < bytesRecv; i++)
					buffer[i] = 0;
			}
		}
		close(client);
	}
	return (0);
}