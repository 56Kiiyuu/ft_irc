#ifndef SERVER_HPP
#define SERVER_HPP

#include <string>
#include <map>

#include "Command.hpp"
#include "Message.hpp"
#include "Client.hpp"
#include "Channels.hpp"

#include <arpa/inet.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include <fcntl.h>
#include <errno.h>

#include <Channels.hpp>

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

		std::vector<Channels> channnels;

		// utils
		std::string				rnl(std::string& buff);
		void 					handlePoll();
		void 					handleCon();
		bool 					readSocketFd(std::string& buff, struct pollfd& pollFd);
		void 					handleCmds(std::string& buffClient, int socketFd);
		// + plus tard;
	public:
		Server(int port, const std::string& password);
		~Server();

		void					startServer();
		Client&					getClients();
		bool					isNickInUse(const std::string& nick);
		const std::string&		getPassword() const;
		std::vector<Channels>&	getChannels();
		void					setPassword(const std::string& password);
		int&					getRun();
		int						getServerSocket();
		void					disconnectClient(int clientFd, const std::string& quitReason);
};

#endif
