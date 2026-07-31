#include "Client.hpp"
#include <unistd.h>



Client::Client()
{

}

Client::Client(int serverSocket, sockaddr_in addrServer)
{
	/* setup du server fd dans le pollfd (en cas de connexion il sera en pollin) */
	addNewClient(serverSocket, addrServer);
}

Client::~Client()
{
	std::size_t i = 0;

	std::cout << "Client destructor" << std::endl;

	while (i < this->_fd.size())
	{
		close(this->_fd[i].fd);
		i++;
	}
}

void Client::addNewClient(int fd, struct sockaddr_in addrClient)
{
	struct pollfd tmpFd;
	ClientInfo ci;

	tmpFd.fd = fd;
	tmpFd.revents = 0;
	tmpFd.events = POLLIN;

	ci.fd = fd;
	ci.nickname = "";
	ci.user = "";
	ci.realname = "";
	ci.buff = "";
	ci.isRegistered = false;
	ci._addrClient = addrClient;
	ci.addrClientSize = sizeof(addrClient);


	this->_fd.push_back(tmpFd);
	this->_clientInfo[fd] = ci;
}

void Client::removeClient(int fd)
{
	//Suppression de la map clientInfo
	this->_clientInfo.erase(fd);

	//Suppression du vector pollfd
	for (std::vector<struct pollfd>::iterator it = this->_fd.begin(); it != this->_fd.end(); ++it)
	{
		if (it->fd == fd)
		{
			this->_fd.erase(it);
			return;
		}
	}
}

std::map<int, Client::ClientInfo>& Client::getClientInfo()
{
	return this->_clientInfo;
}


std::vector<struct pollfd>& Client::getPollFd()
{
	return this->_fd;
}
