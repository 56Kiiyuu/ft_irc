/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Pass.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kevlim <kevlim@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/29 12:50:44 by kevlim            #+#    #+#             */
/*   Updated: 2026/07/31 15:10:22 by kevlim           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"
#include "Server.hpp"
#include "Client.hpp"
#include "Message.hpp"
#include "NumericReplies.hpp"
#include <iostream>
#include <sys/socket.h>

//verif si mdp est correct
void	cmdPass(Server& server, Client::ClientInfo& sender, int socketFd, const Message& msg)
{
	std::string clientNick = sender.nickname.empty() ? "*" : sender.nickname;
	//check if pass exist
	if (msg.getParams().empty() || msg.getParams()[0].empty())
	{
		std::string err = ERR_NEEDMOREPARAMS(clientNick, "PASS");
		send(socketFd, err.c_str(), err.length(), 0);
		std::cout << "[PASS] ERR_NEEDMOREPARAMS (461) envoye a FD " << socketFd << std::endl;
		return;
	}

	//check if already registered
	if (sender.isRegistered)
	{
		std::string err = ERR_ALREADYREGISTRED(clientNick);
		send(socketFd, err.c_str(), err.length(), 0);
		std::cout << "[PASS] ERR_ALREADYREGISTRED (462) envoye a " << clientNick << std::endl;
		return;
	}

	std::string input_password = msg.getParams()[0];

	//check with server pass
	if (input_password != server.getPassword())
	{
		std::string err = ERR_PASSWDMISMATCH(clientNick);
		send(socketFd, err.c_str(), err.length(), 0);
		std::cout << "[PASS] ERR_PASSWDMISMATCH (464) envoye a FD " << socketFd << std::endl;

		sender.hasPass = false; // mdp invalid
		return;
	}
	std::cout << "[PASS] Mdp recu : " << input_password << std::endl;
}
