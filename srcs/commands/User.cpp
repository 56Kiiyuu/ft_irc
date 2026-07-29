/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kevlim <kevlim@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/29 13:09:48 by kevlim            #+#    #+#             */
/*   Updated: 2026/07/29 14:33:03 by kevlim           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"
#include "Server.hpp"
#include "Client.hpp"
#include "Message.hpp"
#include <iostream>

//enregistre le nom user et son realname
void	cmdUser(Server& server, Client::ClientInfo& sender, int socketFd, const Message& msg)
{
	(void)server;
	(void)sender;
	(void)socketFd;
	//4 args : <username> <hostname> <servername> :<realname>
	if (msg.getParams().size() < 4)
	{
		std::cout << "[USER] Erreur: Pas assez de parametres" << std::endl;
		// a remplacer par : envoyer ERR_NEEDMOREPARAMS (461)
		return;
	}

	sender.user = msg.getParams()[0];
	sender.realname = msg.getParams()[3];

	std::cout << "[USER] User enregistre : " << sender.user << " (" << sender.realname << ")" << std::endl;
	if (!sender.nickname.empty() && !sender.user.empty())
	{
		std::string welcome = ":server 001 " + sender.nickname + " :Welcome to the IRC Network " + sender.nickname + "\r\n";
		send(socketFd, welcome.c_str(), welcome.length(), 0);
		std::cout << "[SERVER] Sent RPL_WELCOME (001) to " << sender.nickname << std::endl;
	}
}
