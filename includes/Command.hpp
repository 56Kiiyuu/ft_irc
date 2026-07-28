#ifndef COMMAND_HPP
#define COMMAND_HPP

#include <string>
#include <map>

class Server;
class Client;
class Message;

typedef	void (*CmdHandler)(Server& server, Client& sender, const Message& msg);

class CommandManager
{
	private:
		std::map<std::string, CmdHandler>	_commands;
	public:
		CommandManager();
		~CommandManager();

		void	routeCommand(Server& server, Client& sender, const Message& msg);
};

void cmdPass(Server& server, Client& sender, const Message& msg);
void cmdNick(Server& server, Client& sender, const Message& msg);
void cmdUser(Server& server, Client& sender, const Message& msg);
void cmdJoin(Server& server, Client& sender, const Message& msg);
void cmdPrivmsg(Server& server, Client& sender, const Message& msg);

#endif
