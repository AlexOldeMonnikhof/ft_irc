#include "Server.hpp"

void    Server::cmdJoin(int fd, Command& cmd)
{
    vector<string>  channels, passwords;
    size_t j = 0;
    if (cmd.getSize() < 2)
    {
        sendMsg(fd, ERR_NEEDMOREPARAMS(_clients[fd].getNickname(), _host));
        return;
    }
    channels = splitVector(cmd.getCmd(1), ',');
    if (cmd.getSize() == 3)
        passwords = splitVector(cmd.getCmd(2), ',');
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
            if (!_channels[index].getPassword().empty())
            {
                if (passwords.empty() || j >= channels.size() || _channels[index].getPassword() != passwords[j++])
                {
                    sendMsg(fd, ERR_BADCHANNELKEY(_clients[fd].getNickname(), _host, channels[i]));
                    continue;
                }
            }
            if (_channels[index].getInviteOnly() && !_channels[index].isUserInvited(_clients[fd].getNickname()))
            {
                sendMsg(fd, ERR_INVITEONLY(_clients[fd].getNickname(), channels[i]));
                continue;
            }
            _channels[index].join(fd, _clients[fd].getNickname());
        }
        else
            _channels.push_back(Channel(fd, _clients[fd].getNickname(), channels[i]));
    }
}
