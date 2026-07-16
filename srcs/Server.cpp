#include "Server.hpp"
#include "Client.hpp"
#include <iostream>

Server::Server()
{
	initCommands();
}

Server::~Server()
{
}

void	Server::initCommands()
{
	_commands["NICK"] = &Server::execNick;
	_commands["JOIN"] = &Server::execJoin;
	_commands["PRIVMSG"] = &Server::execPrivmsg;
	_commands["PASS"] = &Server::execPass;
	// + plus tard;
}

void	Server::routeCommand(Client& sender, const Message& msg)
{
	std::string cmd = msg.getCommand();

	std::map<std::string, CmdFunc>::iterator it = _commands.find(cmd);
	if (it != _commands.end())
	{
		CmdFunc func = it->second;
		(this->*func)(sender, msg);
	}
	else
	{
		std::cout << "Unknown command : " << cmd << std::endl;
	}
}

void	Server::execNick(Client& sender, const Message& msg)
{
	std::cout << "[execNick] EXEC" << std::endl;
	if (msg.getParams().empty())
	{
		std::cout << "[execNick] Error : No nickname" << std::endl;
		//error "ERR_NONICKNAMEGIVEN (431)"
		return;
	}
	std::string oldNick = sender.getNickname();
	sender.setNickname(msg.getParams()[0]);
	std::cout << "[execNick] Nickname of " << oldNick << " change to : " << sender.getNickname() << std::endl;
}

void	Server::execJoin(Client& sender, const Message& msg)
{
	(void)sender; (void)msg;
std::cout << "[execJoin] try to join " << (msg.getParams().empty() ? "nothing" : msg.getParams()[0]) << std::endl;	// Create or join channel
}

void Server::execPrivmsg(Client& sender, const Message& msg)
{
	(void)sender; (void)msg;
	std::cout << "[execPrivmsg] send msg !" << std::endl;
}

void Server::execPass(Client& sender, const Message& msg)
{
	(void)sender; (void)msg;
	std::cout << "[execPass] mdp" << std::endl;
}
