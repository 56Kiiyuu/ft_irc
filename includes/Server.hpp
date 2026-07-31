#ifndef SERVER_HPP
#define SERVER_HPP

#include <string>
#include <map>

#include "Command.hpp"
#include "Message.hpp"
#include "Client.hpp"

#include <arpa/inet.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include <fcntl.h>
#include <errno.h>

class Server
{
	private:
		// peut etre a supr (a voir par la suite)
		std::string _password;

		int _socketServer;
		int _socketClient;
		struct sockaddr_in _addrServer;
		struct sockaddr_in _addrClient;

		int _run;

		Client clients;
		CommandManager	_cmdManager;

		// utils
		std::string	rnl(std::string& buff);
		void 		handlePoll();
		void 		handleCon();
		bool 		readSocketFd(std::string& buff, struct pollfd& pollFd);
		void 		handleCmds(std::string& buffClient, int socketFd);
		// + plus tard;
	public:
		Server();
		~Server();

		void				startServer();
		Client&				getClients();
		bool				isNickInUse(const std::string& nick);
		const std::string&	getPassword() const;
		void				setPassword(const std::string& password);
		int&				getRun();
};

#endif
