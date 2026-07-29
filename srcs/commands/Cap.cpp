/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cap.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kevlim <kevlim@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/29 14:08:38 by kevlim            #+#    #+#             */
/*   Updated: 2026/07/29 14:21:03 by kevlim           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"
#include "Server.hpp"
#include "Message.hpp"
#include <sys/socket.h>
#include <iostream>

void cmdCap(Server& server, Client::ClientInfo& sender, int socketFd, const Message& msg)
{
	(void)server;
	(void)sender;

	if (!msg.getParams().empty() && msg.getParams()[0] == "LS")
	{
		std::string reply = ":server CAP * LS :\r\n";
		send(socketFd, reply.c_str(), reply.length(), 0);
		std::cout << "[CAP] Réponse envoyée à Irssi sur FD " << socketFd << std::endl;
	}
}
