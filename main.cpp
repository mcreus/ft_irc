/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcreus <mcreus@student.42perpignan.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/24 16:27:03 by mcreus            #+#    #+#             */
/*   Updated: 2023/11/30 14:01:18 by mcreus           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <iomanip>
#include <cstring>
#include <cstdlib>
#include <unistd.h>

//socket
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int	main (int ac, char **av)
{
	if (ac != 3)
		std::cerr << "Error: Your program must be writed as : <program_name><port><password>" << std::endl;
	else
	{
        int client;
        int server;
		int	port = atoi(av[1]);
        bool    isExit = false;
        int     bufSize = 1024;
        char    buffer[bufSize];
		//std::string	password = av[2];

        struct sockaddr_in  server_addr;
        socklen_t    size;
        
        //init socket

        client = socket(AF_INET, SOCK_STREAM, 0);

        if (client < 0)
        {
            std::cout << "Error establishing connection." << std::endl;
            exit (1);
        }
        std::cout << " server socket connection created..." << std::endl;
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
        std::cout << "looking for clients..." << std::endl;
        
        // listening socket

        listen(client, 1);
        server = accept(client, (struct sockaddr*)&server_addr, &size);

        if (server < 0)
        {
            std::cout << "Error accepting client..." << std::endl;
            exit (1);
        }
        while (server > 0)
        {
            std::strcpy(buffer, "server connected...\n");
            send(server, buffer, bufSize, 0);
            
            std::cout << "Connected with client..." << std::endl;
            std::cout << "Enter # to end the connection" << std::endl;

            /*std::cout << "client:";
            do
            {
                recv(server, buffer, bufSize, 0);
                //std::cout << "buffer" << " ";
                if (*buffer == 0)
                {
                    *buffer = '*';
                    isExit = true;
                }
            } while (*buffer != '*');*/
            do
            {
                std::cout << "\nServer: ";
                do
                {
                    std::cin >> buffer;
                    send(server, buffer, bufSize, 0);
                    if (*buffer == '#')
                    {
                        send(server, buffer, bufSize, 0);
                        *buffer = '*';
                        isExit = true;
                    }
                } while (*buffer != '*');
                std::cout << "Client: ";
                do
                {
                    recv(server, buffer, bufSize, 0);
                    std::cout << buffer << " ";
                    if (*buffer == '#')
                    {
                        *buffer = '*';
                        isExit = true;
                    }
                } while (*buffer != '*');
            } while (isExit);
            std::cout << "Connection terminated..." << std::endl;
            std::cout << "Goodbye..." << std::endl;
            isExit = false;
            for (int i = 0; i < bufSize; i++)
            {
                buffer[i] = 0;
            }
            exit (1);
        }
        close(client);
	}
    
	return (0);
}