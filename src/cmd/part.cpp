#include "Server.hpp"

void    Server::cmdPart(int fd, Command& cmd)
{
    vector<string>  channels;

    if (cmd.getSize() < 2)
    {
        sendMsg(fd, ERR_NEEDMOREPARAMS(_clients[fd].getNickname(), _host));
        return;
    }
    channels = splitVector(cmd.getCmd(1), ',');
    for (size_t i = 0; i < channels.size(); i++)
    {
        if (channels[i][0] != '#' || !channels[i][1])
        {
            sendMsg(fd, ERR_BADCHANMASK(_host, channels[i]));
            return;
        }
    }
    for (size_t i = 0; i < channels.size(); i++)
    {
        size_t index = getChannelIndex(_channels, channels[i]);
        if (index != string::npos)
        {
            if (_channels[index].clientInChannel(_clients[fd].getNickname()))
                _channels[index].part(fd, _clients[fd].getNickname(), _host, _clients);
            else
                sendMsg(fd, ERR_NOTONCHANNEL(_host, channels[i], _clients[fd].getNickname()));
            if (_channels[index].getClientsSize() == 0)
                _channels.erase(_channels.begin() + index);
        }
        else
            sendMsg(fd, ERR_NOSUCHCHANNEL(_host, channels[i], _clients[fd].getNickname()));
    }
}