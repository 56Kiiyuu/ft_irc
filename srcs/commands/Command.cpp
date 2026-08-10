#include "Command.hpp"
#include "Server.hpp"
#include "Client.hpp"
#include "Message.hpp"
#include <iostream>
#include <sstream>

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
	_commands["TOPIC"] = &cmdTopic;
	_commands["KICK"] = &cmdKick;
	_commands["PART"] = &cmdPart;
	// _commands["QUIT"] = &cmdQuit;
	_commands["INVITE"] = &cmdInvite;

}

CommandManager::~CommandManager()
{}

std::vector<std::string> splitString(const std::string& str, char delimiter)
{
	std::vector<std::string> tokens;
	std::stringstream ss(str);
	std::string token;
	while (std::getline(ss, token, delimiter))
	{
			tokens.push_back(token);
	}
	return tokens;
}

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
