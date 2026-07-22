#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <string>
#include <poll.h>
#include <vector>
#include <map>

//TEMPORAIRE
class Client
{
	private:
		std::vector<struct pollfd> _fd;

		std::map<int, std::string> _nicknames;
		std::map<int, std::string> _users;


	public:
		Client(int serverSocket);
		~Client() {}

		void addNewClient(int fd, std::string nickname, std::string user);
		void setNickname(const std::string& nick);
		const std::map<int, std::string>& Client::getNickname() const;
		const std::vector<struct pollfd>& Client::getPollFd() const;
};

#endif
