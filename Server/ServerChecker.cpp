#include "Server.hpp"

bool	Server::checkChannel(std::string channelName, int senderFd)
{
	if (this->_channels.find(channelName) == this->_channels.end())
	{
		std::string error = ":localhost 401 " + channelName + " :\n";
		std::cerr << channelName << " :Channel not in server " << channelName << std::endl;
		send(senderFd, error.c_str(), error.length(), 0);
		return false;
	}
	return true;
}

bool	Server::checkUserInChannel(std::string channelName, int senderFd, std::string target)
{
	std::map<int, User*>::iterator it = this->checkUserInServer(target, senderFd);
	if (it == client_socket.end())
		return false;
	std::map<int, User*> users = this->_channels[channelName]->getUsers();
	std::map<int, User*>::iterator it2 = users.begin();
	while (it2 != users.end() && it2->second->getNickName() != target)
		it2++;
	if (it2 == users.end())
	{
		std::string error = ":localhost 441 " + client_socket.find(senderFd)->second->getNickName() + " " + target + " " + channelName + " :\n";
		std::cerr << target << " :User not in channel " << channelName << std::endl;
		send(senderFd, error.c_str(), error.length(), 0);
		return false;
	}
	return true;
}

//################################################//
//       different from "checkUserInChannel()"    //
// here we check if the client sending the request//
//        is in the channel, not the target       //
//################################################//
bool	Server::userCanActInChannel(std::string channelName, int senderFd)
{
	std::string	target = this->client_socket.find(senderFd)->second->getNickName();
	std::map<int, User*> users = this->_channels[channelName]->getUsers();
	std::map<int, User*>::iterator it2 = users.begin();
	while (it2 != users.end() && it2->second->getNickName() != target)
		it2++;
	if (it2 == users.end())
	{
		std::string error = ":localhost 442 " + channelName + " :\n";
		std::cerr << target << " :User not in channel " << channelName << std::endl;
		send(senderFd, error.c_str(), error.length(), 0);
		return false;
	}
	return true;
}

std::map<int, User*>::iterator	Server::checkUserInServer(std::string target, int senderFd)
{
	std::map<int, User*>::iterator it = client_socket.begin();
	while (it != client_socket.end() && it->second->getNickName() != target)
		it++;
	if (it == client_socket.end())
	{
		std::string error = ":localhost 401 " + target + " :\n";
		std::cerr << target << " :User not in server\n";
		send(senderFd, error.c_str(), error.length(), 0);
	}
	return it;
}
