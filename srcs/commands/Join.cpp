/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kevlim <kevlim@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/29 13:16:57 by kevlim            #+#    #+#             */
/*   Updated: 2026/08/07 14:56:01 by kevlim           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"
#include "Server.hpp"
#include "Client.hpp"
#include "Message.hpp"
#include "Channels.hpp"
#include "NumericReplies.hpp"
#include <iostream>
#include <sys/socket.h>

void cmdJoin(Server& server, Client::ClientInfo& sender, int socketFd, const Message& msg)
{
	if (!sender.isRegistered)
	{
		std::string clientNick = sender.nickname.empty() ? "*" : sender.nickname;
		std::string err = ERR_NOTREGISTERED(clientNick);
		send(socketFd, err.c_str(), err.length(), 0);
		return;
	}

	if (msg.getParams().empty() || msg.getParams()[0].empty())
	{
		std::string err = ERR_NEEDMOREPARAMS(sender.nickname, "JOIN");
		send(socketFd, err.c_str(), err.length(), 0);
		return;
	}

	// si JOIN 0 = leave tout channel
	//TODO appliquer la logique de PART DEDANS

	// decoupe salons et cles
	std::vector<std::string> chanList = splitString(msg.getParams()[0], ',');
	std::vector<std::string> keyList;
	if (msg.getParams().size() >= 2)
		keyList = splitString(msg.getParams()[1], ',');

	for (std::size_t cIdx = 0; cIdx < chanList.size(); cIdx++)
	{
		std::string chanName = chanList[cIdx];
		std::string providedKey = (cIdx < keyList.size()) ? keyList[cIdx] : "";

		if (chanName.empty() || chanName[0] != '#')
		{
			std::string err = ERR_BADCHANMASK(sender.nickname, chanName);
			send(socketFd, err.c_str(), err.length(), 0);
			continue;
		}

		std::vector<Channels>& channels = server.getChannels();
		int chanIdx = -1;

		for (std::size_t i = 0; i < channels.size(); i++)
		{
			if (channels[i].getName() == chanName)
			{
				chanIdx = static_cast<int>(i);
				break;
			}
		}

		//creation / verif canal
		if (chanIdx == -1)
		{
			channels.push_back(Channels(socketFd, chanName));
			chanIdx = static_cast<int>(channels.size() - 1);
		}
		else
		{
			Channels& chan = channels[chanIdx];

			if (chan.isOp(socketFd))
			{
				continue;
			}

			bool alreadyIn = false;
			std::vector<int> currentUsers = chan.getUser();
			for (std::size_t i = 0; i < currentUsers.size(); i++)
			{
				if (currentUsers[i] == socketFd)
				{
					alreadyIn = true;
					break;
				}
			}
			if (alreadyIn)
				continue;

			//verif mode+k (Password)
			if (chan.getNeedPassword())
			{
				if (providedKey.empty() || providedKey != chan.getPassword())
				{
					std::string err = ERR_BADCHANNELKEY(sender.nickname, chanName);
					send(socketFd, err.c_str(), err.length(), 0);
					continue;
				}
			}

			//verif mode+l(User Limit)
			if (chan.getHasUserLimit())
			{
				std::size_t totalUsers = chan.getUser().size();
				if (totalUsers >= static_cast<std::size_t>(chan.getLimitUser()))
				{
					std::string err = ERR_CHANNELISFULL(sender.nickname, chanName);
					send(socketFd, err.c_str(), err.length(), 0);
					continue;
				}
			}

			//verif mode+i (Invite Only)
			if (chan.getNeedInvite())
			{
				std::string err = ERR_INVITEONLYCHAN(sender.nickname, chanName);
				send(socketFd, err.c_str(), err.length(), 0);
				continue;
			}

			chan.addUser(socketFd);
		}

		Channels& chan = channels[chanIdx];

		// msg broadcast (JOIN)
		std::string prefix = ":" + sender.nickname + "!" + sender.user + "@127.0.0.1";
		std::string joinMsg = prefix + " JOIN " + chanName + "\r\n";

		chan.sendMsg(-1, server.getServerSocket(), joinMsg);

		if (!chan.getTopic().empty())
		{
			std::string topicReply = RPL_TOPIC(sender.nickname, chanName, chan.getTopic());
			send(socketFd, topicReply.c_str(), topicReply.length(), 0);
		}

		//liste des membres
		std::string userList = "";
		std::vector<int> users = chan.getUser();

		//owners
		for (std::size_t i = 0; i < users.size(); i++)
		{
			if (!userList.empty())
				userList += " ";

			int userFd = users[i];
			std::string prefixSymbol = chan.isOp(userFd) ? "@" : "";
			std::string nick = server.getClients().getClientInfo()[userFd].nickname;

			userList += prefixSymbol + nick;
		}

		std::string namReply = RPL_NAMREPLY(sender.nickname, chanName, userList);
		std::string endNames = RPL_ENDOFNAMES(sender.nickname, chanName);

		send(socketFd, namReply.c_str(), namReply.length(), 0);
		send(socketFd, endNames.c_str(), endNames.length(), 0);
	}
}
