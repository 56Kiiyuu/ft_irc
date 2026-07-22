#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <string>
#include <poll.h>

//TEMPORAIRE
class Client
{
	private:
		std::string _nickname;
		struct pollfd _fd;
	public:
		Client();
		Client(std::string nick, int socketFd);
		~Client() {}

		void setNickname(const std::string& nick);
		const std::string& getNickname() const;
		const struct pollfd& getPollFd() const;
};

#endif
