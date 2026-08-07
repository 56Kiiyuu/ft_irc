#include "Channels.hpp"

Channels::Channels(int ownerFd, std::string name)
{
	this->owner.push_back(ownerFd);
	this->name = name;
	this->topic = "";
	this->restrictedTopic = false;
	this->needInvite = false;
	this->needPassword = false;
	this->hasUserLimit = false;
	this->limitUser = 0;
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

std::string Channels::getTopic()
{
	return this->topic;
}

void Channels::setTopic(const std::string& newTopic, const std::string& setter)
{
	topic = newTopic;
	topicSetter = setter;
	topicTime = std::time(NULL);
}

bool Channels::getNeedInvite()
{
	return this->needInvite;
}

void Channels::setNeedInvite(bool _bool)
{
	this->needInvite = _bool;
}

bool Channels::getRestrictedTopic()
{
	return this->restrictedTopic;
}

void Channels::setRestrictedTopic(bool _bool)
{
	this->restrictedTopic = _bool;
}

bool Channels::getNeedPassword()
{
	return this->needPassword;
}
void Channels::setNeedPassword(bool _bool)
{
	this->needPassword = _bool;
}

void Channels::setPassword(std::string password)
{
	this->password = password;
}

std::vector<int>& Channels::getUser()
{
	return this->user;
}

std::vector<int>& Channels::getOwner()
{
	return this->owner;
}

void Channels::addNewOperator(int i)
{
	this->owner.push_back(this->user[i]);
	this->user.erase (this->user.begin()+i);
}

void Channels::deleteOperator(int i)
{
	this->user.push_back(this->owner[i]);
	this->owner.erase (this->owner.begin()+i);
}

bool Channels::getHasUserLimit()
{
	return this->hasUserLimit;
}

void Channels::setHasUserLimit(bool _bool, int limitUser)
{
	this->hasUserLimit = _bool;
	this->limitUser = limitUser;
}

bool Channels::isOp(int fd)
{
	for (std::size_t i=0 ; i < this->owner.size() ; i++)
	{
		if (fd == this->owner[i])
			return 1;
	}
	return 0;
}

std::string Channels::getPassword()
{
	return this->password;
}

int Channels::getLimitUser()
{
	return this->limitUser;
}
const std::string& Channels::getTopicSetter() const
{
	return this->topicSetter;
}

time_t Channels::getTopicTime()
{
	return this->topicTime;
}

void Channels::kickUser(int index)
{
	this->user.erase(this->user.begin() + index);
}

void Channels::kickOwner(int index)
{
	this->owner.erase(this->owner.begin() + index);
}