/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Nick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kevlim <kevlim@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/29 13:01:42 by kevlim            #+#    #+#             */
/*   Updated: 2026/07/31 15:09:45 by kevlim           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"
#include "Server.hpp"
#include "Client.hpp"
#include "Message.hpp"
#include "NumericReplies.hpp"
#include <iostream>
#include <sys/socket.h>

static bool	isValidNickname(const std::string& nick)
{
	// 9 chars maximum
	if (nick.empty() || nick.length() > 9)
		return false;
	// forbidden to start with digit or invalid char
	if (std::isdigit(nick[0]) || nick[0] == '-' || nick[0] == '#' || nick[0] == ':')
		return false;
	//valid chars
	std::string validChars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789-[]{\\}\\|";
	for (size_t i = 0; i < nick.length(); ++i)
	{
		if (validChars.find(nick[i]) == std::string::npos)
			return false;
	}
	return true;
}
//set le nickname
void	cmdNick(Server& server, Client::ClientInfo& sender, int socketFd, const Message& msg)
{
	std::string clientNick = sender.nickname.empty() ? "*" : sender.nickname;

	//check if nickname
	if (msg.getParams().empty() || msg.getParams()[0].empty())
	{
		std::string err = ERR_NONICKNAMEGIVEN(clientNick);
		send(socketFd, err.c_str(), err.length(), 0);
		std::cout << "[NICK] ERR_NONICKNAMEGIVEN (431) sent" << std::endl;
		return;
	}

	std::string newNick = msg.getParams()[0];

	// if invalid chars
	if (!isValidNickname(newNick))
	{
		std::string err = ERR_ERRONEUSNICKNAME(clientNick, newNick);
		send(socketFd, err.c_str(), err.length(), 0);
		std::cout << "[NICK] ERR_ERRONEUSNICKNAME (432) pour : " << newNick << std::endl;
		return;
	}

	// if nickname already used
	if (server.isNickInUse(newNick))
	{
		std::string err = ERR_NICKNAMEINUSE(clientNick, newNick);
		send(socketFd, err.c_str(), err.length(), 0);
		std::cout << "[NICK] ERR_NICKNAMEINUSE (433) pour : " << newNick << std::endl;
		return;
	}

	std::string oldNick = sender.nickname;
	sender.nickname = newNick;
	std::cout << "[NICK] New nickname : " << sender.nickname << std::endl;
	if (!sender.nickname.empty() && !sender.user.empty())
	{
		std::string welcome = ":server 001 " + sender.nickname + " :Welcome to the IRC Network " + sender.nickname + "\r\n";
		send(socketFd, welcome.c_str(), welcome.length(), 0);
		std::cout << "[SERVER] Sent RPL_WELCOME (001) to " << sender.nickname << std::endl;
	}

	// if already registered and changed nick
	if (sender.isRegistered)
	{
		std::string nickMsg = ":" + oldNick + "!" + sender.user + "@127.0.0.1 NICK " + newNick + "\r\n";
		send(socketFd, nickMsg.c_str(), nickMsg.length(), 0);

		// TODO: Prevenir aussi les membres des salons ou se trouve le client
		return;
	}

	// if register with new nick
	if (!sender.isRegistered && sender.hasPass && !sender.nickname.empty() && !sender.user.empty())
	{
		sender.isRegistered = true;

		std::string welcome = RPL_WELCOME(sender.nickname, sender.user, "127.0.0.1");
		send(socketFd, welcome.c_str(), welcome.length(), 0);

		std::string yourHost = RPL_YOURHOST(sender.nickname);
		send(socketFd, yourHost.c_str(), yourHost.length(), 0);

		std::cout << "[SERVER] Client registered success (NICK) : " << sender.nickname << std::endl;
	}
}
