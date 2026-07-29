#include "Channels.hpp"

Channels::Channels(int ownerFd)
{
	this->owner.push_back(ownerFd);
}

void Channels::joinChannels(int fd)
{
	this->user.push_back(fd);
}

void Channels::sendMsg(int msgFd, int serverFd, std::string msg)
{
	int i = 0;

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