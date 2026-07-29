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
	(void)serverFd;
	std::size_t i = 0;

	while (i < owner.size())
	{
		if (msgFd == owner[i])
		{
			i++;
			continue;
		}
		send(owner[i], msg.c_str(), msg.length(), 0);
		std::cout << "send msg" << std::endl;
		i++;
	}

	i = 0;
	while (i < user.size())
	{
		if (msgFd == user[i])
		{
			i++;
			continue;
		}
		send(user[i], msg.c_str(), msg.length(), 0);
		std::cout << "send msg" << std::endl;
		i++;
	}
}

Channels::~Channels()
{

}

std::string Channels::getName()
{
	return this->name;
}