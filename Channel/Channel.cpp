#include "Channel.hpp"

Channel::Channel(User &user, std::string const &name): _name(name), _pass(""), admin(0), nextUserNumber()
{
	addUser(&user);
	admin = &user;
}

Channel::Channel(User &user, std::string const &name, std::string const &pass): _name(name), _pass(pass), admin(0), nextUserNumber()
{
	addUser(&user);
	admin = &user;
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

User *Channel::getAdmin() const
{
    return (this->admin);
}

void Channel::addUser(User *user)
{
    users[nextUserNumber++] = user;
}

void Channel::removeUser(std::string target)
{
    std::map<int, User*>::iterator it = users.begin();
    while (it->second->getNickName() != target && it != users.end())
    	it++;
    if (it != users.end()) 
    {
        users.erase(it);
        if (admin == it->second) 
        {
            if (!users.empty())
                admin = users.begin()->second;
            else
		admin = 0;
        }
    }
    if (users.empty())
    	delete this;
}

void	Channel::setTopic(std::string new_topic)
{
	this->_topic = new_topic;
}

std::string	Channel::getTopic()
{
	return (this->_topic);
}
