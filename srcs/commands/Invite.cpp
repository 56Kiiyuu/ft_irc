#include "Command.hpp"
#include "Server.hpp"
#include "Client.hpp"
#include "Message.hpp"
#include "Channels.hpp"
#include "NumericReplies.hpp"

void cmdInvite(Server& server, Client::ClientInfo& sender, int socketFd, const Message& msg)
{
	if (msg.getParams().size() != 2)
	{
		std::string err = ERR_NEEDMOREPARAMS(sender.nickname, "INVITE");
		send(socketFd, err.c_str(), err.length(), 0);
		return ;
	}

	std::string target = msg.getParams()[0];
	std::string channel = msg.getParams()[1];

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


	// check que l'utilisateur est op
	if (!chan.isOp(sender.fd))
	{
		std::string err = ERR_CHANOPRIVSNEEDED(sender.nickname, channel);
		send(socketFd, err.c_str(), err.length(), 0);
		return;
	}

	// cherche l'utilisateur a inviter sur le server
	std::map<int, Client::ClientInfo> users = server.getClients().getClientInfo();
	std::map<int, Client::ClientInfo>::iterator it = users.begin();

	while (it != users.end())
	{
		if (it->second.nickname == target)
			break;
		it++;
	}

	if (it == users.end())
	{
		std::string err = ERR_NOSUCHNICK(sender.nickname, target);
		send(socketFd, err.c_str(), err.length(), 0);
		return;
	}

	// Verifie que le user invite n'est pas dans le channel
	bool userAlreadyInChannel = false;
	for (std::size_t i=0 ; i < chan.getOwner().size() ; i++)
	{
		if (chan.getOwner()[i] == it->first)
			userAlreadyInChannel = true;
	}
	for (std::size_t i=0 ; i < chan.getUser().size() ; i++)
	{
		if (chan.getUser()[i] == it->first)
			userAlreadyInChannel = true;
	}
	
	if (userAlreadyInChannel)
	{
		std::string err = ERR_USERALREADYINCHANNEL(sender.nickname, target, channel);
		send(socketFd, err.c_str(), err.length(), 0);
		return;
	}

	chan.addInvite(it->first);

	std::string senderConfirm = RPL_INVITE(sender.nickname, it->second.nickname, channel);
	std::string prefix = ":" + sender.nickname + "!" + sender.user + "@127.0.0.1";
	std::string inviteMsg = prefix + " INVITE " + it->second.nickname + " :" + channel + "\r\n";
	send(socketFd, senderConfirm.c_str(), senderConfirm.length(), 0);
	send(it->first, inviteMsg.c_str(), inviteMsg.size(), 0);
}