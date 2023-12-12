/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcreus <mcreus@student.42perpignan.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/27 14:05:16 by mcreus            #+#    #+#             */
/*   Updated: 2023/12/07 18:31:21 by gcot             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "User.hpp"


User::User()
{
    
}

User::User(int new_fd, std::string const &new_nickName, std::string const &new_name)
{
	this->name = new_name;
	this->nickName = new_nickName;
	this->fd_user = new_fd;
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
