#include "Command.hpp"
#include "Server.hpp"
#include "Client.hpp"
#include "Message.hpp"
#include <iostream>

void cmdMode(Server& server, Client::ClientInfo& sender, int socketFd, const Message& msg)
{
	(void)server;
	(void)sender;
	(void)socketFd;
	
	if (msg.getParams().empty())
	{
		std::cout << "[MOODE] Erreur: Aucun Mode selectionner" << std::endl;
		// ERR_NORECIPIENT (411)
	}

	std::string channel = msg.getParams()[0];
	std::vector<Channels>& channels = server.getChannels();
	int indexChannel;
	for (int i=0 ; i < server.getChannels().size() ; i++)
	{
		if (server.getChannels()[i].getName() == channel)
		{
			indexChannel = i;
			break ;
		}
	}


	std::string modes = msg.getParams()[1];
	bool plus = false;
	int indexArgs = 2;
	for (int i=0 ; i < modes.size() ; i++)
	{
		if (modes[i] == '+')
		{
			plus = true;
			continue;
		}
		if (modes[i] == '-')
		{
			plus = false;
			continue;
		}

		switch (modes[i])
		{
		case 'i':
			if (plus == true)
				server.getChannels()[indexChannel].setNeedInvite(true);
			else
				server.getChannels()[indexChannel].setNeedInvite(false);
			indexArgs++;
			continue;
		case 't':
			if (plus == true)
				server.getChannels()[indexChannel].setRestrictedTopic(true);
			else
				server.getChannels()[indexChannel].setRestrictedTopic(false);
			indexArgs++;
			continue;
		case 'k':
			if (plus == true)
			{
				server.getChannels()[indexChannel].setNeedPassword(true);
				server.getChannels()[indexChannel].setPassword(msg.getParams()[indexArgs]);
			}
			else
			server.getChannels()[indexChannel].setNeedPassword(false);
			indexArgs++;
			continue;
		case 'o':
			if (plus == true)
			{
				std::vector<int> user = server.getChannels()[indexChannel].getUser();
				int indexClient = -1;
				for (int i=0 ; i < user.size() ; i++)
				{
					if (server.getClients().getClientInfo()[user[i]].nickname == msg.getParams()[indexArgs])
					{
						indexClient = i;
						break ;
					}
				}
				if (indexClient == -1)
					continue;
				server.getChannels()[indexChannel].addNewOperator(indexClient);
			}
			else
			{
				std::vector<int> owner = server.getChannels()[indexChannel].getOwner();
				int indexClient = -1;
				for (int i=0 ; i < owner.size() ; i++)
				{
					if (server.getClients().getClientInfo()[owner[i]].nickname == msg.getParams()[indexArgs])
					{
						indexClient = i;
						break ;
					}
				}
				if (indexClient == -1)
					continue;
				server.getChannels()[indexChannel].deleteOperator(indexClient);
			}
			indexArgs++;
			continue;
		case 'l':
			if (plus == true)
				server.getChannels()[indexChannel].setHasUserLimit(true, std::atol(msg.getParams()[indexArgs].c_str()));
			else
				server.getChannels()[indexChannel].setHasUserLimit(false, 0);
			indexArgs++;
			continue;
		default:
			continue;
		}

	}
	std::string prefix = ":" + sender.nickname + "!" + sender.user + "@127.0.0.1";
	std::string formattedMsg = prefix + " MODE ";

	for (int i=0 ; i < msg.getParams().size() ; i++)
	{
		formattedMsg += msg.getParams()[i] + " ";
	}
	formattedMsg += "\r\n";
	// send(targetFd, formattedMsg.c_str(), formattedMsg.length(), 0);
	server.getChannels()[indexChannel].sendMsg(socketFd, server.getServerSocket(), formattedMsg);
	// on laisse vide pr l'instant
}
