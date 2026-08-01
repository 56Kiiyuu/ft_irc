/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gabch <gabch@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/29 13:16:57 by kevlim            #+#    #+#             */
/*   Updated: 2026/07/29 17:47:29 by gabch            ###   ########.fr       */
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
	(void)server;
	(void)sender;
	(void)socketFd;

	std::vector<Channels>& channels = server.getChannels();


	if (!sender.isRegistered)
	{
		std::string clientNick = sender.nickname.empty() ? "*" : sender.nickname;
		std::string err = ERR_NOTREGISTERED(clientNick);
		send(socketFd, err.c_str(), err.length(), 0);
		return;
	}
	std::cout << "[JOIN] Tentative de join sur le channel : "
			  << (msg.getParams().empty() ? "aucun" : msg.getParams()[0]) << std::endl;
	if (msg.getParams()[0][0] == '#')
	{
		if (channels.empty() == true)
		{
			channels.push_back(Channels(socketFd, msg.getParams()[0]));
			std::cout << "Create new channel: " << msg.getParams()[0] << std::endl;
		}
		else
		{
			std::size_t i = 0;
			while (i < channels.size())
			{
				if (channels[i].getName() == msg.getParams()[0])
				{
					channels[i].joinChannels(socketFd);
					std::cout << "Join channel: " << msg.getParams()[0] << std::endl;
					return ;
				}
				i++;
			}
			channels.push_back(Channels(socketFd, msg.getParams()[0]));
			std::cout << "Create new channel: " << msg.getParams()[0] << std::endl;
		}
	}
}
