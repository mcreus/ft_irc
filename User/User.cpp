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

User::User(std::string const &nickName, std::string const &name)
{
    this->_name = name;
    this->_nickName = nickName;
}

User::~User()
{
    
}

std::string const	&User::getNickName() const
{
    return (this->_nickName);
}

std::string const	&User::getName() const
{
    return (this->_name);
}