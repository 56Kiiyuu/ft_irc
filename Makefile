NAME = ircserv
INC = includes
SRCS =	srcs/main.cpp \
		srcs/Message.cpp \
		srcs/Server.cpp \
		srcs/Client.cpp \
		srcs/commands/Command.cpp \
		srcs/commands/Cap.cpp \
		srcs/commands/Pass.cpp \
		srcs/commands/Nick.cpp \
		srcs/commands/User.cpp \
		srcs/commands/Join.cpp \
		srcs/commands/Privmsg.cpp \
		srcs/commands/Mode.cpp \
		srcs/commands/Ping.cpp \
		srcs/commands/Topic.cpp \
		srcs/commands/Kick.cpp \
		srcs/commands/Part.cpp \
		srcs/Channels.cpp \
		srcs/commands/Invite.cpp

OBJS = $(SRCS:.cpp=.o)

CPPFLAGS = -Wall -Werror -Wextra -std=c++98 -g -I$(INC)
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
