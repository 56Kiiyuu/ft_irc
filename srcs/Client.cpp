#include "Client.hpp"

Client::Client()
{

}

Client::Client(int serverSocket, sockaddr_in addrServer)
{
	/* setup du server fd dans le pollfd (en cas de connexion il sera en pollin) */
	addNewClient(serverSocket, addrServer, "Server", "Server");
}

void Client::addNewClient(int fd, struct sockaddr_in addrClient, std::string nickname, std::string user)
{
	struct pollfd tmpFd;
	ClientInfo ci;

	tmpFd.fd = fd;
	tmpFd.events = POLLIN;

	ci.nickname = nickname;
	ci.user = user;
	ci._addrClient = addrClient;
	ci.addrClientSize = sizeof(addrClient);


	this->_fd.push_back(tmpFd);
	this->_clientInfo.insert(std::pair<int, ClientInfo>(fd, ci));
}

std::map<int, Client::ClientInfo>& Client::getClientInfo()
{
	return this->_clientInfo;
}


std::vector<struct pollfd>& Client::getPollFd()
{
	return this->_fd;
}
