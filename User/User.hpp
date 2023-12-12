#ifndef USER_HPP
# define USER_HPP

# include <iostream>
# include <iomanip>
# include <string>
# include <unistd.h>

class User
{
	public:

		User(void);
		User(int new_fd, std::string const &new_nickName, std::string const &new_name);
		~User(void);
		std::string const	&getNickName() const;
		std::string const	&getName() const;
		int			getFd_user();

	private:

		std::string	nickName;
		std::string	name;
		int		fd_user;

};

#endif
