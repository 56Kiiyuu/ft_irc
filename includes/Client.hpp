#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <string>

//TEMPORAIRE
class Client
{
	private:
		std::string _nickname;

	public:
		Client() : _nickname("Guest") {}
		~Client() {}

		void setNickname(const std::string& nick) { _nickname = nick; }
		const std::string& getNickname() const { return _nickname; }
};

#endif
