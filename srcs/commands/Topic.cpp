/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Topic.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kevlim <kevlim@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/05 16:32:07 by kevlim            #+#    #+#             */
/*   Updated: 2026/08/05 17:02:55 by kevlim           ###   ########.fr       */
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

void	cmdTopic(Server& server, Client::ClientInfo& sender, int socketFd, const Message& msg)
{
	// si enregistre
	if (!sender.isRegistered)
	{
		std::string clientNick = sender.nickname.empty() ? "*" : sender.nickname;
		send(socketFd, ERR_NOTREGISTERED(clientNick).c_str(), ERR_NOTREGISTERED(clientNick).length(), 0);
		return;
	}

	// si bons params
	if (msg.getParams().empty() || msg.getParams()[0].empty())
	{
		send(socketFd, ERR_NEEDMOREPARAMS(sender.nickname, "TOPIC").c_str(), ERR_NEEDMOREPARAMS(sender.nickname, "TOPIC").length(), 0);
		return;
	}

	std::string chanName = msg.getParams()[0];

	//salons existe ?
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

	if (chanIdx == -1)
	{
		send(socketFd, ERR_NOSUCHCHANNEL(sender.nickname, chanName).c_str(), ERR_NOSUCHCHANNEL(sender.nickname, chanName).length(), 0);
		return;
	}

	Channels& chan = channels[chanIdx];

	//si client est dans le channel
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
		send(socketFd, ERR_NOTONCHANNEL(sender.nickname, chanName).c_str(), ERR_NOTONCHANNEL(sender.nickname, chanName).length(), 0);
		return;
	}

	//lecture
	if (msg.getParams().size() == 1)
	{
		if (chan.getTopic().empty())
		{
			std::string reply = RPL_NOTOPIC(sender.nickname, chanName);
			send(socketFd, reply.c_str(), reply.length(), 0);
		}
		else
		{
			std::string reply = RPL_TOPIC(sender.nickname, chanName, chan.getTopic());
			send(socketFd, reply.c_str(), reply.length(), 0);

			std::string topicWhoTime = RPL_TOPICWHOTIME(sender.nickname, chanName, chan.getTopicSetter(), chan.getTopicTime());
			send(socketFd, topicWhoTime.c_str(), topicWhoTime.length(), 0);
		}
		return;
	}

	//modif
	if (chan.getRestrictedTopic() && !chan.isOp(socketFd))
	{
		std::string err = ERR_CHANOPRIVSNEEDED(sender.nickname, chanName);
		send(socketFd, err.c_str(), err.length(), 0);
		return;
	}

	std::string newTopic = msg.getParams()[1];
	std::string setterMask = sender.nickname + "!" + sender.user + "@127.0.0.1";

	chan.setTopic(newTopic, setterMask);

	std::string prefix = ":" + setterMask;
	std::string topicMsg = prefix + " TOPIC " + chanName + " :" + newTopic + "\r\n";

	chan.sendMsg(-1, server.getServerSocket(), topicMsg); // sendMsg a tout le monde si msgFd = -1
	send(socketFd, topicMsg.c_str(), topicMsg.length(), 0);
}
