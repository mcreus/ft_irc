#include "Server.hpp"

struct sockaddr_in address;

void	Server::initServer()
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
	if (listen(master_socket, 3) < 0)   
	{
		perror("listen");
		exit(EXIT_FAILURE);
	}
	addrlen = sizeof(address);
	poll_fds[0].fd = master_socket;
    poll_fds[0].events = POLLIN;
	poll_fds[0].revents = 0;
    poll_count = 1;
}

void	Server::initArgs()
{
	while(1)
	{
		status = poll(poll_fds, poll_count, 5000);
		if (status < 0)
		{
			perror("poll");
			exit(EXIT_FAILURE);
		}
		else if (status ==0)
		{
			std::cout << "Waiting ..." << std::endl;
			continue;
		}
		for (int i = 0; i < poll_count; i++) 
		{
            if ((poll_fds[i].revents & POLLIN) != 1) {
                // La socket n'est pas prête à être lue
                // on s'arrête là et on continue la boucle
                continue ;
            }
            printf("[%d] Ready for I/O operation\n", poll_fds[i].fd);
            // La socket est prête à être lue !
            if (poll_fds[i].fd == master_socket) 
			{
            // La socket est notre socket serveur qui écoute le port
                Server::newConnection();
            }
            else 
			{
                // La socket est une socket client, on va la lire
                Server::read_data_from_socket(i);
            }
        }
    }
}

void	Server::initMapCommand()
{
	map_command.insert(std::pair<std::string, void (Server::*)(int, char *)>("PRIVMSG", &Server::Privmsg));
	map_command.insert(std::pair<std::string, void (Server::*)(int, char *)>("QUIT", &Server::Quit));
   	map_command.insert(std::pair<std::string, void (Server::*)(int, char *)>("JOIN", &Server::Join));
   	map_command.insert(std::pair<std::string, void (Server::*)(int, char *)>("PART", &Server::Part));
   	map_command.insert(std::pair<std::string, void (Server::*)(int, char *)>("KICK", &Server::Kick));
   	map_command.insert(std::pair<std::string, void (Server::*)(int, char *)>("INVITE", &Server::Invite));
   	map_command.insert(std::pair<std::string, void (Server::*)(int, char *)>("TOPIC", &Server::Topic));
}

void 	Server::newConnection()
{
	new_socket = accept(master_socket, NULL, NULL);
	if (new_socket == -1) 
	{
		perror("accept");
		exit(EXIT_FAILURE);
	}
	add_to_poll_fds(new_socket);
}

void	Server::acceptUser(int new_socket, std::string buff)
{
	//variables user
	std::string	cap_ls;
	std::string	password;
	std::string	nick_name;
	std::string	name;
	int pos;

	if (std::count(buff.begin(), buff.end(), '\n') < 4)
		return;

	pos = buff.find('\n');
	cap_ls = buff.substr(0, pos);
	std::cout << cap_ls << std::endl;
	buff.erase(0,pos + 1);

	pos = buff.find('\n');
	password = buff.substr(0, pos);

	/*check si ya un passe et si c le bon
	if (password.find("PASS :") == std::string::npos)
	{
		std::cout << "Missing password\n";
	   	send(new_socket, "461 Missing password\n", strlen("461 Missing password\n"), 461);
		return ;
	}*/
	password = password.substr(6);
	std::cout << password << std::endl;
	/*
	if (password != this->pass)
	{
		std::cout << "Wrong password\n";
		send(new_socket, "464 Password incorect\n", strlen("464 Password incorect\n"), 464);
		return ;
	}*/
	buff.erase(0,pos + 1);

	pos = buff.find('\n');
	nick_name = buff.substr(0, pos);
	nick_name = nick_name.substr(5);
	std::cout << nick_name << std::endl;
	/*
	for (; it != client_socket.end(); it++)
	{
		if (it->second->getNickName() == nick_name)
		{
			error = "433 " + nick_name + ", nickname is already in use\n";
			send(new_socket, error.c_str(), error.length(), 433);
			return ;
		}
	}*/
	buff.erase(0,pos + 1);


	name = buff;
	name = name.substr(name.find(":") + 1);
	std::cout << name << std::endl;
	client_socket[new_socket] = new User(new_socket, nick_name, name);
}
