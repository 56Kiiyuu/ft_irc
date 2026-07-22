#include "Client.hpp"

Client::Client() : _nickname("Guest")
{

}

Client::Client(std::string nick, int socketFd) : _nickname(nick)
{
	_fd.fd = socketFd;
	_fd.events = POLLIN;
}

void Client::setNickname(const std::string& nick)
{
	_nickname = nick;
}

const std::string& Client::getNickname() const
{
	return _nickname;
}


const struct pollfd& Client::getPollFd() const
{
	return this->_fd;
}
