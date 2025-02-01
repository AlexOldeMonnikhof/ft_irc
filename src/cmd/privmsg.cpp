#include "Server.hpp"
#include "error.hpp"

void    Server::msgChannel(int fd, const std::string &msg, const std::string &channel, bool sendToSender)
{
    size_t index = getChannelIndex(_channels, channel);
    size_t clientSize = _channels[index].getClientsSize();
    std::vector<std::string>  clients = _channels[index].getClients();
    for (size_t i = 0; i < clientSize; i++)
    {
        if (!sendToSender && getClientFd(clients[i]) == fd)
        {
            continue;
        }

        sendMsg(getClientFd(clients[i]), msg);
    }
}

void    Server::privmsgChannel(int fd, Command& cmd, const std::string &channel)
{
    if (!channelExist(channel))
    {
        sendMsg(fd, ERR_NOSUCHCHANNEL(_host, channel, _clients[fd].getNickname()));
        return;
    }

    size_t index = getChannelIndex(_channels, channel);
    if (!_channels[index].clientInChannel(_clients[fd].getNickname()))
    {
        sendMsg(fd, ERR_NOTONCHANNEL(_host, channel, _clients[fd].getNickname()));
        return;
    }

    std::string op = "";
    if (_channels[index].isOperator(_clients[fd].getNickname()))
    {
        op += "@";
    }

    msgChannel(fd, PRIVMSG_FORMATCH(_clients[fd].getNickname() + op, _clients[fd].getUsername(), _host, channel, cmd.getCmd(2)), channel, false);
}

void    Server::privmsgClient(int fd, Command& cmd, const std::string &nick)
{
    int targetFd = getClientFd(nick);
    if (!targetFd)
    {
        sendMsg(fd, ERR_NOSUCHNICK(_host, nick));
    } else
    {
        sendMsg(targetFd, PRIVMSG_FORMATUSER(_clients[fd].getNickname(), _clients[fd].getUsername(), _host, nick, cmd.getCmd(2)));
    }
}

void    Server::cmdPrivmsg(int fd, Command& cmd)
{
    std::vector<std::string>  targets;
    if (cmd.getSize() < 3)
    {
        sendMsg(fd, ERR_NEEDMOREPARAMS(_clients[fd].getNickname(), _host));
        return;
    }

    targets = splitVector(cmd.getCmd(1), ',');
    for (size_t i = 0; i < targets.size(); i++)
    {
        if (targets[i][0] == '#')
        {
            privmsgChannel(fd, cmd, targets[i]);
        } else
        {
            privmsgClient(fd, cmd, targets[i]);
        }
    }
}
