#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <string>
#include <poll.h>
#include <vector>
#include <map>
#include <iostream>

#include <netdb.h>

//TEMPORAIRE
class Client
{
	public:
		Client();
		Client(int serverSocket, sockaddr_in addrServer);
		~Client();

		typedef struct ClientInfo
		{
			std::string			nickname;
			std::string			user;
			std::string			realname;
			struct sockaddr_in	_addrClient;
			socklen_t			addrClientSize;
			std::string			buff;
			bool				isRegistered;
		} ClientInfo;

		void addNewClient(int fd, struct sockaddr_in addrClient);
		void removeClient(int fd);
		std::map<int, ClientInfo>& getClientInfo();
		std::vector<struct pollfd>& getPollFd();

	private:
		std::vector<struct pollfd> _fd;
		std::map<int, ClientInfo> _clientInfo;

};

#endif
