#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <string>
#include <poll.h>
#include <vector>
#include <map>
#include <iostream>
#include <cstring>

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
			int					fd;
			std::string			nickname;
			std::string			user;
			std::string			realname;
			std::string			buff;
			struct sockaddr_in	_addrClient;
			socklen_t			addrClientSize;
			bool				isRegistered;
			bool				hasPass;

			ClientInfo()
				: fd(-1),
				nickname(""),
				user(""),
				realname(""),
				buff(""),
				addrClientSize(sizeof(_addrClient)),
				isRegistered(false),
				hasPass(false)
			{
				std::memset(&_addrClient, 0, sizeof(_addrClient));
			}
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
