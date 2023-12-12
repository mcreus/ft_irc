#include "Server.hpp"

struct sockaddr_in address;

Server::Server(char **av)
{
	master_socket = socket(AF_INET, SOCK_STREAM, 0);
	max_clients = 20;
	opt = 1;
	port = atoi(av[1]);
	pass = av[2];
	//client_socket.insert(std::pair<int, User*>(master_socket, new User));
}

Server::~Server()
{
	std::map<int, User*>::iterator	it = client_socket.begin();
	std::map<int, User*>::iterator	ite = client_socket.end();
	while (it != ite)
	{
		delete it->second;
		it++;
	}
}

void	Server::initServer()
{
	if (master_socket == 0)
	{
		std::cerr << "Socket failed" << std::endl;
		exit(EXIT_FAILURE);
	}
	if (setsockopt(master_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt)) < 0)
	{
		std::cerr << "Setsockopt" << std::endl;
		exit(EXIT_FAILURE);
	}
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons( port );
	if (bind(master_socket, (struct sockaddr *)&address, sizeof(address)) < 0)
	{
		std::cerr << "Bind failed" << std::endl;
		exit(EXIT_FAILURE);
	}
	std::cout << "Listener on port " << port << std::endl;
	if (listen(master_socket, 3) < 0)   
	{
		perror("listen");
		exit(EXIT_FAILURE);
	}
	addrlen = sizeof(address);
	std::cout << "Waiting for connections ..." << std::endl;
}

void	Server::initArgs()
{
	FD_ZERO(&readfds);
	FD_SET(master_socket, &readfds);
	max_fd = master_socket;
	std::map<int, User*>::iterator	it = client_socket.begin();
	for (; it != client_socket.end(); it++)
	{
		//std::cout << "fd = " << it->first << std::endl;
		FD_SET(it->first ,&readfds);
		if(it->first > max_fd)
			max_fd = it->first;
	}
	//std::cout << "max_fd" << max_fd << std::endl;
	activity = select(max_fd + 1 , &readfds , NULL , NULL , NULL);
	if (activity < 0)
		perror("errno");
}

void	Server::newConnection()
{
	new_socket = 0;
	if (FD_ISSET(master_socket, &readfds))
	{
		if ((new_socket = accept(master_socket,(struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0)
		{
			perror("accept");
			exit(EXIT_FAILURE);
		}
		if (client_socket.find(new_socket) == client_socket.end())
			this->acceptUser(new_socket);
		else
			std::cout << "Client is already connected !!!" << std::endl;
		/*std::cout << "New connection , socket fd is " << new_socket << " , ip is : " << inet_ntoa(address.sin_addr) << ", port : " << ntohs(address.sin_port) << std::endl;
		send(new_socket, message.c_str(), message.length(), 0);
		std::cout << "Welcome message sent successfully\n";*/
	}
}

void	Server::acceptUser(int new_socket)
{
	//variables user
	std::string	cap_ls;
	std::string	password;
	std::string	nick_name;
	std::string	name;

	//variables read
	std::size_t	found;
	char		buf[1024];
	int		end;

	end = read(new_socket, buf, 1024);
	buf[end - 1] = '\0';
	cap_ls = buf;
	std::cout << cap_ls << std::endl;
	end = read(new_socket, buf, 1024);
	buf[end - 1] = '\0';
	password = buf;
	found = password.find("PASS :");
	if (found == std::string::npos)
	{
		std::cout << "Missing password\n";
		return ;
	}
	password = password.substr(6);
	std::cout << password << std::endl;
	if (password != this->pass)
	{
		std::cout << "Wrong password\n";
		return ;
	}
	end = read(new_socket, buf, 1024);
	buf[end - 1] = '\0';
	nick_name = buf;
	nick_name = nick_name.substr(5);
	end = read(new_socket, buf, 1024);
	buf[end - 1] = '\0';
	name = buf;
	found = name.find(":");
	name = name.substr(found + 1);
	client_socket.insert(std::pair<int, User*>(new_socket, new User(new_socket, nick_name, name)));
}

void	Server::listenSocket()
{
	std::map<int, User*>::iterator	it = client_socket.begin();
	char	buffer[1024];
	for (; it != client_socket.end(); it++)
	{
		if (FD_ISSET( it->first , &readfds))
		{
			if ((valread = read(it->first, buffer, 1024)) == 0)
			{
				this->disconnection(it->first);
				it = client_socket.begin();
			}
			else
			{
				buffer[valread] = '\0';
				this->sendAllClient(it->first, buffer);
			}
		}
	}
}

void	Server::disconnection(int fd)
{
	getpeername(fd , (struct sockaddr*)&address , (socklen_t*)&addrlen);
	std::cout << "Host disconnected , ip " << inet_ntoa(address.sin_addr) << " , port " << ntohs(address.sin_port) << std::endl;
	client_socket.erase(fd);
	close(fd);
	if (client_socket.empty())
		max_fd = master_socket;
	else
	{
		std::map<int, User*>::iterator	it_new = client_socket.end();
		it_new--;
		max_fd = it_new->first;
	}
}

void	Server::sendAllClient(int fd, char *buffer)
{
	std::map<int, User*>::iterator	it = client_socket.find(fd);
	std::map<int, User*>::iterator	it_new = client_socket.begin();
	std::string	message = it->second->getNickName();

	message.append(" ");
	message.append(buffer);
	std::cout << message.c_str() << std::endl;
	for (; it_new != client_socket.end(); it_new++)
	{
		if (fd != it_new->first)
			send(it_new->first, message.c_str(), message.length(), 0);
	}
}
/*int const   &getMasterSocket() const;
int const   &getaddrlen() const;
int const   &getNewSocket() const;
int const   &getMaxClients() const;
int const   &getClientSocket() const;
int const   &getActivity() const;
int const   &getValread() const;
int const   &getFd() const;
int const   &getMaxFd() const;*/

int const   &Server::getPort() const
{
	return (port);
}
