#include <iostream>
#include "Server.hpp"
#include "Client.hpp"
#include "Message.hpp"

int main() {
	//create serv
	Server server;
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
		std::cout << "\n[Reception reseau] : " << testCommands[i];
		// parsing
		Message msg(testCommands[i]);
		// routing to the correct cmd
		server.routeCommand(dummyClient, msg);
	}
	return 0;
}
