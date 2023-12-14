#include "Channel.hpp"

Channel::Channel(User &user, std::string const &name): _name(name), _admin(user), _pass("")
{

}

Channel::Channel(User &user, std::string const &name, std::string const &pass): _name(name), _admin(user), _pass(pass)
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
