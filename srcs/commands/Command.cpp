#include "Command.hpp"
#include "Server.hpp"
#include "Client.hpp"
#include "Message.hpp"
#include <iostream>

CommandManager::CommandManager()
{
	_commands["CAP"] = &cmdCap;
	_commands["PASS"] = &cmdPass;
	_commands["NICK"] = &cmdNick;
	_commands["USER"] = &cmdUser;
	_commands["JOIN"] = &cmdJoin;
	_commands["PRIVMSG"] = &cmdPrivmsg;
	_commands["MODE"] = &cmdMode;
	_commands["PING"] = &cmdPing;
}

CommandManager::~CommandManager()
{}

void CommandManager::routeCommand(Server& server, Client::ClientInfo& sender, int socketFd, const Message& msg)
{
	std::string cmd = msg.getCommand();

	std::map<std::string, CmdHandler>::iterator it = _commands.find(cmd);
	if (it != _commands.end())
	{
		CmdHandler handler = it->second;
		handler(server, sender, socketFd, msg);
	}
	else
	{
		std::cout << "Commande inconnue : " << cmd << std::endl;
		// sendReply(...) ERR_UNKNOWNCOMMAND (421)
	}
}
