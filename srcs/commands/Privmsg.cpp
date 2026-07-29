/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Privmsg.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gabch <gabch@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/29 13:17:54 by kevlim            #+#    #+#             */
/*   Updated: 2026/07/29 18:04:39 by gabch            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"
#include "Server.hpp"
#include "Client.hpp"
#include "Message.hpp"
#include <iostream>
#include "Channels.hpp"

void cmdPrivmsg(Server& server, Client::ClientInfo& sender, int socketFd, const Message& msg)
{
	(void)socketFd;
	//Verif des param
	if (msg.getParams().empty())
	{
		std::cout << "[PRIVMSG] Erreur: Aucun destinataire" << std::endl;
		// ERR_NORECIPIENT (411)
	}
	if (msg.getParams().size() < 2)
	{
		std::cout << "[PRIVMSG] Erreur: Aucun texte à envoyer" << std::endl;
		// ERR_NOTEXTTOSEND (412)
		return;
	}

	std::string target = msg.getParams()[0];
	std::string text = msg.getParams()[1];

	//pr l'instant j'ignore le #
	if (target[0] == '#')
	{
		// std::cout << "[PRIVMSG] Salon " << target << " ignore (pour l'instant)" << std::endl;

		std::size_t i = 0;
		std::vector<Channels>& channels = server.getChannels();
		while (i < channels.size())
		{
			std::cout << "channels name : " << channels[i].getName() << " Target : " << target << std::endl;
			if (channels[i].getName() == target)
			{
				std::string prefix = ":" + sender.nickname + "!" + sender.user + "@127.0.0.1";
				std::string formattedMsg = prefix + " PRIVMSG " + target + " :" + text + "\r\n";
				channels[i].sendMsg(socketFd, server.getServerSocket(), formattedMsg);
				std::cout << "[PRIVMSG] " << sender.nickname << " -> " << target << " : " << text << std::endl;
				break ;
			}
			i++;
		}
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
		std::cout << "[PRIVMSG] Erreur: Utilisateur " << target << " introuvable" << std::endl;
		// ERR_NOSUCHNICK (401)
		return;
	}

	std::string prefix = ":" + sender.nickname + "!" + sender.user + "@127.0.0.1";
	std::string formattedMsg = prefix + " PRIVMSG " + target + " :" + text + "\r\n";

	send(targetFd, formattedMsg.c_str(), formattedMsg.length(), 0);
	std::cout << "[PRIVMSG] " << sender.nickname << " -> " << target << " : " << text << std::endl;
}
