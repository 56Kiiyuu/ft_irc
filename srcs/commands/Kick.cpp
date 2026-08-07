#include "Command.hpp"
#include "Server.hpp"
#include "Client.hpp"
#include "Message.hpp"
#include "Channels.hpp"
#include "NumericReplies.hpp"

void cmdKick(Server& server, Client::ClientInfo& sender, int socketFd, const Message& msg)
{


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


	Channels& chan = server.getChannels()[indexChannel];

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
		// ERR
		return ;
	}

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