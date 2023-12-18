#include "Channel.hpp"

Channel::Channel(User &user, std::string const &name): _name(name), _pass(""), host(0), nextUserNumber(0)
{
	addUser(&user);
	host = &user;
	addAdmin(&user);
    t = false;
	k = false;
	i = false;
	limiteUser = 0;
	nb_user = 1;
}

Channel::Channel(User &user, std::string const &name, std::string const &pass): _name(name), _pass(pass), host(0), nextUserNumber(0)
{
	addUser(&user);
	host = &user;
	addAdmin(&user);
    t = false;
	k = true;
	i = false;
	limiteUser = 0;
	nb_user = 1;
}

Channel::~Channel()
{

}

std::string const   &Channel::getName() const
{
	return (this->_name);
}

std::string const   &Channel::getPass() const
{
	return (this->_pass);
}

std::map<int, User *> const &Channel::getUsers() const
{
	return (this->users);
}

User *Channel::getHost() const
{
	return (this->host);
}

void Channel::addUser(User *user)
{
	users[nextUserNumber++] = user;
	nb_user++;
}

void Channel::removeUser(std::string target)
{
	User *tmp;
	std::map<int, User*>::iterator it = users.begin();
	while (it->second->getNickName() != target && it != users.end())
		it++;
	if (it != users.end()) 
	{
		nb_user--;
		if (isUserAdmin(it->second))
			removeAdmin(it->second);
		tmp = it->second;
		users.erase(it);
		if (host == tmp) 
		{
			if (!users.empty())
			{
				if (!admins.empty())
					host = *admins.begin();
				else
				{
					host = users.begin()->second;
					ModeOp(true, host->getNickName(), host->getFd_user());
				}
			}
			else
				host = 0;
		}
	}
}

void Channel::addAdmin(User *admin)
{
	admins.insert(admin);
}

void Channel::removeAdmin(User *admin)
{
	admins.erase(admin);
}

bool Channel::isUserAdmin(User *user) const
{
	return admins.find(user) != admins.end();
}

void Channel::addInvite(User *user)
{
	invite.insert(user);
}

bool Channel::isUserInvite(User *user) const
{
	return invite.find(user) != invite.end();
}

void Channel::removeInvite(User *user)
{
	invite.erase(user);
}

void	Channel::setTopic(std::string new_topic)
{
	this->_topic = new_topic;
}

std::string	Channel::getTopic()
{
	return (this->_topic);
}

void	Channel::ModeInvite(bool change, int fd)
{
	std::string	succes;
	std::map<int, User*>::iterator it = users.begin();
	if (this->i == change)
	{
		if (change)
		{
			std::cout << this->_name  << " :Channel already in invite mode only" << std::endl;
		}
		else
		{
			std::cout << this->_name  << " :Channel already not in invite mode only" << std::endl;
		}
		return ;
	}
	if (change)
		succes = ":localhost MODE " + this->_name + " +i:\n";
	else
		succes = ":localhost MODE " + this->_name + " -i:\n";
	it = users.begin();
	while (it != users.end())
	{
		send(it->second->getFd_user(), succes.c_str(), succes.length(), 0);
		it++;
	}
	this->i = change;
	fd++;
}

void	Channel::ModeTopic(bool change, int fd)
{
	std::string	succes;
	std::map<int, User*>::iterator it = users.begin();
	if (this->t == change)
	{
		if (change)
		{
			std::cout << this->_name  << " :Channel already in admin topic mode only" << std::endl;
		}
		else
		{
			std::cout << this->_name  << " :Channel already not in admin topic mode only" << std::endl;
		}
		return ;
	}
	if (change)
		succes = ":localhost MODE " + this->_name + " +t:\n";
	else
		succes = ":localhost MODE " + this->_name + " -t:\n";
	it = users.begin();
	while (it != users.end())
	{
		send(it->second->getFd_user(), succes.c_str(), succes.length(), 0);
		it++;
	}
	this->t = change;
	fd++;
}

void	Channel::ModeKey(bool change, std::string new_pass, int fd)
{
	std::string	succes;
	std::map<int, User*>::iterator it = users.begin();
	if (this->k == change && new_pass == "")
	{
		if (change)
		{
			std::cout << this->_name  << " :Channel pass is already on" << std::endl;
		}
		else
		{
			std::cout << this->_name  << " :Channel pass is already off" << std::endl;
		}
		return ;
	}
	if (change)
	{
		succes = ":localhost MODE " + this->_name + " +k:\n";
		if (new_pass != "")
			this->_pass = new_pass;
	}
	else
		succes = ":localhost MODE " + this->_name + " -k:\n";
	it = users.begin();
	while (it != users.end())
	{
		send(it->second->getFd_user(), succes.c_str(), succes.length(), 0);
		it++;
	}
	this->k = change;
	fd++;
}

void	Channel::ModeLimite(bool change, int new_limit, int fd)
{
	std::string	succes;
	std::map<int, User*>::iterator it = users.begin();
	if (this->l == change && !new_limit)
	{
		if (change)
		{
			std::cout << this->_name  << " :Channel limite already set" << std::endl;
		}
		else
		{
			std::cout << this->_name  << " :Channel limite already unset" << std::endl;
		}
		return ;
	}
	if (change)
	{
		succes = ":localhost MODE " + this->_name + " +l:\n";
		if (new_limit > 0)
			this->limiteUser = new_limit;
	}
	else
		succes = ":localhost MODE " + this->_name + " -l:\n";
	it = users.begin();
	while (it != users.end())
	{
		send(it->second->getFd_user(), succes.c_str(), succes.length(), 0);
		it++;
	}
	this->l = change;
	fd++;
}

void	Channel::ModeOp(bool change, std::string user, int fd)
{
	std::map<int, User*>::iterator it = users.begin();
	User *target;
	std::string error, succes;
	while (it != users.end() && it->second->getNickName() != user)
		it++;
	if (it == users.end())
	{
		error = ":localhost 442 " + this->_name + " :\n";
		std::cerr << user << " :User not in channel " << this->_name << std::endl;
		send(fd, error.c_str(), error.length(), 0);
		return ;
	}
	target = it->second;
	if (change)
	{
		if (this->isUserAdmin(target))
		{
			error = ":localhost 442 " + this->_name + " :\n";
			std::cerr << user << " :User already admin in channel " << this->_name << std::endl;
			send(fd, error.c_str(), error.length(), 0);
			return ;
		}
		this->addAdmin(target);
		succes = ":localhost MODE " + this->_name + " +o " + user +" :\n";
		std::cout << user << " :Promote to admin in channel " << this->_name << std::endl;
		it = users.begin();
		while (it != users.end())
		{
			send(it->second->getFd_user(), succes.c_str(), succes.length(), 0);
			it++;
		}
	}
	else
	{
		if (!this->isUserAdmin(target))
		{
			error = ":localhost 442 " + this->_name + " :\n";
			std::cerr << user << " :User isn't admin in channel " << this->_name << std::endl;
			send(fd, error.c_str(), error.length(), 0);
			return ;
		}
		this->removeAdmin(target);
		succes = ":localhost MODE " + this->_name + " -o " + user +" :\n";
		std::cout << user << " :Remove from admin in channel " << this->_name << std::endl;
		it = users.begin();
		while (it != users.end())
		{
			send(it->second->getFd_user(), succes.c_str(), succes.length(), 0);
			it++;
		}
	}
	fd++;
}

bool	Channel::VerifPassword(std::string password)
{
	if (_pass == "" || _pass == password || !k)
		return (true);
	return (false);
}

bool	Channel::VerifLimite()
{
	if (limiteUser <= nb_user && limiteUser != 0 && l)
		return (false);
	return (true);
}

bool	Channel::ChannelInvite()
{
	return this->i;
}

bool	Channel::ChannelTopic()
{
	return this->t;
}
