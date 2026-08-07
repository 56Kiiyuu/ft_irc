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
	bool plus = true;
	std::size_t indexArgs = 2;

	// pour les modes
	std::string appliedModes = "";
	std::string appliedArgs = "";
	char lastSign = '\0';
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

		bool modeChanged = false;
		std::string currentArg = "";

		switch (modes[i])
		{
			case 'i':
				if (chan.getNeedInvite() != plus)
				{
					chan.setNeedInvite(plus);
					modeChanged = true;
				}
				break;
			case 't':
				if (chan.getRestrictedTopic() != plus)
				{
					chan.setRestrictedTopic(plus);
					modeChanged = true;
				}
				break;
			case 'k':
				if (plus == true)
				{
					if (indexArgs >= msg.getParams().size())
						continue; // Secu out-of-bounds
					currentArg = msg.getParams()[indexArgs++];
					chan.setNeedPassword(true);
					chan.setPassword(currentArg);
					modeChanged = true;
				}
				else
				{
					if (chan.getNeedPassword())
					{
						chan.setNeedPassword(false);
						chan.setPassword("");
						modeChanged = true;
					}
				}
				break;
			case 'o':
			{
				if (indexArgs >= msg.getParams().size())
						continue; // Secu out-of-bounds
				std::string targetNick = msg.getParams()[indexArgs++];
				currentArg = targetNick;

				int targetFd = -1;
				std::map<int, Client::ClientInfo>& clientMap = server.getClients().getClientInfo();
				std::map<int, Client::ClientInfo>::iterator it;
				for (it = clientMap.begin(); it != clientMap.end(); ++it)
				{
					if (it->second.nickname == targetNick)
					{
						targetFd = it->first;
						break;
					}
				}

				if (targetFd == -1)
				{
					std::string err = ERR_NOSUCHNICK(sender.nickname, targetNick);
					send(socketFd, err.c_str(), err.length(), 0);
					break;
				}

				if (plus)
				{
					if (!chan.isOp(targetFd))
					{
						chan.addNewOperator(targetFd);
						modeChanged = true;
					}
				}
				else
				{
					if (chan.isOp(targetFd))
					{
						chan.deleteOperator(targetFd);
						modeChanged = true;
					}
				}
				break;
			}
			case 'l':
				if (plus == true)
				{
					if (indexArgs >= msg.getParams().size())
						continue; //secu out-of-bounds

					int limit = 0;
					std::string limitStr = msg.getParams()[indexArgs++];
					std::stringstream ss(limitStr);
					ss >> limit;

					if (limit > 0)
					{
						chan.setHasUserLimit(true, limit);
						currentArg = limitStr;
						modeChanged = true;
					}
				}
				else
				{
					if (chan.getHasUserLimit())
					{
						chan.setHasUserLimit(false, 0);
						modeChanged = true;
					}
				}
				break;

			default:
			{
				std::string err = ERR_UNKNOWNMODE(sender.nickname, std::string(1, modes[i]));
				send(socketFd, err.c_str(), err.length(), 0);
			}
		}

		if (modeChanged)
		{
			char currentSign = plus ? '+' : '-';
			if (currentSign != lastSign)
			{
				appliedModes += currentSign;
				lastSign = currentSign;
			}
			appliedModes += modes[i];
			if (!currentArg.empty())
				appliedArgs += " " + currentArg;
		}
	}

	if (!appliedModes.empty())
	{
		std::string prefix = ":" + sender.nickname + "!" + sender.user + "@127.0.0.1";
		std::string formattedMsg = prefix + " MODE " + channel + " " + appliedModes + appliedArgs + "\r\n";

		chan.sendMsg(-1, server.getServerSocket(), formattedMsg);

		std::cout << "[MODE] Applied -> " << formattedMsg;
	}
	// on laisse vide pr l'instant
}
