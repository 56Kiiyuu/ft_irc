#include "Server.hpp"
#include "Client.hpp"
#include <iostream>
#include <unistd.h>

#include <stdio.h>

Server::Server()
{
	initCommands();

	this->_socketServer = socket(AF_INET, SOCK_STREAM, 0);
	this->_addrServer.sin_addr.s_addr = inet_addr("127.0.0.1");
	this->_addrServer.sin_family = AF_INET;
	this->_addrServer.sin_port = htons(6667);
}

Server::~Server()
{
}

std::string	Server::rnl(int socketClient)
{
	static std::string buff;
	char tmpBuffChar[512];

	int n;

	while (1)
	{
		std::string::size_type pos = buff.find("\r\n");
		if (pos != std::string::npos)
		{
			std::string retStr = buff.substr(0, pos);
			buff.erase(0, pos + 2);
			return retStr;
		}

		n = recv(socketClient, tmpBuffChar, sizeof(tmpBuffChar) - 1, 0);
		if (n <= 0)
			break;

		buff.append(tmpBuffChar, n);
	}
	return "";
}

void Server::startServer()
{
	bind(this->_socketServer, (const struct sockaddr *)&this->_addrServer, sizeof(this->_addrServer));
	std::cout << "Bind Server" << std::endl;

	listen(this->_socketServer, SOMAXCONN);
	std::cout << "Server listening" << std::endl;

	socklen_t addrClientSize = sizeof(this->_addrClient);
	this->_socketClient = accept(this->_socketServer, (struct sockaddr *)&this->_addrClient, &addrClientSize);
	sleep(5);
	while (1)
	{

		std::string line = rnl(this->_socketClient);

		std::cout << "Client: " << line << std::endl;
		if (line == "CAP LS 302")
		{
			char msgserv[22] = ":server CAP * LS :\r\n";
			send(this->_socketClient, msgserv, sizeof(msgserv), 0);
			std::cout << "Server: " << msgserv << std::endl;
		}
		else if (line == "JOIN :")
		{
			char msgserv1[40] = ":server 461 * :Not enough parameters\r\n";
			send(this->_socketClient, msgserv1, sizeof(msgserv1), 0);
			std::cout << "Server: " << msgserv1 << std::endl;
		}
	}
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
