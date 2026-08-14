#include <iostream>
#include "Server.hpp"
#include "Client.hpp"
#include "Message.hpp"
#include <csignal>
#include <string>
#include <cstdlib>

Server* g_server = 0;

void	handler(int sig, siginfo_t *info, void *context)
{
	(void)info;
	(void)context;

	if (sig == SIGINT)
	{
		std::cout << "\n[+] STOP Server claim" << std::endl;
		if (g_server != 0)
		{
			g_server->getRun() = 0;
		}
	}
}

int parsePort(const std::string& portStr)
{
	if (portStr.empty())
		return -1;

	char* endPtr;
	long port = std::strtol(portStr.c_str(), &endPtr, 10);

	if (*endPtr != '\0' || port < 1024 || port > 65535)
		return -1;

	return static_cast<int>(port);
}

int main(int argc, char **argv)
{
	if (argc != 3)
	{
		std::cerr << "Usage: ./ircserv <port> <password>" << std::endl;
		return 1;
	}

	int port = parsePort(argv[1]);
	if (port == -1)
	{
		std::cerr << "Error: Invalid port (must be a number between 1024 and 65535)" << std::endl;
		return 1;
	}

	signal(SIGPIPE, SIG_IGN);
	struct sigaction sa;
	sa.sa_sigaction = handler;
	sa.sa_flags = SA_SIGINFO;
	sigemptyset(&sa.sa_mask);
	sigaction(SIGINT, &sa, NULL);

	std::string password = argv[2];

	try
	{
		Server server(port, password);
		server.setPassword(password);
		g_server = &server;
		server.startServer();
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
		g_server = NULL;
		return 1;
	}
	g_server = NULL;
	std::cout << "[+] Server STOP" << std::endl;
	return 0;
}
