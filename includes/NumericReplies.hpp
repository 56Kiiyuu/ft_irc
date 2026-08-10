#ifndef NUMERICREPLIES_HPP
#define NUMERICREPLIES_HPP
#define SERVER_NAME std::string("ircserv")

#include <sstream>
#include <string>

template <typename T>
std::string to_string(T value)
{
	std::stringstream ss;
	ss << value;
	return ss.str();
}

//SUCCESS / INFO (001 - 399)
#define RPL_WELCOME(nick, user, host) (":" + SERVER_NAME + " 001 " + nick + " :Welcome to the Internet Relay Network " + nick + "!" + user + "@" + host + "\r\n")
#define RPL_YOURHOST(nick) (":" + SERVER_NAME + " 002 " + nick + " :Your host is " + SERVER_NAME + ", running version 1.0\r\n")
#define RPL_CHANNELMODEIS(nick, channel, mode, mode_params) (":" + SERVER_NAME + " 324 " + nick + " " + channel + " " + mode + mode_params + "\r\n")
#define RPL_NOTOPIC(nick, channel) (":" + SERVER_NAME + " 331 " + nick + " " + channel + " :No topic is set\r\n")
#define RPL_TOPIC(nick, channel, topic) (":" + SERVER_NAME + " 332 " + nick + " " + channel + " :" + topic + "\r\n")
#define RPL_TOPICWHOTIME(nick, channel, setter, topicTime) (":" + SERVER_NAME + " 333 " + nick + " " + channel + " " + setter + " " + to_string(topicTime) + "\r\n")
#define RPL_NAMREPLY(nick, channel, users) (":" + SERVER_NAME + " 353 " + nick + " = " + channel + " :" + users + "\r\n")
#define RPL_ENDOFNAMES(nick, channel) (":" + SERVER_NAME + " 366 " + nick + " " + channel + " :End of /NAMES list\r\n")
#define RPL_INVITE(sender_nick, invited_nick, channel) (":" + SERVER_NAME + " 341 " + sender_nick + " " + invited_nick + " " + channel + "\r\n")

//ERRORS (400 - 599)
#define ERR_NOSUCHNICK(nick, target) (":" + SERVER_NAME + " 401 " + nick + " " + target + " :No such nick/channel\r\n")
#define ERR_NOSUCHCHANNEL(nick, channel) (":" + SERVER_NAME + " 403 " + nick + " " + channel + " :No such channel\r\n")
#define ERR_CANNOTSENDTOCHAN(nick, channel) (":" + SERVER_NAME + " 404 " + nick + " " + channel + " :Cannot send to channel\r\n")
#define ERR_TOOMANYCHANNELS(nick, channel) (":" + SERVER_NAME + " 405 " + (nick.empty() ? "*" : nick) + " " + channel + " :You have joined too many channels\r\n")
#define ERR_NORECIPIENT(nick, command) (":" + SERVER_NAME + " 411 " + nick + " :No recipient given (" + command + ")\r\n")
#define ERR_NOTEXTTOSEND(nick) (":" + SERVER_NAME + " 412 " + nick + " :No text to send\r\n")
#define ERR_UNKNOWNCOMMAND(nick, cmd) (":" + SERVER_NAME + " 421 " + nick + " " + cmd + " :Unknown command\r\n")
#define ERR_NONICKNAMEGIVEN(nick) (":" + SERVER_NAME + " 431 " + (nick.empty() ? "*" : nick) + " :No nickname given\r\n")
#define ERR_ERRONEUSNICKNAME(nick, attempted) (":" + SERVER_NAME + " 432 " + (nick.empty() ? "*" : nick) + " " + attempted + " :Erroneous nickname\r\n")
#define ERR_NICKNAMEINUSE(nick, attempted) (":" + SERVER_NAME + " 433 " + (nick.empty() ? "*" : nick) + " " + attempted + " :Nickname is already in use\r\n")
#define ERR_USERNOTINCHANNEL(nick, channel) (":" + SERVER_NAME + " 441 " + nick + " " + channel + " :Target is not in the channel\r\n")
#define ERR_NOTONCHANNEL(nick, channel) (":" + SERVER_NAME + " 442 " + nick + " " + channel + " :You're not on that channel\r\n")
#define ERR_USERALREADYINCHANNEL(nick, target, channel) (":" + SERVER_NAME + " 443 " + nick + " " + target + " " + channel + ":is already on channel\r\n")

#define ERR_NOTREGISTERED(nick) (":" + SERVER_NAME + " 451 " + (nick.empty() ? "*" : nick) + " :You have not registered\r\n")
#define ERR_NEEDMOREPARAMS(nick, cmd) (":" + SERVER_NAME + " 461 " + (nick.empty() ? "*" : nick) + " " + cmd + " :Not enough parameters\r\n")
#define ERR_ALREADYREGISTRED(nick) (":" + SERVER_NAME + " 462 " + nick + " :Unauthorized command (already registered)\r\n")
#define ERR_PASSWDMISMATCH(nick) (":" + SERVER_NAME + " 464 " + (nick.empty() ? "*" : nick) + " :Password incorrect\r\n")
#define ERR_CHANNELISFULL(nick, channel) (":" + SERVER_NAME + " 471 " + (nick.empty() ? "*" : nick) + " " + channel + " :Cannot join channel (+l)\r\n")
#define ERR_UNKNOWNMODE(nick, modechar) (":" + SERVER_NAME + " 472 " + (nick) + " " + (modechar) + " :is unknown mode char to me\r\n")
#define ERR_INVITEONLYCHAN(nick, channel) (":" + SERVER_NAME + " 473 " + (nick.empty() ? "*" : nick) + " " + channel + " :Cannot join channel (+i)\r\n")
#define ERR_BADCHANNELKEY(nick, channel) (":" + SERVER_NAME + " 475 " + (nick.empty() ? "*" : nick) + " " + channel + " :Cannot join channel (+k)\r\n")
#define ERR_BADCHANMASK(nick, channel) (":" + SERVER_NAME + " 476 " + (nick.empty() ? "*" : nick) + " " + channel + " :Bad Channel Mask\r\n")
#define ERR_CHANOPRIVSNEEDED(nick, channel) (":" + SERVER_NAME + " 482 " + (nick) + " " + (channel) + " :You're not channel operator\r\n")
#endif
