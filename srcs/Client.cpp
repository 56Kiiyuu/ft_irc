#include "Client.hpp"

Client::Client(int serverSocket)
{
	/* setup du server fd dans le pollfd (en cas de connexion il sera en pollin) */
	struct pollfd tmpFd;

	tmpFd.fd = serverSocket;
	tmpFd.events = POLLIN;
	_fd.push_back({serverSocket, });
}

void Client::setNickname(const std::string& nick)
{
	_nickname = nick;
}

const std::string& Client::getNickname() const
{
	return _nickname;
}


const std::vector<struct pollfd>& Client::getPollFd() const
{
	return this->_fd;
}
