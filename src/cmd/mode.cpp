#include "Server.hpp"
#include "Channel.hpp"

void Server::cmdMode(int fd, Command& cmd)
{
    if (cmd.getSize() < 3)
    {
        sendMsg(fd, ERR_NEEDMOREPARAMS(_clients[fd].getNickname(), _host));
        return;
    }
    string channel = cmd.getCmd(1);
    if (!channelExist(channel))
    {
        sendMsg(fd, ERR_NOSUCHCHANNEL(_host, channel, _clients[fd].getNickname()));
        return;
    }
    size_t i = getChannelIndex(_channels, channel);
    if (!_channels[i].isOperator(_clients[fd].getNickname()))
    {
        sendMsg(fd, ERR_CHANOPRIVSNEEDED(_host, _clients[fd].getNickname(), channel));
        return;
    }
    bool adding = false;
    string mode = cmd.getCmd(2);
    if (mode[0] == '+')
        adding = true;
    else if (mode[0] == '-')
        adding = false;
    else
    {
        cout << "invalid mode: " << mode << endl;
        return ;
    }
    // if (target[1] == 'o')
    //     modeOperator(_channels[index], adding, cmd);
    if (mode[1] == 'i')
        modeInviteOnly(fd, _channels[i], adding);
    // else if (target[1] == 'k')
    //     modeKey(_channels[index], adding, cmd);
    // else if (target[1] == 't')
    //     modeTopic(_channels[index], adding);
    // else if (target[1] == 'l')
    //     modeLimit(_channels[index], adding, cmd);
}
