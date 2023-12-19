#include "Server.hpp"

void	Server::read_data_from_socket(int i )
{
	char buffer[1024] = {0};
	int bytes_read;
	int sender_fd;

	sender_fd = poll_fds[i].fd;
	bytes_read = recv(sender_fd, buffer, 2024, 0);
	if (bytes_read <= 0)
	{
		if (bytes_read == 0)
			;
		else 
			std::cout << "error recev\n";
		close(sender_fd); // Close socket
		Server::del_from_poll_fds(i);
	}
	else 
	{
		buffer[bytes_read] = 0;
		if (client_socket.find(sender_fd) == client_socket.end())
		{
			_buffer[sender_fd] += buffer;
			Server::acceptUser(sender_fd, _buffer[sender_fd]);
		}
		else
		{
			_buffer[sender_fd] += buffer;
			if (_buffer[sender_fd].find('\n') != std::string::npos)
			{
				char test[1024];
				std::strcpy(test, _buffer[sender_fd].c_str());
				this->command(sender_fd, test);
				_buffer[sender_fd] = "";
			}
		}
	}
}

// Add an fd that waits for reading and writing in poll_fds
void	Server::add_to_poll_fds(int new_fd) 
{
    poll_fds[poll_count].fd = new_fd;
    poll_fds[poll_count].events = POLLIN | POLLOUT;
	poll_fds[poll_count].revents = 0;
    poll_count++;
}

// Delete an fd from the poll_fds table
void	Server::del_from_poll_fds(int i) 
{
// Copy the fd from the end of the array to this index
	poll_fds[i] = poll_fds[poll_count - 1];
	poll_count--;
}
