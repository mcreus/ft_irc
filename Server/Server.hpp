#ifndef SERVER_HPP
# define SERVER_HPP

# include <iostream>
# include "../User/User.hpp"

class Server
{
	public:
    
		Server(int max_client);
		~Server(void);
	
	private:
    
	    	User	*Users;
	    	int	nb_max_client;
	    	int	nb_users;
};

#endif
