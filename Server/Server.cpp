#include "Server.hpp"

struct sockaddr_in address;

Server::Server(char **av)
{
	master_socket = socket(AF_INET, SOCK_STREAM, 0);
	max_clients = 20;
	opt = 1;
	port = atoi(av[1]);
	pass = av[2];
	poll_size = 20;
}

Server::~Server()
{
	std::map<int, User*>::iterator	it = client_socket.begin();
	std::map<int, User*>::iterator	ite = client_socket.end();
	while (it != ite)
	{
		delete it->second;
		it++;
	}
}

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
   	map_command.insert(std::pair<std::string, void (Server::*)(int, char *)>("JOIN", &Server::joinChannel));
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

void Server::read_data_from_socket(int i )
{
    char buffer[1024];
    int bytes_read;
    int sender_fd;

	sender_fd = poll_fds[i].fd;
	memset(&buffer, '\0', sizeof buffer);
	bytes_read = recv(sender_fd, buffer, 2024, 0);
	if (bytes_read <= 0) 
	{
		if (bytes_read == 0) 
		{
			Server::disconnection(sender_fd);
		}
		else 
		{
			std::cout << "error recev\n";
		}
		close(sender_fd); // Ferme la socket
		Server::del_from_poll_fds(i);
	}
	else 
	{
		if (client_socket.find(sender_fd) == client_socket.end())
		{
			_buffer[sender_fd] += buffer;
			Server::acceptUser(sender_fd, _buffer[sender_fd]);
		}
		else
		{
			this->command(sender_fd, buffer);
		}
    }

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

void Server::Privmsg(int senderFd, char *buffer)
{
	std::string message = buffer;
	std::istringstream iss(message);
	std::string command, target, msg;
	iss >> command >> target;
	std::getline(iss, msg);
	msg = msg.substr(msg.find_first_not_of(" \t"));
	std::map<int, User*>::iterator senderIt = client_socket.find(senderFd);
	if (senderIt == client_socket.end())
	{
		std::cerr << "User not found" << std::endl;
		return;
	}
	for (std::map<int, User*>::iterator it = client_socket.begin(); it != client_socket.end(); ++it)
	{
		if (it->second->getNickName() == target)
		{
			std::cout << "Private message from " << senderIt->second->getNickName() << " to " << target << ": " << message << std::endl;
			std::string privateMessage = "PRIVMSG" + senderIt->second->getNickName() + ' ' + message + "\n";
			send(it->first, privateMessage.c_str(), privateMessage.length(), 0);
			return;
			}
	}
	std::cerr << "User not found" << std::endl;
}

void	Server::command(int fd, char *buffer)
{
	std::map<std::string, void (Server::*)(int fd, char *buffer)>::iterator	it = map_command.begin();
	std::string	msg = buffer;
	std::string	command = msg.assign(msg, 0, msg.find_first_of(" \t"));
	std::cout << std::endl << command << std::endl;
	while (it != map_command.end() && it->first != command)
		it++;
	if (it != map_command.end())
		(this->*(it->second))(fd, buffer);
}

void	Server::disconnection(int fd)
{
	getpeername(fd , (struct sockaddr*)&address , (socklen_t*)&addrlen);
	std::cout << "Host disconnected , ip " << inet_ntoa(address.sin_addr) << " , port " << ntohs(address.sin_port) << std::endl;
	delete client_socket[fd];
	client_socket.erase(fd);
	_buffer.erase(fd);
	//close(fd);
	if (client_socket.empty())
		max_fd = master_socket;
	else
	{
		std::map<int, User*>::iterator	it_new = client_socket.end();
		it_new--;
		max_fd = it_new->first;
	}
}

// Ajouter un fd qui attend en lcture et ecriture dans le poll_fds
void Server::add_to_poll_fds(int new_fd) 
{
    poll_fds[poll_count].fd = new_fd;
    poll_fds[poll_count].events = POLLIN | POLLOUT;
	poll_fds[poll_count].revents = 0;
    poll_count++;
}

// Supprimer un fd du tableau poll_fds
void Server::del_from_poll_fds(int i) 
{
	// Copie le fd de la fin du tableau à cet index
	poll_fds[i] = poll_fds[poll_count - 1];
	poll_count--;
}

void Server::joinChannel(int fd, char *buffer)
{
	// Extrait le nom du canal depuis la commande
	std::string message = buffer;
	std::istringstream iss(message);
	std::string command, channelName;
	iss >> command >> channelName;
	std::map<int, User*>::iterator it = client_socket.find(fd);
	std::string	user_name = it->second->getNickName();

	// Vérifie si le canal existe
	if (_channels.find(channelName) == _channels.end())
	{
		_channels[channelName] = new Channel(*(client_socket[fd]), channelName);
		//std::string success = ":" + it->second->getNickName() + "!~" + it->second->getNickName()[0] + "@localhost JOIN " + channelName + "\n";
		std::string success = ":" + user_name + "!~" + user_name[0] + "@0::1 JOIN " + channelName + "\n";
		std::cout << success << "\n";
		send(fd, success.c_str(), success.length(), 0);
		success = ":localhost MODE " + channelName + " +nt\n";
		send(fd, success.c_str(), success.length(), 0);
		success = ":localhost 353 " + user_name + " = " + channelName + " :@" + user_name + "\n";
		send(fd, success.c_str(), success.length(), 0);
		success = ":localhost 366 " + user_name + " " + channelName + " :End of /NAMES list.\n";
		send(fd, success.c_str(), success.length(), 0);
	}
	// Vérifie si l'utilisateur est déjà dans le canal
	else
	{
		if (_channels[channelName]->getUsers().find(fd) != _channels[channelName]->getUsers().end())
		{
			std::string error = "443 " + channelName + " :User is already in channel\n";
			send(fd, error.c_str(), error.length(), 443);
		}
		else
		{
			//ajout d un utilisateur dans un channel existant
			User *admin = _channels[channelName]->getAdmin();
			std::string success = ":" + user_name + "!~" + user_name[0] + "@localhost JOIN " + channelName + "\n";
			std::cout << success << "\n";
			send(fd, success.c_str(), success.length(), 0);
			success = ":localhost 332 " + user_name + " " + channelName + " :This is my cool channel! https://localhost\n";
			send(fd, success.c_str(), success.length(), 0);
			success = ":localhost 333 " + user_name + " " + admin->getNickName() + "!~" + admin->getNickName()[0] + "@@localhost 1547691506\n";
			send(fd, success.c_str(), success.length(), 0);
			success = ":localhost 353 " + user_name + " @ " + channelName + " :" + user_name + " @" + admin->getNickName() + "\n";
			send(fd, success.c_str(), success.length(), 0);
			success = ":localhost 366 " + user_name + " " + channelName + " :End of /NAMES list.\n";
			send(fd, success.c_str(), success.length(), 0);
			_channels[channelName]->addUser(it->second);
		}
	}
	
}
