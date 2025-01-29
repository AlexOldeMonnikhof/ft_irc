#include "Server.hpp"

void    Server::msgChannel(int fd, string msg, string channel)
{
    size_t index = getChannelIndex(_channels, channel);
    size_t clientSize = _channels[index].getClientsSize();
    vector<string>  clients = _channels[index].getClients();
    for (size_t i = 0; i < clientSize; i++)
    {
        string nick = _clients[fd].getNickname();
        sendMsg(getClientFd(clients[i]), msg);
    }
}

void    Server::privmsgChannel(int fd, Command& cmd, string channel)
{
    if (!channelExist(channel))
    {
        sendMsg(fd, ERR_NOSUCHCHANNEL(_host, channel, _clients[fd].getNickname()));
        return;
    }
    else
    {
        size_t index = getChannelIndex(_channels, channel);
        size_t clientSize = _channels[index].getClientsSize();
        vector<string>  clients = _channels[index].getClients();
        for (size_t i = 0; i < clientSize; i++)
        {
            string nick = _clients[fd].getNickname();
            if (_channels[index].isOperator(nick))
                nick += " -OP-";
            sendMsg(getClientFd(clients[i]), nick + " (PRIVMSG " + channel + "): " + cmd.getCmd(2) + "\r\n");
        }
    }
}

void    Server::privmsgClient(int fd, Command& cmd, string nick)
{
    int targetFd = getClientFd(nick);
    if (!targetFd)
        sendMsg(fd, ERR_NOSUCHNICK(_host, nick));
    else
        sendMsg(targetFd, "PRIVMSG " + nick + " :" + cmd.getCmd(2) + "\r\n"); // CHANGE THIS WITH RIGHT MESSAGE
}

void    Server::cmdPrivmsg(int fd, Command& cmd)
{
    vector<string>  targets;
    if (cmd.getSize() < 3)
    {
        sendMsg(fd, ERR_NEEDMOREPARAMS(_clients[fd].getNickname(), _host));
        return;
    }
    targets = splitVector(cmd.getCmd(1), ',');
    for (size_t i = 0; i < targets.size(); i++)
    {
        if (targets[i][0] == '#')
            privmsgChannel(fd, cmd, targets[i]);
        else
            privmsgClient(fd, cmd, targets[i]);
    }
}
