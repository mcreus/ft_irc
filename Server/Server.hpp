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
        void    createChannel();
        void    joinChannel();
    

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
        int	i;
        fd_set  readfds;
        std::map<int, User*>   client_socket;
        std::map<std::string, void (Server::*)(int fd, char *buffer)>  map_command;
        std::string	pass;
        std::map<std::string, Channel *> _channel;
        //Channel _channel;
};

#endif
