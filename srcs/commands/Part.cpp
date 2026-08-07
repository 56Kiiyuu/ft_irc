#include "Command.hpp"
#include "Server.hpp"
#include "Client.hpp"
#include "Message.hpp"
#include "Channels.hpp"
#include "NumericReplies.hpp"
#include <iostream>
#include <sys/socket.h>

void cmdPart(Server& server, Client::ClientInfo& sender, int socketFd, const Message& msg)
{
	//verif register
	if (!sender.isRegistered)
	{
		std::string clientNick = sender.nickname.empty() ? "*" : sender.nickname;
		std::string err = ERR_NOTREGISTERED(clientNick);
		send(socketFd, err.c_str(), err.length(), 0);
		return;
	}

	if (msg.getParams().empty() || msg.getParams()[0].empty())
	{
		std::string err = ERR_NEEDMOREPARAMS(sender.nickname, "PART");
		send(socketFd, err.c_str(), err.length(), 0);
		return;
	}

	std::vector<std::string> chanList = splitString(msg.getParams()[0], ',');
	std::string reason = (msg.getParams().size() >= 2) ? msg.getParams()[1] : "";

	std::vector<Channels>& channels = server.getChannels();

	for (std::size_t cIdx = 0; cIdx < chanList.size(); cIdx++)
	{
		std::string chanName = chanList[cIdx];
		int chanIdx = -1;

		for (std::size_t i = 0; i < channels.size(); i++)
		{
			if (channels[i].getName() == chanName)
			{
				chanIdx = static_cast<int>(i);
				break;
			}
		}

		// si existe pas
		if (chanIdx == -1)
		{
			std::string err = ERR_NOSUCHCHANNEL(sender.nickname, chanName);
			send(socketFd, err.c_str(), err.length(), 0);
			continue;
		}

		Channels& chan = channels[chanIdx];
		//si membre du channel

		bool isMember = false;
		std::vector<int> currentUsers = chan.getUser();
		for (std::size_t i = 0; i < currentUsers.size(); i++)
		{
			if (currentUsers[i] == socketFd)
			{
				isMember = true;
				break;
			}
		}

		if (!isMember)
		{
			std::string err = ERR_NOTONCHANNEL(sender.nickname, chanName);
			send(socketFd, err.c_str(), err.length(), 0);
			continue;
		}

		std::string prefix = ":" + sender.nickname + "!" + sender.user + "@127.0.0.1";
		std::string partMsg = prefix + " PART " + chanName;
		if (!reason.empty())
			partMsg += " :" + reason;
		partMsg += "\r\n";

		chan.sendMsg(-1, server.getServerSocket(), partMsg);

		chan.removeUser(socketFd); //supprimer de user ET de owner

		//suppr le channel si plus personne
		if (chan.getUser().empty())
		{
			channels.erase(channels.begin() + chanIdx);
		}
	}
}
