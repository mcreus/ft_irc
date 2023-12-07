/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcreus <mcreus@student.42perpignan.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/27 14:05:16 by mcreus            #+#    #+#             */
/*   Updated: 2023/12/04 18:00:49 by mcreus           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "User.hpp"


User::User()
{
    
}

User::User(std::string const &new_nickName, std::string const &new_name)
{
    this->name = new_name;
    this->nickName = new_nickName;
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
