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
	this->_run = 1;
}

Server::~Server()
{
	std::cout << "server destructor" << std::endl;
}

int&	Server::getRun()
{
	return _run;
}

std::vector<Channels>&	Server::getChannels()
{
	return channnels;
}

std::string	Server::rnl(std::string& buff)
{
	std::string::size_type pos = buff.find("\r\n");
	size_t delLen = 2;
	// si nc envoie que \n
	if (pos == std::string::npos)
	{
		pos = buff.find("\n");
		delLen = 1;
	}
	// si aucun saut de ligne (cmd incomplet)
	if (pos == std::string::npos)
		return "";
	// extraire la cmd
	std::string line = buff.substr(0, pos);
	buff.erase(0, pos + delLen);

	// suppr \r (^M) en fin de ligne
	if (!line.empty() && line[line.length() - 1] == '\r')
		line.erase(line.length() - 1);
	return line;
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
	std::cout << "[-] Client FD " << pollFd.fd << " deconnecte." << std::endl;
	close(pollFd.fd);
	return false;
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

void Server::handleCmds(std::string& buffClient, int socketFd)
{
	while (true)
	{
		std::string line = rnl(buffClient);
		// si plus de ligne avec \n ou \r\n = sort
		if (line.empty() && buffClient.find("\n") == std::string::npos)
			break;
		// si ligne vide = saute
		if (line.empty() || line.find_first_not_of(" \t\r\n") == std::string::npos)
			continue;

		Client::ClientInfo& sender = clients.getClientInfo()[socketFd];
		Message msg(line);
		_cmdManager.routeCommand(*this, sender, socketFd, msg);
	}
}

void Server::handlePoll()
{
	while (this->_run)
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
						clients.removeClient(clientFd);
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


Client&	Server::getClients()
{
	return this->clients;
}

bool Server::isNickInUse(const std::string& nick)
{
	std::map<int, Client::ClientInfo>& clientsMap = this->clients.getClientInfo();

	for (std::map<int, Client::ClientInfo>::iterator it = clientsMap.begin(); it != clientsMap.end(); ++it)
	{
		if (it->second.nickname == nick)
			return true;
	}
	return false;
}

const std::string& Server::getPassword() const
{
	return this->_password;
}

void Server::setPassword(const std::string& password)
{
	this->_password = password;
}

int Server::getServerSocket()
{
	return this->_socketServer;
}
