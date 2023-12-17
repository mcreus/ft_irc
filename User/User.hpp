#ifndef USER_HPP
# define USER_HPP

# include <iostream>
# include <iomanip>
# include <string>
# include <unistd.h>
# include <map>
# include "../Channel/Channel.hpp"

class Channel;

class User
{
	public:

		User(void);
		User(int new_fd, std::string const &new_nickName);
		~User(void);
		std::string const	&getNickName() const;
		std::string const	&getName() const;
		int			getFd_user();
		void			setName(std::string const &new_name);
		//void			addChannel(std::string, Channel *);
		//std::map<std::string, Channel *>	getChannel();

	private:

		std::string	nickName;
		std::string	name;
		int		fd_user;
		//std::map<std::string, Channel *>	user_channel;
};

#endif
