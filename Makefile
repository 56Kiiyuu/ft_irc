NAME = ircserv
INC = includes
SRCS =	srcs/main.cpp \
		srcs/Parsing.cpp \
		srcs/Server.cpp \
		srcs/Client.cpp
OBJS = $(SRCS:.cpp=.o)

CPPFLAGS = -Wall -Werror -Wextra -std=c++98 -g -I$(INC)
CPP = c++

all: $(NAME)

$(NAME): $(OBJS)
	$(CPP) $(CPPFLAGS) $(OBJS) -o $(NAME)

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
