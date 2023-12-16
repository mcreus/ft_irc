NAME	=	ircserv

CC		=	c++ 
CFLAGS	=	-Wall -Wextra -Werror -std=c++98

SRCS	=	main.cpp \
			Server/Server.cpp \
			Server/ServerInit.cpp \
			Server/ServerCommand.cpp \
			Server/ServerChecker.cpp \
			Server/ServerSocket.cpp \
			User/User.cpp \
			Channel/Channel.cpp
OBJS	=	$(SRCS:.cpp=.o)

%.o:%.cpp
		c++ -Wall -Wextra -Werror -std=c++98 -I./ -c $< -o $@

all: $(NAME)

$(NAME): $(OBJS)
		@echo "\033[0;33m\nCOMPILATION ircserv\n"
		$(CC) $(CFLAGS) $(OBJS) -o $(NAME)
		@echo "\033[1;32m\nCOMPILATION REUSSIE\n"


clean:
		@echo "\033[0;31m\nDELETING OBJS\n"
		rm -rf $(OBJS)
		@echo "\033[1;32m\nDONE\n"

fclean: clean
		@echo "\033[0;31m\nDELETING ircserv\n"
		rm -rf $(NAME)
		@echo "\033[1;32m\nDONE\n"

re: fclean all
