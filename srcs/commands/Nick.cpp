/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Nick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kevlim <kevlim@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/29 13:01:42 by kevlim            #+#    #+#             */
/*   Updated: 2026/07/29 14:36:04 by kevlim           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"
#include "Server.hpp"
#include "Client.hpp"
#include "Message.hpp"
#include <iostream>

//set le nickname
void	cmdNick(Server& server, Client::ClientInfo& sender, int socketFd, const Message& msg)
{
	(void)server;
	(void)sender;
	(void)socketFd;
	//verif si nick donne
	if (msg.getParams().empty())
	{
		std::cout << "[NICK] Erreur: Aucun nickname fourni" << std::endl;
		// a remplacer par : envoyer ERR_NONICKNAMEGIVEN (431)
		return;
	}
	sender.nickname = msg.getParams()[0];

	//TODO: verif si nickname deja pris (isNickInUse())
	//maj pseudo client
	/* std::string oldNick = sender.getNickname();
	sender.setNickname(newNick);*/
	std::cout << "[NICK] Nouveau pseudo : " << sender.nickname << std::endl;

	if (!sender.nickname.empty() && !sender.user.empty())
	{
		std::string welcome = ":server 001 " + sender.nickname + " :Welcome to the IRC Network " + sender.nickname + "\r\n";
		send(socketFd, welcome.c_str(), welcome.length(), 0);
		std::cout << "[SERVER] Sent RPL_WELCOME (001) to " << sender.nickname << std::endl;
	}
}
