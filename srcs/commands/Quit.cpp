#include "Command.hpp"
#include "Server.hpp"
#include "Client.hpp"
#include "Message.hpp"
#include "NumericReplies.hpp"
#include <iostream>
#include <sys/socket.h>

void cmdQuit(Server& server, Client::ClientInfo& sender, int socketFd, const Message& msg)
{
	(void)sender;
	std::string reason = "Leaving";
	if (!msg.getParams().empty() && !msg.getParams()[0].empty())
	{
		reason = msg.getParams()[0];
		if (reason[0] == ':')
			reason.erase(0, 1);
	}

	std::string errNotice = "ERROR :Closing Link: 127.0.0.1 (" + reason + ")\r\n";
	send(socketFd, errNotice.c_str(), errNotice.length(), 0);
	std::cout << "[-] Client FD " << socketFd << " deconnecte (QUIT : " << reason << ")." << std::endl;

	server.disconnectClient(socketFd, reason);
}
