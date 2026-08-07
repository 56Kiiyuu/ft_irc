#include "Channels.hpp"

Channels::Channels(int ownerFd, std::string name)
{
	this->user.push_back(ownerFd);
	this->owner.push_back(ownerFd);
	this->name = name;
	this->topic = "";
	this->restrictedTopic = false;
	this->needInvite = false;
	this->needPassword = false;
	this->hasUserLimit = false;
	this->limitUser = 0;
}

void Channels::sendMsg(int msgFd, int serverFd, std::string msg)
{
	(void)serverFd;

	for (std::size_t i = 0; i < this->user.size(); i++)
	{
		if (msgFd >= 0 && this->user[i] == msgFd)
			continue;

		send(this->user[i], msg.c_str(), msg.length(), 0);
		std::cout << "[DEBUG] send msg to FD: " << this->user[i] << std::endl;
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

void Channels::addUser(int clientFd)
{
	for (std::size_t i = 0; i < this->user.size(); i++)
	{
		if (this->user[i] == clientFd)
			return;
	}
	this->user.push_back(clientFd);
}

void Channels::removeUser(int clientFd)
{
	for (std::vector<int>::iterator it = this->user.begin(); it != this->user.end(); ++it)
	{
		if (*it == clientFd)
		{
			this->user.erase(it);
			break;
		}
	}
	deleteOperator(clientFd);
}

void Channels::addNewOperator(int clientFd)
{
	// cherche le Fd correspondant dans le vecteur (si deja op)
	for (std::size_t i = 0; i < this->owner.size(); i++)
	{
		if (this->owner[i] == clientFd)
			return;
	}
	// sinon ajoute le dans la liste des op
	this->owner.push_back(clientFd);
}

void Channels::deleteOperator(int clientFd)
{
	// cherche le Fd correspondant dans le vecteur
	for (std::vector<int>::iterator it = this->owner.begin(); it != this->owner.end(); ++it)
	{
		if (*it == clientFd)
		{
			this->owner.erase(it);
			break; // supprime et stop
		}
	}
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
			return true;
	}
	return false;
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

void Channels::addInvite(int fd)
{
	this->invite.push_back(fd);
}

bool Channels::checkInvite(int fd)
{
	for (std::size_t i=0 ; i < this->invite.size() ; i++)
	{
		if (fd == this->invite[i])
		{
			this->invite.erase(this->invite.begin() + i);
			return true;
		}
	}
	return false;
}