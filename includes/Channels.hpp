#ifndef CHANNELS_HPP
#define CHANNELS_HPP

#include <string>
#include <vector>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <ctime>
#include <fcntl.h>

#include "Client.hpp"

class Channels
{
	private:
		std::vector<int> owner;
		std::vector<int> user;
		std::string name;
		std::string topic;
		std::vector<std::string> modes;
		bool needInvite;
		bool restrictedTopic;
		bool needPassword;
		std::string password;
		bool hasUserLimit;
		int limitUser;
		std::string topicSetter; // author
		time_t topicTime; // date de modif

	public:
		Channels(int ownerFd, std::string name);
		void sendMsg(int msgFd, int serverFd, std::string msg);

		bool getNeedInvite();
		void setNeedInvite(bool _bool);
		bool getRestrictedTopic();
		void setRestrictedTopic(bool _bool);
		bool getNeedPassword();
		void setNeedPassword(bool _bool);
		void setPassword(std::string password);
		std::string getPassword();
		int getLimitUser();
		bool getHasUserLimit();
		void setHasUserLimit(bool _bool, int limitUser);

		void addUser(int clientFd);
		void removeUser(int clientFd);
		void addNewOperator(int clientFd);
		void deleteOperator(int clientFd);

		std::vector<int>& getUser();
		std::vector<int>& getOwner();

		bool isOp(int fd);


		~Channels();
		std::string getName();
		std::string getTopic();

		const std::string& getTopicSetter() const;
		time_t getTopicTime();
		void setTopic(const std::string& newTopic, const std::string& setter);

		void kickUser(int index);
		void kickOwner(int index);

};

#endif
