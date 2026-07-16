#ifndef SERVER_HPP
#define SERVER_HPP

#include <string>
#include <map>
#include "Message.hpp"

class Client;

class Server
{
	private:
		//def type de la fonction pointeur
		typedef void (Server::*CmdFunc)(Client& sender, const Message& msg);

		std::map<std::string, CmdFunc> _commands; // table de fonction (routage)

		void	execNick(Client& sender, const Message& msg);
		void	execJoin(Client& sender, const Message& msg);
		void	execPrivmsg(Client& sender, const Message& msg);
		void	execPass(Client& sender, const Message& msg);
		// + plus tard;
	public:
		Server();
		~Server();

		void	initCommands();
		void	routeCommand(Client& sender, const Message& msg);
};

#endif
