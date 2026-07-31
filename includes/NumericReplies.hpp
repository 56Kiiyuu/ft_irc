#ifndef NUMERICREPLIES_HPP
#define NUMERICREPLIES_HPP

#define SERVER_NAME std::string("ircserv")

//SUCCESS / INFO (001 - 399)
#define RPL_WELCOME(nick, user, host) (":" + SERVER_NAME + " 001 " + nick + " :Welcome to the Internet Relay Network " + nick + "!" + user + "@" + host + "\r\n")
#define RPL_YOURHOST(nick) (":" + SERVER_NAME + " 002 " + nick + " :Your host is " + SERVER_NAME + ", running version 1.0\r\n")
#define RPL_TOPIC(nick, channel, topic) (":" + SERVER_NAME + " 332 " + nick + " " + channel + " :" + topic + "\r\n")
#define RPL_NAMREPLY(nick, channel, users) (":" + SERVER_NAME + " 353 " + nick + " = " + channel + " :" + users + "\r\n")
#define RPL_ENDOFNAMES(nick, channel) (":" + SERVER_NAME + " 366 " + nick + " " + channel + " :End of /NAMES list\r\n")

//ERRORS (400 - 599)
#define ERR_NOSUCHNICK(nick, target) (":" + SERVER_NAME + " 401 " + nick + " " + target + " :No such nick/channel\r\n")
#define ERR_NOSUCHCHANNEL(nick, channel) (":" + SERVER_NAME + " 403 " + nick + " " + channel + " :No such channel\r\n")
#define ERR_TOOMANYCHANNELS(nick, channel) (":" + SERVER_NAME + " 405 " + (nick.empty() ? "*" : nick) + " " + channel + " :You have joined too many channels\r\n")
#define ERR_UNKNOWNCOMMAND(nick, cmd) (":" + SERVER_NAME + " 421 " + nick + " " + cmd + " :Unknown command\r\n")
#define ERR_NONICKNAMEGIVEN(nick) (":" + SERVER_NAME + " 431 " + (nick.empty() ? "*" : nick) + " :No nickname given\r\n")
#define ERR_ERRONEUSNICKNAME(nick, attempted) (":" + SERVER_NAME + " 432 " + (nick.empty() ? "*" : nick) + " " + attempted + " :Erroneous nickname\r\n")
#define ERR_NICKNAMEINUSE(nick, attempted) (":" + SERVER_NAME + " 433 " + (nick.empty() ? "*" : nick) + " " + attempted + " :Nickname is already in use\r\n")
#define ERR_NOTONCHANNEL(nick, channel) (":" + SERVER_NAME + " 442 " + nick + " " + channel + " :You're not on that channel\r\n")
#define ERR_NOTREGISTERED(nick) (":" + SERVER_NAME + " 451 " + (nick.empty() ? "*" : nick) + " :You have not registered\r\n")
#define ERR_NEEDMOREPARAMS(nick, cmd) (":" + SERVER_NAME + " 461 " + (nick.empty() ? "*" : nick) + " " + cmd + " :Not enough parameters\r\n")
#define ERR_ALREADYREGISTRED(nick) (":" + SERVER_NAME + " 462 " + nick + " :Unauthorized command (already registered)\r\n")
#define ERR_PASSWDMISMATCH(nick) (":" + SERVER_NAME + " 464 " + (nick.empty() ? "*" : nick) + " :Password incorrect\r\n")
#define ERR_CHANNELISFULL(nick, channel) (":" + SERVER_NAME + " 471 " + (nick.empty() ? "*" : nick) + " " + channel + " :Cannot join channel (+l)\r\n")
#define ERR_INVITEONLYCHAN(nick, channel) (":" + SERVER_NAME + " 473 " + (nick.empty() ? "*" : nick) + " " + channel + " :Cannot join channel (+i)\r\n")
#define ERR_BADCHANNELKEY(nick, channel) (":" + SERVER_NAME + " 475 " + (nick.empty() ? "*" : nick) + " " + channel + " :Cannot join channel (+k)\r\n")
#define ERR_BADCHANMASK(nick, channel) (":" + SERVER_NAME + " 476 " + (nick.empty() ? "*" : nick) + " " + channel + " :Bad Channel Mask\r\n")
#endif
