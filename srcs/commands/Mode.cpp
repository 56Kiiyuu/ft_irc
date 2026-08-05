#include "Command.hpp"
#include "Server.hpp"
#include "Client.hpp"
#include "Message.hpp"
#include "NumericReplies.hpp"
#include <iostream>
#include <sstream>


void cmdMode(Server& server, Client::ClientInfo& sender, int socketFd, const Message& msg)
{

	if (msg.getParams().empty())
	{
		std::string err = ERR_NEEDMOREPARAMS(sender.nickname, "MODE");
		send(socketFd, err.c_str(), err.length(), 0);
		return;
	}


	std::string channel = msg.getParams()[0];
	// std::vector<Channels>& channels = server.getChannels();
	if (channel.empty() || channel[0] != '#')
		return;

	int indexChannel = -1;
	for (std::size_t i=0 ; i < server.getChannels().size() ; i++)
	{
		if (server.getChannels()[i].getName() == channel)
		{
			indexChannel = static_cast<int>(i);
			break ;
		}
	}

	if (indexChannel == -1)
	{
		std::string err = ERR_NOSUCHCHANNEL(sender.nickname, channel);
		send(socketFd, err.c_str(), err.length(), 0);
		return ;
	}

	Channels& chan = server.getChannels()[indexChannel];
	//si MODE #channel sans +i, +k
	if (msg.getParams().size() < 2)
	{
		std::string activeModes = "+";
		std::string modeArgs = "";

		if (chan.getNeedInvite())
			activeModes += "i";
		if (chan.getRestrictedTopic())
			activeModes += "t";
		if (chan.getNeedPassword())
		{
			activeModes += "k";
			modeArgs += " " + chan.getPassword();
		}
		if (chan.getHasUserLimit())
		{
			activeModes += "l";
			std::stringstream ss;
			ss << chan.getLimitUser();
			modeArgs += " " + ss.str();
		}

		std::string reply = RPL_CHANNELMODEIS(sender.nickname, channel, activeModes, modeArgs);
		send(socketFd, reply.c_str(), reply.length(), 0);
		return;
	}

	if (!chan.isOp(sender.fd))
	{
		std::string err = ERR_CHANOPRIVSNEEDED(sender.nickname, channel);
		send(socketFd, err.c_str(), err.length(), 0);
		return;
	}

	std::string modes = msg.getParams()[1];
	bool plus = false;
	std::size_t indexArgs = 2;
	for (std::size_t i=0 ; i < modes.size() ; i++)
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
			chan.setNeedInvite(plus);
			continue;
		case 't':
			chan.setRestrictedTopic(plus);
			continue;
		case 'k':
			if (plus == true)
			{
				if (indexArgs >= msg.getParams().size())
					continue; // Secu out-of-bounds
				chan.setNeedPassword(true);
				chan.setPassword(msg.getParams()[indexArgs]);
				indexArgs++;
			}
			else
				chan.setNeedPassword(false);
			continue;
		case 'o':
		{
			if (indexArgs >= msg.getParams().size())
					continue; // Secu out-of-bounds
			std::string targetNick = msg.getParams()[indexArgs];

			if (plus == true)
			{
				std::vector<int> user = chan.getUser();
				for (std::size_t j = 0; j < user.size(); j++)
				{
					if (server.getClients().getClientInfo()[user[j]].nickname == targetNick)
					{
						chan.addNewOperator(static_cast<int>(j));
						break;
					}
				}
			}
			else
			{
				std::vector<int> owner = chan.getOwner();
				for (std::size_t j=0 ; j < owner.size() ; j++)
				{
					if (server.getClients().getClientInfo()[owner[j]].nickname == targetNick)
					{
						chan.deleteOperator(static_cast<int>(j));
						break;
					}
				}
			}
			indexArgs++;
			continue;
		}

		case 'l':
			if (plus == true)
			{
				if (indexArgs >= msg.getParams().size())
					continue; //secu out-of-bounds

				int	limit = 0;
				std::stringstream	ss(msg.getParams()[indexArgs]);
				ss >> limit;

				if (limit > 0)
					chan.setHasUserLimit(true, limit);
				indexArgs++;
			}
			else
				chan.setHasUserLimit(false, 0);
			continue;

		default:
			{
				std::string err = ERR_UNKNOWNMODE(sender.nickname, std::string(1, modes[i]));
				send(socketFd, err.c_str(), err.length(), 0);
			}
			continue;
		}

	}
	std::string prefix = ":" + sender.nickname + "!" + sender.user + "@127.0.0.1";
	std::string formattedMsg = prefix + " MODE";

	for (std::size_t i=0 ; i < msg.getParams().size() ; i++)
	{
		formattedMsg += " " + msg.getParams()[i];
	}
	formattedMsg += "\r\n";
	chan.sendMsg(socketFd, server.getServerSocket(), formattedMsg);
	send(socketFd, formattedMsg.c_str(), formattedMsg.length(), 0);

	std::cout << "[MODE] Finish setup mode -> " << std::endl;
	std::cout << "[MODE] " << formattedMsg << std::endl;
	std::cout << "[MODE] i:" << server.getChannels()[indexChannel].getNeedInvite() << " k:" << server.getChannels()[indexChannel].getNeedPassword() << " Password:" << server.getChannels()[indexChannel].getNeedPassword() << std::endl;
	// on laisse vide pr l'instant
}
