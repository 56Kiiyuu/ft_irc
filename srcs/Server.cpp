#include "Server.hpp"
#include "Client.hpp"
#include "NumericReplies.hpp"
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

	clients.addNewClient(this->_socketServer, this->_addrServer);
}

Server::~Server()
{
}

std::string	Server::rnl(std::string& buff)
{
	std::string::size_type pos = buff.find("\r\n");
	if (pos != std::string::npos)
	{
		std::string retStr = buff.substr(0, pos);
		buff.erase(0, pos + 2);
		return retStr;
	}
	return "";
}

bool Server::readSocketFd(std::string& buff, struct pollfd& pollFd)
{
	char tmpBuffChar[512];
	int n = recv(pollFd.fd, tmpBuffChar, sizeof(tmpBuffChar) - 1, 0);

	if (n > 0)
	{
		tmpBuffChar[n] = '\0';
		buff.append(tmpBuffChar, n);
		return true; //donnees recu succes
	}
	else if (n == 0 || (n < 0 && errno != EAGAIN && errno != EWOULDBLOCK))
	{
		std::cout << "[-] Client FD " << pollFd.fd << " deconnecte." << std::endl;

		close(pollFd.fd);
		clients.removeClient(pollFd.fd);
		return false; // Le client est deco
	}
	return true;
}

void Server::handleCon()
{
	std::cout << "The action is pollin with socket server" << std::endl;

	socklen_t addrClientSize = sizeof(this->_addrClient);
	this->_socketClient = accept(this->_socketServer, (struct sockaddr *)&this->_addrClient, &addrClientSize);
	fcntl(this->_socketClient, F_SETFL, O_NONBLOCK);
	this->clients.addNewClient(this->_socketClient, this->_addrClient);
	std::cout << "Accept & Add new client FD " << this->_socketClient << std::endl;
}

void Server::handleCmds(std::string& buffClient, int socketFd, std::string& nick, std::string& user)
{
	std::string line = rnl(buffClient);
	while (!line.empty())
	{
		Message msg(line);
		Client::ClientInfo& sender = clients.getClientInfo()[socketFd];
		_cmdManager.routeCommand(*this, sender, socketFd, msg);

		line = rnl(buffClient);
	}
}

void Server::handlePoll()
{
	while (1)
	{
		std::vector<struct pollfd>& pollFd = this->clients.getPollFd();

		std::cout << "Wait poll" << std::endl;
		int nbEvent = poll(pollFd.data(), pollFd.size(), -1);

		if (nbEvent <= 0)
		{
			std::cout << "Error with poll" << std::endl;
			return;
		}

		for (size_t i = 0; i < pollFd.size(); ++i)
		{
			if (pollFd[i].revents & POLLIN)
			{
				// CAS 1 : Nouveaute sur le socket serveur -> accept()
				if (pollFd[i].fd == this->_socketServer)
				{
					handleCon();
				}
				// CAS 2 : Nouveaute sur un socket client -> recv()
				else
				{
					int clientFd = pollFd[i].fd;
					std::string& buffClient = clients.getClientInfo()[clientFd].buff;

					// Si readSocketFd renvoie true (client toujours actif)
					if (readSocketFd(buffClient, pollFd[i]))
					{
						handleCmds(buffClient, clientFd);
					}
					else
					{
						// Le client suppr du vector dans removeClient !
						// On decremente i pour ne pas sauter le client suivant dans la boucle for
						--i;
					}
				}
			}
		}
	}
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
	handlePoll();
}

Client& Server::getClients()
{
	return this->clients;
}
