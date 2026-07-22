#include "Client.hpp"

Client::Client(int serverSocket)
{
	/* setup du server fd dans le pollfd (en cas de connexion il sera en pollin) */
	addNewClient(serverSocket, "Server", "Server");
}

void Client::addNewClient(int fd, std::string nickname, std::string user)
{
	struct pollfd tmpFd;

	tmpFd.fd = fd;
	tmpFd.events = POLLIN;
	this->_fd.push_back(tmpFd);
	this->_nicknames.insert(std::pair<int, std::string>(fd, nickname));
	this->_users.insert(std::pair<int, std::string>(fd, user));
}

const std::map<int, std::string>& Client::getNickname() const
{
	return this->_nicknames;
}


const std::vector<struct pollfd>& Client::getPollFd() const
{
	return this->_fd;
}
