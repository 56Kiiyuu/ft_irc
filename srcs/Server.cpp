#include "Server.hpp"
#include "Client.hpp"
#include <iostream>
#include <unistd.h>

#include <stdio.h>

Server::Server() : clients()
{
	// initCommands();

	this->_socketServer = socket(AF_INET, SOCK_STREAM, 0);
	this->_addrServer.sin_addr.s_addr = inet_addr("127.0.0.1");
	this->_addrServer.sin_family = AF_INET;
	this->_addrServer.sin_port = htons(6667);

	clients.addNewClient(this->_socketServer, this->_addrServer, "Server", "Server");
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
	if (bind(this->_socketServer, (const struct sockaddr *)&this->_addrServer, sizeof(this->_addrServer)) == -1)
	{
		perror("bind: ");
		return ;
	}
	std::cout << "Bind Server" << std::endl;

	if (listen(this->_socketServer, SOMAXCONN) == -1)
	{
		std::cout << "Error: bind" << std::endl;
		return ;
	}
	std::cout << "Server listening" << std::endl;

	while (1)
	{

		std::vector<struct pollfd> pollFd = clients.getPollFd();

		std::cout << "Wait poll" << std::endl;
		int nbEvent = poll(pollFd.data(), pollFd.size(), -1);

		if (nbEvent <= 0)
		{
			std::cout << "Error with poll" << std::endl;
			return ;
		}

		int actions = 0;
		while (actions < nbEvent)
		{
			// std::cout << "Poll detect a action" << std::endl;

			if (pollFd[0].revents & POLLIN)
			{
				std::cout << "The action is pollin with socket server" << std::endl;

				socklen_t addrClientSize = sizeof(this->_addrClient);
				this->_socketClient = accept(this->_socketServer, (struct sockaddr *)&this->_addrClient, &addrClientSize);
				std::cout << "Accept the client" << std::endl;


				// CAP test
				std::string line = rnl(this->_socketClient);

				while (line.empty() == 0)
				{
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
					else if (line == "USER gchalmel gchalmel 127.0.0.1 :gchalmel")
					{
						char msgserv2[53] = "001 dan :Welcome to our IRC Server\r\n";
						send(this->_socketClient, msgserv2, sizeof(msgserv2), 0);
						std::cout << "Server: " << msgserv2 << std::endl;
					}

					line = rnl(this->_socketClient);
				}



				std::cout << "add new client" << std::endl;
				clients.addNewClient(this->_socketClient, this->_addrClient, "test", "test");
			}
			actions++;
		}
	}
}

/*void	Server::initCommands()
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
}*/

/*void	Server::execNick(Client& sender, const Message& msg)
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
}*/
