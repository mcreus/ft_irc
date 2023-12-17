#include "User.hpp"

User::User()
{
    
}

User::User(int new_fd, std::string const &new_nickName)
{
	this->nickName = new_nickName;
	this->fd_user = new_fd;

	write(new_fd, "Info Welcome in Corddis\n", 24);
}

User::~User()
{
    
}

std::string const	&User::getNickName() const
{
    return (this->nickName);
}

std::string const	&User::getName() const
{
    return (this->name);
}

int	User::getFd_user()
{
	return (this->fd_user);
}

void	User::setName(std::string const &new_name)
{
	this->name = new_name;
}

/*void	User::addChannel(std::string name, Channel *channel)
{
	user_channel[name] = channel;
}

std::map<std::string, Channel *>	User::getChannel()
{
	return (this->user_channel);
}*/
