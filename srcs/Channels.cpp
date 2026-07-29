#include "Channels.hpp"

Channels::Channels(int ownerFd, std::string name)
{
	this->owner.push_back(ownerFd);
	this->name = name;
}

void Channels::joinChannels(int fd)
{
	this->user.push_back(fd);
}

void Channels::sendMsg(int msgFd, int serverFd, std::string msg)
{
	std::size_t i = 0;

	while (i < owner.size())
	{
		if (msgFd == owner[i])
		{
			i++;
			continue;
		}
		send(serverFd, msg.c_str(), sizeof(msg.c_str()), 0);
	}

	i = 0;
	while (i < user.size())
	{
		if (msgFd == user[i])
		{
			i++;
			continue;
		}
		send(serverFd, msg.c_str(), sizeof(msg.c_str()), 0);
	}
}

Channels::~Channels()
{

}

std::string Channels::getName()
{
	return this->name;
}