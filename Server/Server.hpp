#ifndef SERVER_HPP
# define SERVER_HPP

# include <iomanip>
# include <cstring>
# include <cstdlib>
# include <unistd.h>
# include <stdio.h>
# include <map>
# include "../User/User.hpp"

//socket
# include <sys/types.h>
# include <sys/socket.h>
# include <netinet/in.h>
# include <arpa/inet.h>

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
        void	sendPrivateMessage(int send_user, std::string buffer);
        void	commandMessage(int fd, char *buffer);
        int const   &getMasterSocket() const;
        int const   &getaddrlen() const;
        int const   &getNewSocket() const;
        int const   &getMaxClients() const;
        int const   &getClientSocket() const;
        int const   &getActivity() const;
        int const   &getValread() const;
        int const   &getFd() const;
        int const   &getMaxFd() const;
        int const   &getPort() const;

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
        std::string	pass;
        std::string	channelName;
};

#endif
