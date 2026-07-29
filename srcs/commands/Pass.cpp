/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Pass.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kevlim <kevlim@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/29 12:50:44 by kevlim            #+#    #+#             */
/*   Updated: 2026/07/29 14:37:14 by kevlim           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"
#include "Server.hpp"
#include "Client.hpp"
#include "Message.hpp"
#include <iostream>

//verif si mdp est correct
void	cmdPass(Server& server, Client::ClientInfo& sender, int socketFd, const Message& msg)
{
	(void)server; //link le serv
	(void)sender;
	(void)socketFd;
	//verif si mdp
	if (msg.getParams().empty())
	{
		std::cout << "[PASS] Erreur: Pas assez de parametres" << std::endl;
		// a remplacer par un envoyer ERR_NEEDMOREPARAMS (461)
		return;
	}
	std::string password = msg.getParams()[0];
	std::cout << "[PASS] Mdp recu : " << password << std::endl;

	// TODO : comparer avec le mdp reel du serveur
	// sender.setPassOK(true);
}
