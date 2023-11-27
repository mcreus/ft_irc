#ifndef USER_HPP
# define USER_HPP

# include <iostream>
# include <iomanip>
# include <string>

class User
{
	public:

		User();
		User(std::string const &nickName, std::string const &name);
		~User();
		std::string const	&getNickName() const;
		std::string const	&getName() const;

	private:

		std::string	_nickName;
		std::string _name;

};

#endif