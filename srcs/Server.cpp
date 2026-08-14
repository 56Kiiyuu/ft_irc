#include "Server.hpp"
#include "Client.hpp"
#include "Channels.hpp"
#include "NumericReplies.hpp"
#include <iostream>
#include <unistd.h>
#include <stdexcept>
#include <set>
#include <stdio.h>

Server::Server(int port, const std::string& password) : _password(password), _run(1), clients()
{
	this->_socketServer = socket(AF_INET, SOCK_STREAM, 0);
	if (this->_socketServer < 0)
	{
		throw std::runtime_error("Error: socket server creation failed");
	}
	if (fcntl(this->_socketServer, F_SETFL, O_NONBLOCK) < 0)
	{
		close(this->_socketServer);
		throw std::runtime_error("Error: fcntl server non-blocking failed");
	}

	int opt = 1;
	if (setsockopt(this->_socketServer, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
	{
		std::cerr << "Error: setsockopt SO_REUSEADDR failed (errno: " << errno << ")" << std::endl;
		close(this->_socketServer);
		throw std::runtime_error("Error: setsockopt failed");
	}

	this->_addrServer.sin_addr.s_addr = INADDR_ANY;
	this->_addrServer.sin_family = AF_INET;
	this->_addrServer.sin_port = htons(port);

	clients.addNewClient(this->_socketServer, this->_addrServer);
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
	if (n < 0)
		std::cerr << "[-] Erreur recv sur le client FD " << pollFd.fd << std::endl;
	else
		std::cout << "[-] Client FD " << pollFd.fd << " deconnecte." << std::endl;
	return false;
}

void Server::handleCon()
{
	std::cout << "The action is pollin with socket server" << std::endl;

	socklen_t addrClientSize = sizeof(this->_addrClient);
	this->_socketClient = accept(this->_socketServer, (struct sockaddr *)&this->_addrClient, &addrClientSize);
	if (this->_socketClient < 0)
	{
		std::cerr << "[-] Error accept client" << std::endl;
		return;
	}
	fcntl(this->_socketClient, F_SETFL, O_NONBLOCK);
	this->clients.addNewClient(this->_socketClient, this->_addrClient);
	std::cout << "Accept & Add new client FD " << this->_socketClient << std::endl;
}

void Server::handleCmds(std::string& buffClient, int socketFd)
{
	std::string line = rnl(buffClient);

	while (!line.empty())
	{
		if (this->clients.getClientInfo().find(socketFd) == this->clients.getClientInfo().end())
		{
			buffClient.clear();
			return;
		}
		// si ligne vide = saute
		if (line.find_first_not_of(" \t\r\n") != std::string::npos)
		{
			Client::ClientInfo& sender = clients.getClientInfo()[socketFd];
			Message msg(line);
			_cmdManager.routeCommand(*this, sender, socketFd, msg);
			if (this->clients.getClientInfo().find(socketFd) == this->clients.getClientInfo().end())
				return;
		}
		if (this->clients.getClientInfo().find(socketFd) == this->clients.getClientInfo().end())
		{
			buffClient.clear();
			return;
		}
		line = rnl(buffClient);
	}
}

void Server::handlePoll()
{
	while (this->_run)
	{
		std::vector<struct pollfd>& pollFd = this->clients.getPollFd();

		std::cout << "Wait poll" << std::endl;
		int nbEvent = poll(pollFd.data(), pollFd.size(), -1);

		if (nbEvent < 0)
		{
			if (errno == EINTR) //sigint
			{
				if (!this->_run)
					return;
				continue;
			}
			std::cout << "Error with poll (errno: " << errno << ")" << std::endl;
			return;
		}

		size_t currentSize = pollFd.size();
		for (size_t i = 0; i < currentSize; ++i)
		{
			if (pollFd[i].revents == 0)
				continue;

			// CAS 1 : Nouveau client sur le socket serveur
			if (pollFd[i].fd == this->_socketServer)
			{
				if (pollFd[i].revents & POLLIN)
				{
					handleCon();
					break;
				}
			}
			// CAS 2 : Activite ou deco sur un socket client
			else
			{
				int clientFd = pollFd[i].fd;

				// Si le socket signale une deconnexion ou une erreur directe
				if (pollFd[i].revents & (POLLHUP | POLLERR | POLLNVAL))
				{
					this->disconnectClient(clientFd, "Connection closed");
					break;
				}

				// le socket lis des donnees
				if (pollFd[i].revents & POLLIN)
				{
					if (this->clients.getClientInfo().find(clientFd) == this->clients.getClientInfo().end())
						continue;

					std::string& buffClient = this->clients.getClientInfo()[clientFd].buff;

					if (readSocketFd(buffClient, pollFd[i]))
					{
						handleCmds(buffClient, clientFd);
						if (this->clients.getClientInfo().find(clientFd) == this->clients.getClientInfo().end())
						{
							break;
						}
					}
					else
					{
						// si erreur de lecture ou false
						this->disconnectClient(clientFd, "Connection closed by client");
						break;
					}
				}
			}
		}
	}
	// close(this->_socketServer);
}

void Server::startServer()
{
	if (bind(this->_socketServer, (const struct sockaddr *)&this->_addrServer, sizeof(this->_addrServer)) == -1)
	{
		throw std::runtime_error(std::string("Error: bind failed: ") + strerror(errno));		return ;
	}
	std::cout << "Bind Server" << std::endl;

	if (listen(this->_socketServer, SOMAXCONN) == -1)
	{
		throw std::runtime_error(std::string("Error: listen failed: ") + strerror(errno));
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

//fonction pour deco les client
void Server::disconnectClient(int clientFd, const std::string& quitReason)
{
	//secu
	std::map<int, Client::ClientInfo>& clientMap = this->clients.getClientInfo();
	std::map<int, Client::ClientInfo>::iterator clientIt = clientMap.find(clientFd);

	if (clientIt == clientMap.end())
		return;

	Client::ClientInfo sender = clientIt->second;
	//msg
	std::string reason = quitReason.empty() ? "Client quit" : quitReason;
	std::string prefix = ":" + sender.nickname + "!" + sender.user + "@127.0.0.1";
	std::string quitMsg = prefix + " QUIT :" + reason + "\r\n";

	std::vector<Channels>& channels = this->getChannels();
	std::set<int> recipients;
	//clear client de tous les channels
	for (std::size_t i = 0; i < channels.size(); i++)
	{
		std::vector<int>& users = channels[i].getUser();
		bool wasInChan = false;
		for (std::size_t u = 0; u < users.size(); u++)
		{
			if (users[u] == clientFd)
			{
				wasInChan = true;
				break;
			}
		}

		if (wasInChan)
		{
			for (std::size_t u = 0; u < users.size(); u++)
			{
				if (users[u] != clientFd)
					recipients.insert(users[u]);
			}
			channels[i].removeUser(clientFd);
		}
	}

	//envoie QUIT une seule fois a chaque client
	for (std::set<int>::iterator it = recipients.begin(); it != recipients.end(); ++it)
	{
		send(*it, quitMsg.c_str(), quitMsg.length(), 0);
	}

	//retirer client des channels et suppr si vides
	for (int i = static_cast<int>(channels.size()) - 1; i >= 0; --i)
	{
		if (channels[i].getUser().empty())
		{
			channels.erase(channels.begin() + i);
		}
	}

	//rm client et close son socket
	close(clientFd);
	this->clients.removeClient(clientFd);
}
