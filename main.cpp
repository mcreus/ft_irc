/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcreus <mcreus@student.42perpignan.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/24 16:27:03 by mcreus            #+#    #+#             */
/*   Updated: 2023/11/24 17:08:56 by mcreus           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <iomanip>
#include <string>

int	main (int ac, char **av)
{
	if (ac != 3)
		std::cerr << "Error: Your program must be writed as : <program_name><port><password>" << std::endl;
	else
	{
		int	port = std::atoi(av[1]);
		std::string	password = av[2];
	}
	return (0);
}