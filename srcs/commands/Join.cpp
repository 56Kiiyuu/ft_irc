/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kevlim <kevlim@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/29 13:16:57 by kevlim            #+#    #+#             */
/*   Updated: 2026/07/31 15:39:09 by kevlim           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"
#include "Server.hpp"
#include "Client.hpp"
#include "Message.hpp"
#include "NumericReplies.hpp"
#include <iostream>
#include <sys/socket.h>

void cmdJoin(Server& server, Client::ClientInfo& sender, int socketFd, const Message& msg)
{
	(void)server;

	if (!sender.isRegistered)
	{
		std::string clientNick = sender.nickname.empty() ? "*" : sender.nickname;
		std::string err = ERR_NOTREGISTERED(clientNick);
		send(socketFd, err.c_str(), err.length(), 0);
		return;
	}
	std::cout << "[JOIN] Tentative de join sur le channel : "
			  << (msg.getParams().empty() ? "aucun" : msg.getParams()[0]) << std::endl;
}
