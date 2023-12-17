#include "Channel.hpp"

Channel::Channel(User &user, std::string const &name): _name(name), _pass(""), host(0), nextUserNumber(0)
{
	addUser(&user);
	host = &user;
	addAdmin(&user);
}

Channel::Channel(User &user, std::string const &name, std::string const &pass): _name(name), _pass(pass), host(0), nextUserNumber(0)
{
	addUser(&user);
	host = &user;
	addAdmin(&user);
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
}

void Channel::removeUser(std::string target)
{
    std::map<int, User*>::iterator it = users.begin();
    std::set<User*>::iterator ait = admins.begin();
    while (it->second->getNickName() != target && it != users.end())
    	it++;
    if (it != users.end()) 
    {
        users.erase(it);
        admins.erase(ait);
        if (host == it->second) 
        {
            if (!users.empty())
            {
                host = users.begin()->second;
                addAdmin(host);
			}
            else
				host = 0;
        }
    }
    if (users.empty())
    	delete this;
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

void	Channel::setTopic(std::string new_topic)
{
	this->_topic = new_topic;
}

std::string	Channel::getTopic()
{
	return (this->_topic);
}
