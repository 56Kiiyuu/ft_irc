#ifndef CHANNELS_HPP
#define CHANNELS_HPP

#include <string>
#include <vector>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include <fcntl.h>

#include "Client.hpp"

class Channels
{
	private:
		std::vector<int> owner;
		std::vector<int> user;
		std::string name;
		std::vector<std::string> modes;
		bool needInvite;
		bool restrictedTopic;
		bool needPassword;
		std::string password;
		bool hasUserLimit;
		int limitUser;
	public:
		Channels(int ownerFd, std::string name);
		void joinChannels(int fd);
		void sendMsg(int msgFd, int serverFd, std::string msg);

		bool getNeedInvite();
		void setNeedInvite(bool _bool);
		bool getRestrictedTopic();
		void setRestrictedTopic(bool _bool);
		bool getNeedPassword();
		void setNeedPassword(bool _bool);
		void setPassword(std::string password);
		bool getHasUserLimit();
		void setHasUserLimit(bool _bool, int limitUser);

		void addNewOperator(int i);
		void deleteOperator(int i);

		std::vector<int> getUser();
		std::vector<int> getOwner();



		~Channels();
		std::string getName();
};

#endif
