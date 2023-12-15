#ifndef USER_HPP
# define USER_HPP

# include <iostream>
# include <iomanip>
# include <string>
# include <unistd.h>
# include <map>

class User
{
	public:

		User(void);
		User(int new_fd, std::string const &new_nickName, std::string const &new_name);
		~User(void);
		std::string const	&getNickName() const;
		std::string const	&getName() const;
		int			getFd_user();
		void			addPrivateConv(int, User *);
		std::map<int, User*>	getPrivateConv();

	private:

		std::string	nickName;
		std::string	name;
		int		fd_user;
		std::map<int, User*>   private_conv;
};

#endif
