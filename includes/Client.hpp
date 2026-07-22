#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <string>
#include <poll.h>
#include <vector>
#include <map>

#include <netdb.h>

//TEMPORAIRE
class Client
{
	private:
		typedef struct ClientInfo
		{
			std::string nickname;
			std::string user;
			struct sockaddr_in _addrClient;
			socklen_t addrClientSize;
		} ClientInfo;


		std::vector<struct pollfd> _fd;
		std::map<int, ClientInfo> _clientInfo;

	public:
		Client();
		Client(int serverSocket, sockaddr_in addrServer);
		~Client() {}

		void addNewClient(int fd, struct sockaddr_in addrClient, std::string nickname, std::string user);
		std::map<int, ClientInfo>& getClientInfo();
		std::vector<struct pollfd>& getPollFd();
};

#endif
