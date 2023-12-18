#include "Server.hpp"

void	Server::command(int fd, char *buffer)
{
	std::map<std::string, void (Server::*)(int fd, char *buffer)>::iterator	it = map_command.begin();
	std::string	msg = buffer;
	std::string	command = msg.assign(msg, 0, msg.find_first_of(" \t"));
	std::cout << buffer << std::endl;
	if (command[0] == '/')
		command = command.substr(1);
	while (it != map_command.end() && it->first != command)
		it++;
	if (it != map_command.end())
		(this->*(it->second))(fd, buffer);
}

void	Server::Privmsg(int fd, char *buffer)
{
	std::string message = buffer;
	//std::cout << message << std::endl;
	if (message.find("#") == std::string::npos)
		this->PrivmsgUser(fd, buffer);
	else
		this->PrivmsgChannel(fd, buffer);
}

void	Server::PrivmsgUser(int fd, char *buffer)
{
	std::string message = buffer;
	std::istringstream iss(message);
	std::string command, target;
	iss >> command >> target;
	std::map<int, User*>::iterator senderIt = client_socket.find(fd);
	for (std::map<int, User*>::iterator it = client_socket.begin(); it != client_socket.end(); ++it)
	{
		if (it->second->getNickName() == target)
		{
			message = message.substr(message.find_first_of(":"));
			std::string privateMessage = ":" + senderIt->second->getNickName() + " PRIVMSG " + it->second->getNickName() + " " + message;
			send(it->first, privateMessage.c_str(), privateMessage.length(), 0);
			return;
		}
	}
	std::string privateMessage = ":localhost 401 " + target + " :\n";
	send(senderIt->first, privateMessage.c_str(), privateMessage.length(), 0);
	std::cerr << "User not found" << std::endl;
}

void	Server::PrivmsgChannel(int fd, char *buffer)
{
	std::string message = buffer;
	std::istringstream iss(message);
	std::string command, target;
	std::string succes;
	iss >> command >> target;
	std::map<int, User*>::iterator senderIt = client_socket.find(fd);
	if (!this->checkChannel(target, fd))
		return;
	if (!this->userCanActInChannel(target, fd))
		return ;
	std::map<int, User*> user_in_ch = _channels[target]->getUsers();
	std::map<int, User*>::iterator it = user_in_ch.begin();
	User *host = _channels[target]->getHost();
	message = message.substr(message.find_first_of(":"));
	succes = ":" + senderIt->second->getNickName() + "!~" + host->getNickName()[0] + "@localhost PRIVMSG " + target + " " + message;
	std::cout << succes << std::endl;
	while (it != user_in_ch.end())
	{
		if (it->second->getNickName() != senderIt->second->getNickName())
			send(it->second->getFd_user(), succes.c_str(), succes.length(), 0);
		it++;
	}
}

void	Server::Join(int fd, char *buffer)
{
	// Extrait le nom du canal depuis la commande
	std::string message = buffer;
	std::istringstream iss(message);
	std::string command, channelName, password;
	iss >> command >> channelName >> password;
	std::map<int, User*>::iterator it = client_socket.find(fd);
	std::string	user_name = it->second->getNickName();

	// Vérifie si le canal existe
	if (_channels.find(channelName) == _channels.end())
	{
		_channels[channelName] = new Channel(*(client_socket[fd]), channelName, password);
		//std::string success = ":" + it->second->getNickName() + "!~" + it->second->getNickName()[0] + "@localhost JOIN " + channelName + "\n";
		std::string success = ":" + user_name + "!" + user_name[0] + "@0::1 JOIN " + channelName + "\n";
		std::cout << success << "\n";
		send(fd, success.c_str(), success.length(), 0);
		success = ":localhost MODE " + channelName + " +nt\n";
		send(fd, success.c_str(), success.length(), 0);
		success = ":localhost 353 " + user_name + " = " + channelName + " :@" + user_name + "\n";
		send(fd, success.c_str(), success.length(), 0);
		success = ":localhost 366 " + user_name + " " + channelName + " :End of /NAMES list.\n";
		send(fd, success.c_str(), success.length(), 0);
		_channels[channelName]->setTopic(":");
	}
	// Vérifie si l'utilisateur est déjà dans le canal
	else
	{
		//verifie si deja dans le channel
		if (_channels[channelName]->getUsers().find(fd) != _channels[channelName]->getUsers().end())
		{
			std::string error = ":localhost 443 " + user_name + " " + channelName + " :\n";
			std::cerr << user_name << " :User already in channel " << channelName << std::endl;
			send(fd, error.c_str(), error.length(), 0);
			return ;
		}
		if (_channels[channelName]->ChannelInvite())
		{
			if (!_channels[channelName]->isUserInvite(client_socket.find(fd)->second))
			{
				std::string error = ":localhost 473 " + channelName + " :\n";
				std::cerr << user_name << " :User not invite in channel " << channelName << std::endl;
				send(fd, error.c_str(), error.length(), 0);
				return ;
			}
			_channels[channelName]->removeInvite(client_socket.find(fd)->second);
		}
		if (!_channels[channelName]->VerifPassword(password))
		{
			std::string error = ":localhost 475 " + channelName + " :\n";
			std::cerr << "Invalid password in channel " << channelName << std::endl;
			send(fd, error.c_str(), error.length(), 0);
			return ;
		}
		if (!_channels[channelName]->VerifLimite())
		{
			std::string error = ":localhost 471 " + channelName + " :\n";
			std::cerr << "Channel is full " << channelName << std::endl;
			send(fd, error.c_str(), error.length(), 0);
			return ;
		}
		//ajout d un utilisateur dans un channel existant
		User *host = _channels[channelName]->getHost();
		std::string success = ":" + user_name + "!" + user_name[0] + "@localhost JOIN " + channelName + "\n";
		std::cout << success << "\n";
		send(fd, success.c_str(), success.length(), 0);
		success = ":localhost 332 " + user_name + " " + channelName + " :" + _channels[channelName]->getTopic() + "\n";
		send(fd, success.c_str(), success.length(), 0);
		success = ":localhost 333 " + user_name + " " + host->getNickName() + "!" + host->getNickName()[0] + "@localhost 1547691506\n";
		send(fd, success.c_str(), success.length(), 0);
		success = ":localhost 353 " + user_name + " @ " + channelName + " :" + user_name;
		std::map<int, User*> user_in_ch = _channels[channelName]->getUsers();
		std::map<int, User*>::iterator it2 = user_in_ch.begin();
		while (it2 != user_in_ch.end())
		{
			if (this->_channels[channelName]->isUserAdmin(it2->second))
				success += (" @" + it2->second->getNickName());
			else
				success += (" " + it2->second->getNickName());
			it2++;
		}
		success += "\n";
		send(fd, success.c_str(), success.length(), 0);
		success = ":localhost 366 " + user_name + " " + channelName + " :End of /NAMES list.\n";
		send(fd, success.c_str(), success.length(), 0);
		_channels[channelName]->addUser(it->second);
		success = ":" + user_name + "!" + user_name[0] + "@localhost JOIN :" + channelName + "\n";
		user_in_ch = _channels[channelName]->getUsers();
		it2 = user_in_ch.begin();
		while (it2 != user_in_ch.end())
		{
			if (it2->second->getNickName() != it->second->getNickName())
				send(it2->second->getFd_user(), success.c_str(), success.length(), 0);
			it2++;
		}
	}
}

void	Server::Part(int fd, char *buffer)
{
	std::string message = buffer;
	std::istringstream iss(message);
	std::string command, channelName;
	iss >> command >> channelName;
	std::map<int, User*>::iterator it = client_socket.find(fd);
	std::string	user_name = it->second->getNickName();

	if (_channels[channelName]->getUsers().find(fd) != _channels[channelName]->getUsers().end())
	{
		std::string error = ":localhost 442 " + channelName + " :\n";
		std::cerr << user_name << " :User not in channel " << channelName << std::endl;
		send(it->first, error.c_str(), error.length(), 0);
		return ;
	}
	std::map<int, User*> user_in_ch = _channels[channelName]->getUsers();
	std::map<int, User*>::iterator it2 = user_in_ch.begin();
	std::string success = ":" + user_name + "!~" + user_name[0] + "@localhost PART " + channelName + "\n";
	while (it2 != user_in_ch.end())
	{
		send(it2->second->getFd_user(), success.c_str(), success.length(), 0);
		it2++;
	}
	_channels[channelName]->removeUser(user_name);
}

void	Server::Quit(int fd, char *buffer)
{
	std::string	user_name = client_socket.find(fd)->second->getNickName();
	std::map<std::string, Channel*>::iterator it_ch = _channels.begin();
	while (it_ch != _channels.end())
	{
		//std::cout << it_ch->first << std::endl;
		if (this->checkUserInChannel(it_ch->first, fd, user_name))
		{
			it_ch->second->removeUser(user_name);
		}
		it_ch++;
	}
	std::map<int, User*>::iterator it = client_socket.begin();
	std::string success = ":" + user_name + "!" + user_name[0] + "@localhost QUIT :" + buffer;
	while (it != client_socket.end())
	{
		send(it->second->getFd_user(), success.c_str(), success.length(), 0);
		it++;
	}
	delete client_socket[fd];
	client_socket.erase(fd);
	_buffer.erase(fd);
	if (client_socket.empty())
		max_fd = master_socket;
	else
	{
		std::map<int, User*>::iterator	it_new = client_socket.end();
		it_new--;
		max_fd = it_new->first;
	}
}

void	Server::Kick(int fd, char *buffer)
{
	std::string message = buffer;
	std::istringstream iss(message);
	std::string command, channelName, target, reason;
	iss >> command >> channelName >> target;
	std::map<int, User*>::iterator it = client_socket.find(fd);
	std::string	user_name = it->second->getNickName();
	std::string	success;

	if (!this->checkChannel(channelName, fd))
		return ;
	if (!this->checkUserInChannel(channelName, fd, target))
		return ;
	if (!this->_channels[channelName]->isUserAdmin(client_socket.find(fd)->second))
	{
		std::string	error = ":localhost 481 :\n";
		send(fd, error.c_str(), error.length(), 0);
		std::cout << client_socket.find(fd)->second->getNickName() << " :User not operator in channel " << channelName << std::endl;
		return ;
	}
	if (message.find_first_of(":") != std::string::npos)
	{
		reason = message.substr(message.find_first_of(":"));
		success = ":" + user_name + "!~" + user_name[0] + "@localhost KICK " + channelName + " " + target + " " + reason +"\n";
	}
	else
		success = ":" + user_name + "!~" + user_name[0] + "@localhost KICK " + channelName + " " + target + " :" + user_name +"\n";
	std::map<int, User*> user_in_ch = _channels[channelName]->getUsers();
	std::map<int, User*>::iterator it2 = user_in_ch.begin();
	while (it2 != user_in_ch.end())
	{
		send(it2->second->getFd_user(), success.c_str(), success.length(), 0);
		std::cout << it2->second->getNickName() << std::endl;
		it2++;
	}
	_channels[channelName]->removeUser(target);
}

void	Server::Invite(int fd, char *buffer)
{
	std::string message = buffer;
	std::istringstream iss(message);
	std::string command, channelName, target;
	iss >> command >> target >> channelName;
	std::string	user_name = client_socket.find(fd)->second->getNickName();
	std::string	success = ":" + user_name + "~!" + user_name[0] + "@localhost INVITE " + target + " " + channelName + "\n";
	if (!this->checkChannel(channelName, fd))
		return ;
	if (!this->userCanActInChannel(channelName, fd))
		return ;
	if (checkUserInServer(target, fd) == client_socket.end())
		return ;
	send(checkUserInServer(target, fd)->first, success.c_str(), success.length(), 0);
	this->_channels[channelName]->addInvite(checkUserInServer(target, fd)->second);
}

void	Server::Topic(int fd, char *buffer)
{
	std::string message = buffer;
	std::istringstream iss(message);
	std::string command, channelName, topic, success;
	iss >> command >> channelName;
	std::string	user_name = client_socket.find(fd)->second->getNickName();
	if (message.find_first_of(":") != std::string::npos)
	{
		if (_channels[channelName]->ChannelTopic())
		{
			if (!_channels[channelName]->isUserAdmin(client_socket.find(fd)->second))
			{
				std::string	error = ":localhost 481 :\n";
				send(fd, error.c_str(), error.length(), 0);
				std::cout << client_socket.find(fd)->second->getNickName() << " :User not operator in channel " << channelName << std::endl;
				return ;
			}
		}
		topic = message.substr(message.find_first_of(":") + 1);
		this->_channels[channelName]->setTopic(topic);
		success = ":" + user_name + "!" + user_name[0] + "@localhost TOPIC " + channelName + " " + topic + "\n";
	}
	else
		success = ":localhost 332 " + command + " " + channelName + " " + _channels[channelName]->getTopic() + "\n";
	if (!this->checkChannel(channelName, fd))
		return ;
	if (!this->userCanActInChannel(channelName, fd))
		return ;
	send(fd, success.c_str(), success.length(), 0);
}

void	Server::Mode(int fd, char *buffer)
{
	std::string message = buffer;
	std::istringstream iss(message);
	std::string command, channelName, mode, target;
	iss >> command >> channelName >> mode >> target;
	bool	bo = false;
	
	if (!this->checkChannel(channelName, fd))
		return ;
	if (!this->userCanActInChannel(channelName, fd))
		return ;
	if (!this->_channels[channelName]->isUserAdmin(client_socket.find(fd)->second))
	{
		std::string	error = ":localhost 481 :\n";
		send(fd, error.c_str(), error.length(), 0);
		std::cout << client_socket.find(fd)->second->getNickName() << " :User not operator in channel " << channelName << std::endl;
		return ;
	}
	if (mode == "")
		return ;
	if (mode[0] == '+')
		bo = true;
	if (mode[1] == 'i')
		this->_channels[channelName]->ModeInvite(bo, fd);
	else if (mode[1] == 'k')
		this->_channels[channelName]->ModeKey(bo, target, fd);
	else if (mode[1] == 'o')
		this->_channels[channelName]->ModeOp(bo, target, fd);
	else if (mode[1] == 't')
		this->_channels[channelName]->ModeTopic(bo, fd);
	else if (mode[1] == 'l')
		this->_channels[channelName]->ModeLimite(bo, std::atoi(target.c_str()), fd);
	/*std::cout << command << std::endl;
	std::cout << channelName << std::endl;
	std::cout << mode << std::endl;
	std::cout << target << std::endl;
	(void)fd;*/
}

void	Server::setUserName(int fd, char *buffer)
{
	if (client_socket.find(fd) == client_socket.end())
		return ;
	std::string	name = buffer;
	name = name.substr(name.find(":") + 1);
	client_socket[fd]->setName(name);
}
