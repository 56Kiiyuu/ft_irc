#ifndef SERVER_HPP
#define SERVER_HPP

#include <string>
#include <map>

#include "Message.hpp"
#include "Client.hpp"

#include <arpa/inet.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include <fcntl.h>
#include <errno.h>

class Client;

class Server
{
	private:
		// peut etre a supr (a voir par la suite)
		int _socketServer;
		int _socketClient;
		struct sockaddr_in _addrServer;
		struct sockaddr_in _addrClient;

		Client clients;


		//def type de la fonction pointeur
		typedef void (Server::*CmdFunc)(Client& sender, const Message& msg);

		std::map<std::string, CmdFunc> _commands; // table de fonction (routage)
		/*void	execNick(Client& sender, const Message& msg);
		void	execJoin(Client& sender, const Message& msg);
		void	execPrivmsg(Client& sender, const Message& msg);
		void	execPass(Client& sender, const Message& msg);*/

		// utils
		std::string	rnl(std::string& buff);
		void handlePoll();
		void handleCon();
		void readSocketFd(std::string& buff, struct pollfd pollFd);
		void handleCmds(std::string& buffClient, int socketFd);

		// + plus tard;
	public:
		Server();
		~Server();

		void	startServer();
		/*void	initCommands();
		void	routeCommand(Client& sender, const Message& msg);*/
};

#endif
