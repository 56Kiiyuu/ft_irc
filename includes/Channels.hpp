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
	public:
		Channels(int ownerFd, std::string name);
		void joinChannels(int fd);
		void sendMsg(int msgFd, int serverFd, std::string msg);
		~Channels();
		std::string getName();
};

#endif
