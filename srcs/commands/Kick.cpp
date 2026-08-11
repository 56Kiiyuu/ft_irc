#include "Command.hpp"
#include "Server.hpp"
#include "Client.hpp"
#include "Message.hpp"
#include "Channels.hpp"
#include "NumericReplies.hpp"

void cmdKick(Server& server, Client::ClientInfo& sender, int socketFd, const Message& msg)
{
	std::cout << "KICK : args=" << msg.getParams().size() << std::endl;
	if (msg.getParams().size() < 2 || msg.getParams().size() > 3)
	{
		std::string err = ERR_NEEDMOREPARAMS(sender.nickname, "KICK");
		send(socketFd, err.c_str(), err.length(), 0);
		return ;
	}
	std::cout << "1 ERR_NEEDMOREPARAMS: Pass" << std::endl;
	std::string channel = msg.getParams()[0];
	std::string target = msg.getParams()[1];

	int indexChannel = -1;
	for (std::size_t i=0 ; i < server.getChannels().size() ; i++)
	{
		if (server.getChannels()[i].getName() == channel)
		{
			indexChannel = static_cast<int>(i);
			break ;
		}
	}

	if (indexChannel == -1)
	{
		std::string err = ERR_NOSUCHCHANNEL(sender.nickname, channel);
		send(socketFd, err.c_str(), err.length(), 0);
		return ;
	}
	std::cout << "2 ERR_NOSUCHCHANNEL: Pass" << std::endl;
	Channels& chan = server.getChannels()[indexChannel];

	// check que le user est dans le canal
	bool isMember = chan.isOp(socketFd);
	if (!isMember)
	{
		std::vector<int> users = chan.getUser();
		for (std::size_t i = 0; i < users.size(); i++)
		{
			if (users[i] == socketFd)
			{
				isMember = true;
				break;
			}
		}
	}
	if (!isMember)
	{
		send(socketFd, ERR_NOTONCHANNEL(sender.nickname, chan.getName()).c_str(), ERR_NOTONCHANNEL(sender.nickname, chan.getName()).length(), 0);
		return;
	}
	std::cout << "3 ERR_NOTONCHANNEL: Pass" << std::endl;

	// check que 'l'utilisateur est op
	if (!chan.isOp(sender.fd))
	{
		std::string err = ERR_CHANOPRIVSNEEDED(sender.nickname, channel);
		send(socketFd, err.c_str(), err.length(), 0);
		return;
	}
	std::cout << "4 ERR_CHANOPRIVSNEEDED: Pass" << std::endl;

	int userIndex = -1;
	int ownerIndex = -1;
	std::vector<int> user = chan.getUser();
	for (std::size_t j = 0; j < user.size(); j++)
	{
		if (server.getClients().getClientInfo()[user[j]].nickname == target)
		{
			userIndex = static_cast<int>(j);
			break;
		}
	}
	if (userIndex == -1)
	{
		std::vector<int> owner = chan.getOwner();
		for (std::size_t j = 0; j < owner.size(); j++)
		{
			if (server.getClients().getClientInfo()[owner[j]].nickname == target)
			{
				ownerIndex = static_cast<int>(j);
				break;
			}
		}
	}

	if (ownerIndex == -1 && userIndex == -1)
	{
		std::string err = ERR_USERNOTINCHANNEL(sender.nickname, channel);
		send(socketFd, err.c_str(), err.length(), 0);
		return;
	}
	std::cout << "5 ERR_USERNOTINCHANNEL: Pass" << std::endl;


	std::string prefix = ":" + sender.nickname + "!" + sender.user + "@127.0.0.1";
	std::string formattedMsg = prefix + " KICK";

	for (std::size_t i=0 ; i < msg.getParams().size() ; i++)
	{
		formattedMsg += " " + msg.getParams()[i];
	}
	formattedMsg += "\r\n";
	chan.sendMsg(socketFd, server.getServerSocket(), formattedMsg);
	send(socketFd, formattedMsg.c_str(), formattedMsg.length(), 0);
	std::cout << formattedMsg << std::endl;

	if (ownerIndex > -1)
		chan.kickOwner(ownerIndex);
	else
		chan.kickUser(userIndex);
}
