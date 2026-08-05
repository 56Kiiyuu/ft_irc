/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Privmsg.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kevlim <kevlim@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/29 13:17:54 by kevlim            #+#    #+#             */
/*   Updated: 2026/08/05 17:24:30 by kevlim           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"
#include "Server.hpp"
#include "Client.hpp"
#include "NumericReplies.hpp"
#include "Message.hpp"
#include <iostream>
#include "Channels.hpp"

void cmdPrivmsg(Server& server, Client::ClientInfo& sender, int socketFd, const Message& msg)
{

	if (!sender.isRegistered)
	{
		std::string clientNick = sender.nickname.empty() ? "*" : sender.nickname;
		std::string err = ERR_NOTREGISTERED(clientNick);
		send(socketFd, err.c_str(), err.length(), 0);
		return;
	}

	//pas de destinataire
	if (msg.getParams().empty() || msg.getParams()[0].empty())
	{
		std::string err = ERR_NORECIPIENT(sender.nickname, "PRIVMSG");
		send(socketFd, err.c_str(), err.length(), 0);
		return;
	}

	//pas de texte
	if (msg.getParams().size() < 2 || msg.getParams()[1].empty())
	{
		std::string err = ERR_NOTEXTTOSEND(sender.nickname);
		send(socketFd, err.c_str(), err.length(), 0);
		return;
	}

	std::string target = msg.getParams()[0];
	std::string text = msg.getParams()[1];
	std::string prefix = ":" + sender.nickname + "!" + sender.user + "@127.0.0.1";
	std::string formattedMsg = prefix + " PRIVMSG " + target + " :" + text + "\r\n";

	if (target[0] == '#')
	{
		std::vector<Channels>& channels = server.getChannels();
		int chanIdx = -1;

		for (std::size_t i = 0; i < channels.size(); i++)
		{
			if (channels[i].getName() == target)
			{
				chanIdx = static_cast<int>(i);
				break;
			}
		}

		//err : channel inexistant
		if (chanIdx == -1)
		{
			std::string err = ERR_NOSUCHNICK(sender.nickname, target);
			send(socketFd, err.c_str(), err.length(), 0);
			return;
		}

		Channels& chan = channels[chanIdx];

		// verif client dans le channel
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
			std::string err = ERR_CANNOTSENDTOCHAN(sender.nickname, target);
			send(socketFd, err.c_str(), err.length(), 0);
			return;
		}

		//envoie le msg a tous les autres membres a part lui
		chan.sendMsg(socketFd, server.getServerSocket(), formattedMsg);
		return;
	}

	//recherche du target dans la map de client
	std::map<int, Client::ClientInfo>& clientMap = server.getClients().getClientInfo();
	std::map<int, Client::ClientInfo>::iterator it;

	int targetFd = -1;
	for (it = clientMap.begin(); it != clientMap.end(); ++it)
	{
		if (it->second.nickname == target)
		{
			targetFd = it->first;
			break;
		}
	}

	//si pas trouve
	if (targetFd == -1)
	{
		std::string err = ERR_NOSUCHNICK(sender.nickname, target);
		send(socketFd, err.c_str(), err.length(), 0);
		return;
	}

	send(targetFd, formattedMsg.c_str(), formattedMsg.length(), 0);
	std::cout << "[PRIVMSG] " << sender.nickname << " -> " << target << " : " << text << std::endl;
}
