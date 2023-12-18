#include "Server.hpp"

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
	std::cout << "destructor de serv\n";
	std::map<int, User*>::iterator	it = client_socket.begin();
	std::map<int, User*>::iterator	ite = client_socket.end();
	while (it != ite)
	{
		delete it->second;
		it++;
	}
	std::map<std::string, Channel*>::iterator it3 = _channels.begin();
	while (it3 != _channels.end())
	{
		delete it3->second;
		it3++;
	}
}
