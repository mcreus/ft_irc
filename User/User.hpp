#ifndef CLIENT_HPP
# define CLIENT_HPP

# include <iostream>
# include <iomanip>
# include <string>

class Client
{
	public:

		Client();
		Client(std::string const &nickName, std::string const &name);
		~Client();
		std::string const	&getNickName() const;
		std::string const	&getName() const;

	private:

		std::string	_nickName;
		std::string _name;

};

#endif