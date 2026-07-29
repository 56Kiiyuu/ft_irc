#include "Command.hpp"
#include "Server.hpp"
#include "Client.hpp"
#include "Message.hpp"
#include <iostream>

void	cmdPing(Server& server, Client::ClientInfo& sender, int socketFd, const Message& msg)
{
	(void)server;
	(void)sender;

	if (!msg.getParams().empty())
	{
		std::string reply = ":server PONG server " + msg.getParams()[0] + "\r\n";
		send(socketFd, reply.c_str(), reply.length(), 0);
	}
}
