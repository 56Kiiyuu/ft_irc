#include <iostream>
#include "Server.hpp"
#include "Client.hpp"
#include "Message.hpp"

#include <signal.h>

Server server;

void	handler(int sig, siginfo_t *info, void *context)
{
	(void)info;
	(void)context;

	if (sig == SIGINT)
	{
		server.getRun() = 0;
	}
}


int main(int argc, char **argv)
{
	if (argc != 3)
	{
		std::cerr << "Usage: ./ircserv <port> <password>" << std::endl;
		return 1;
	}

	signal(SIGPIPE, SIG_IGN);
	std::string password = argv[2];
	server.setPassword(password);
	struct sigaction sa;

	sa.sa_sigaction = handler;
	sa.sa_flags = SA_SIGINFO;
	sigemptyset(&sa.sa_mask);
	sigaction(SIGINT, &sa, NULL);

	server.startServer();
	return 0;
}
