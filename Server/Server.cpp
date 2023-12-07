#include "Server.hpp"

Server::Server(int max_client)
{
	this->nb_max_client = max_client;
	this->Users = new User[max_client];
	this->nb_users = 0;
}

Server::~Server(void)
{
	delete [] this->Users;
}
