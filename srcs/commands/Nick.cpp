#include "Command.hpp"
#include "Server.hpp"
#include "Client.hpp"
#include "Message.hpp"
#include "NumericReplies.hpp"
#include <iostream>
#include <sys/socket.h>
#include <set>
#include <vector>

static bool isValidNickname(const std::string& nick)
{
    // 9 chars maximum
    if (nick.empty() || nick.length() > 9)
        return false;

    // allowed to start
    std::string allowedFirst = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ[]\\`^{}|_";
    if (allowedFirst.find(nick[0]) == std::string::npos)
        return false;

    std::string allowedChars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789[]\\`^{}|_-";
    for (size_t i = 0; i < nick.length(); ++i)
    {
        if (allowedChars.find(nick[i]) == std::string::npos)
            return false;
    }
    return true;
}

// set le nickname
void cmdNick(Server& server, Client::ClientInfo& sender, int socketFd, const Message& msg)
{
    std::string clientNick = sender.nickname.empty() ? "*" : sender.nickname;

    if (!sender.hasPass)
    {
        std::string err = ERR_NOTREGISTERED(clientNick);
        send(socketFd, err.c_str(), err.length(), 0);
        return;
    }

    // check if nickname
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

    // if already registered and changed nick
    if (sender.isRegistered)
    {
        // nick msg format RFC (:OldNick!user@host NICK :NewNick)
        std::string nickMsg = ":" + oldNick + "!" + sender.user + "@127.0.0.1 NICK :" + newNick + "\r\n";
        
        std::set<int> recipients;
        recipients.insert(socketFd);

        // announce other members in same channels
        std::vector<Channels*>& channelList = server.getChannels();
        for (size_t i = 0; i < channelList.size(); ++i)
        {
            if (channelList[i]->hasUser(socketFd))
            {
                const std::vector<int>& users = channelList[i]->getUsers();
                for (size_t j = 0; j < users.size(); ++j)
                {
                    recipients.insert(users[j]); // std::set suppresses duplicates
                }
            }
        }

        for (std::set<int>::iterator it = recipients.begin(); it != recipients.end(); ++it)
        {
            send(*it, nickMsg.c_str(), nickMsg.length(), 0);
        }
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
