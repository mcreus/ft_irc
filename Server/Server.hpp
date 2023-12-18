#ifndef SERVER_HPP
# define SERVER_HPP

# include <iomanip>
# include <cstring>
# include <cstdlib>
# include <unistd.h>
# include <stdio.h>
# include <map>
# include <algorithm>
#include <signal.h>
# include "../Channel/Channel.hpp"
# include "../User/User.hpp"

//socket
# include <sys/types.h>
# include <sys/socket.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <sstream>

#include <poll.h>

class Server
{
	public:

		Server(char **av);
		~Server();
		//init
		void	initServer();
		void	initArgs();
		void	newConnection();
		void	acceptUser(int new_socket, std::string buff);
		void	initMapCommand(void);
		//command
		void	command(int fd, char *buffer);
		void	Privmsg(int senderFd, char *buffer);
		void	PrivmsgUser(int fd, char *buffer);
		void	PrivmsgChannel(int fd, char *buffer);
		void	Quit(int fd, char *buffer);
		void	Kick(int fd, char *buffer);
		void	Invite(int fd, char *buffer);
		void	Topic(int fd, char *buffer);
		void	Part(int fd, char *buffer);
		void	Join(int fd, char *buffer);
		void	Mode(int fd, char *buffer);
		void	setUserName(int fd, char *buffer);
		//gestion sockets
		void	read_data_from_socket(int i);
		void	add_to_poll_fds(int new_socket);
		void	del_from_poll_fds(int i);
		//checker
		bool	checkChannel(std::string, int);
		bool	checkUserInChannel(std::string, int, std::string);
		bool	userCanActInChannel(std::string, int );
		std::map<int, User*>::iterator	checkUserInServer(std::string, int);
	

	private:
		
		int	opt;
		int	master_socket;
		int	addrlen;
		int	new_socket;
		int	max_clients;
		int	activity;
		int	valread;
		int	max_fd;
		int	port;
		int	status;
		struct  pollfd poll_fds[21];
		int	poll_size;
		int	poll_count;
		std::map<int, std::string> _buffer;

		std::map<int, User*>   client_socket;
		std::map<std::string, void (Server::*)(int fd, char *buffer)>  map_command;
		std::string	pass;
		std::map<std::string, Channel *>	_channels;
};

#endif
