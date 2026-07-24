NAME = ircserv
INC = includes
SRCS =	srcs/main.cpp \
		srcs/Parsing.cpp \
		srcs/Server.cpp \
		srcs/Client.cpp
OBJS = $(SRCS:.cpp=.o)
DEPS = $(OBJS:.o=.d)

CPPFLAGS = -Wall -Werror -Wextra -std=c++98 -I$(INC) -MMD -g
CPP = c++

all: $(NAME)

$(NAME): $(OBJS)
	$(CPP) $(CPPFLAGS) $(OBJS) -o $(NAME)

clean:
	rm -f $(OBJS) $(DEPS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re

-include $(DEPS)
