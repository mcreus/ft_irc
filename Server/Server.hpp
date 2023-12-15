#ifndef SERVER_HPP
# define SERVER_HPP

# include <iomanip>
# include <cstring>
# include <cstdlib>
# include <unistd.h>
# include <stdio.h>
# include <map>
# include "../User/User.hpp"
# include "../Channel/Channel.hpp"

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
        void    initServer();
        void    initArgs();
        void    newConnection();
        void	acceptUser(int new_socket);
        void    listenSocket();
        void	disconnection(int fd);
        void	sendAllClient(int fd, char *buffer);
        void	Privmsg(int senderFd, char *buffer);
        void	initMapCommand(void);
        void	command(int fd, char *buffer);
        void    read_data_from_socket(int i);
        void    add_to_poll_fds(int new_socket);
        void    del_from_poll_fds(int i);
        void    joinChannel(int fd, char *buffer);
        void    addUser(int fd, const std::string &nick, const std::string &name);
    

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
        int status;
        struct  pollfd *poll_fds;
        int poll_size;
        int poll_count;

        std::map<int, User*>   client_socket;
        std::map<std::string, void (Server::*)(int fd, char *buffer)>  map_command;
        std::string	                                                   pass;
        std::map<std::string, Channel *>                               _channels;
        //Channel _channel;
};

#endif
