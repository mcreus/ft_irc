#include "Channel.hpp"

Channel::Channel(User &user, std::string const &name): _name(name), _pass(""), _admin(user)
{

}

Channel::Channel(User &user, std::string const &name, std::string const &pass): _name(name), _pass(pass), _admin(user)
{

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

std::set<int> const &Channel::getUsers() const
{
    return _users;
}

User& Channel::getAdmin() const
{
    return _admin;
}

void Channel::addUser(User *user)
{
    _users.insert(user->getFd_user());
}

void Channel::removeUser(int fd)
{
    _users.erase(fd);
}