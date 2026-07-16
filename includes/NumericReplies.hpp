#ifndef NUMERICREPLIES_HPP
#define NUMERICREPLIES_HPP

// SUCCESS/INFOS (001 to 399)
#define RPL_WELCOME(nick, user, host) ("001 " + nick + " :Welcome to the Internet Relay Network " + nick + "!" + user + "@" + host)
#define RPL_YOURHOST(nick, server) ("002 " + nick + " :Your host is " + server + ", running version 1.0")
#define RPL_TOPIC(nick, channel, topic) ("332 " + nick + " " + channel + " :" + topic)
#define RPL_NAMREPLY(nick, channel, users) ("353 " + nick + " = " + channel + " :" + users)
#define RPL_ENDOFNAMES(nick, channel) ("366 " + nick + " " + channel + " :End of /NAMES list")

// ERRORS (400 to 599)
#define ERR_NOSUCHNICK(nick, target) ("401 " + nick + " " + target + " :No such nick/channel")
#define ERR_NOSUCHCHANNEL(nick, channel) ("403 " + nick + " " + channel + " :No such channel")
#define ERR_UNKNOWNCOMMAND(nick, cmd) ("421 " + nick + " " + cmd + " :Unknown command")
#define ERR_NONICKNAMEGIVEN(nick) ("431 " + nick + " :No nickname given")
#define ERR_NICKNAMEINUSE(nick, attempted) ("433 " + nick + " " + attempted + " :Nickname is already in use")
#define ERR_NOTONCHANNEL(nick, channel) ("442 " + nick + " " + channel + " :You're not on that channel")
#define ERR_NOTREGISTERED(nick) ("451 " + nick + " :You have not registered")
#define ERR_NEEDMOREPARAMS(nick, cmd) ("461 " + nick + " " + cmd + " :Not enough parameters")
#define ERR_ALREADYREGISTRED(nick) ("462 " + nick + " :Unauthorized command (already registered)")
#define ERR_BADCHANNELKEY(nick, channel) ("475 " + nick + " " + channel + " :Cannot join channel (+k)")

#endif
