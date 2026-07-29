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


int main() {
	//create serv
	/*Server server;
	// fake client
	Client dummyClient;
	std::cout << "Base Nickname : " << dummyClient.getNickname() << std::endl;
	std::cout << "--------------------------------" << std::endl;
	// simulation msg
	std::string testCommands[] = {
		"PASS superpassword123\r\n",
		"NICK Pogo\r\n",
		"JOIN #general\r\n",
		"PRIVMSG #general :Hello World!\r\n",
		"UNKNOWN_CMD arg1 arg2\r\n" //invalid cmd
	};
	for (size_t i = 0; i < 5; ++i) {
		std::cout << "\n[Reception network] : " << testCommands[i];
		// parsing
		Message msg(testCommands[i]);
		// routing to the correct cmd
		server.routeCommand(dummyClient, msg);
	}*/

	struct sigaction sa;

	sa.sa_sigaction = handler;
	sa.sa_flags = SA_SIGINFO;
	sigemptyset(&sa.sa_mask);
	sigaction(SIGINT, &sa, NULL);

	server.startServer();
	return 0;
}
