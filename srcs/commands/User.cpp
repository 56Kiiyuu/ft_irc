/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kevlim <kevlim@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/29 13:09:48 by kevlim            #+#    #+#             */
/*   Updated: 2026/07/31 15:09:31 by kevlim           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"
#include "Server.hpp"
#include "Client.hpp"
#include "Message.hpp"
#include "NumericReplies.hpp"
#include <iostream>
#include <sys/socket.h>

//enregistre le nom user et son realname
void	cmdUser(Server& server, Client::ClientInfo& sender, int socketFd, const Message& msg)
{
	(void)server;
	std::string clientNick = sender.nickname.empty() ? "*" : sender.nickname;

	//4 args : <username> <hostname> <servername> :<realname>
	if (msg.getParams().size() < 4)
	{
		std::string err = ERR_NEEDMOREPARAMS(clientNick, "USER");
		send(socketFd, err.c_str(), err.length(), 0);
		std::cout << "[USER] ERR_NEEDMOREPARAMS (461) send to FD" << std::endl;
		return;
	}

	if (sender.isRegistered)
	{
		std::string err = ERR_ALREADYREGISTRED(clientNick);
		send(socketFd, err.c_str(), err.length(), 0);
		std::cout << "[USER] ERR_ALREADYREGISTRED (462) send to" << std::endl;
		return;
	}

	sender.user = msg.getParams()[0];
	sender.realname = msg.getParams()[3];

	std::cout << "[USER] User enregistre : " << sender.user << " (" << sender.realname << ")" << std::endl;
	if (!sender.isRegistered && sender.hasPass && !sender.nickname.empty() && !sender.user.empty())
	{
		sender.isRegistered = true;

		std::string welcome = RPL_WELCOME(sender.nickname, sender.user, "127.0.0.1");
		send(socketFd, welcome.c_str(), welcome.length(), 0);

		std::string yourHost = RPL_YOURHOST(sender.nickname);
		send(socketFd, yourHost.c_str(), yourHost.length(), 0);
		std::cout << "[SERVER] Client registered success : " << sender.nickname << " (FD " << socketFd << ")" << std::endl;
	}
}
