#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <string>
#include <poll.h>
#include <vector>

//TEMPORAIRE
class Client
{
	private:
		std::string _nickname;
		std::string _user;
		std::vector<struct pollfd> _fd;
	public:
		Client(int serverSocket);
		~Client() {}

		void addNewClient(int fd, std::string nickname, std::string user);
		void setNickname(const std::string& nick);
		const std::string& getNickname() const;
		const std::vector<struct pollfd>& Client::getPollFd() const;
};

#endif
