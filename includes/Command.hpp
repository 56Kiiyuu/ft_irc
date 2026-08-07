#ifndef COMMAND_HPP
#define COMMAND_HPP

#include <string>
#include <map>
#include "Client.hpp"

class Server;
class Client;
class Message;

typedef void (*CmdHandler)(Server& server, Client::ClientInfo& sender, int socketFd, const Message& msg);

class CommandManager
{
	private:
		std::map<std::string, CmdHandler>	_commands;
	public:
		CommandManager();
		~CommandManager();

		void	routeCommand(Server& server, Client::ClientInfo& sender, int socketFd, const Message& msg);
};

void cmdCap(Server& server, Client::ClientInfo& sender, int socketFd, const Message& msg);
void cmdPass(Server& server, Client::ClientInfo& sender, int socketFd, const Message& msg);
void cmdNick(Server& server, Client::ClientInfo& sender, int socketFd, const Message& msg);
void cmdUser(Server& server, Client::ClientInfo& sender, int socketFd, const Message& msg);
void cmdJoin(Server& server, Client::ClientInfo& sender, int socketFd, const Message& msg);
void cmdPrivmsg(Server& server, Client::ClientInfo& sender, int socketFd, const Message& msg);
void cmdMode(Server& server, Client::ClientInfo& sender, int socketFd, const Message& msg);
void cmdPing(Server& server, Client::ClientInfo& sender, int socketFd, const Message& msg);
void cmdTopic(Server& server, Client::ClientInfo& sender, int socketFd, const Message& msg);
void cmdKick(Server& server, Client::ClientInfo& sender, int socketFd, const Message& msg);

#endif
