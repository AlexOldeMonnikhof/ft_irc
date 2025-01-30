#include "Server.hpp"
#include "Channel.hpp"

void    Server::modeUserLimit(int fd, Channel& channel, bool adding, Command& cmd)
{
    int limit = 0;
    if (adding)
    {
        if (cmd.getSize() < 4)
            sendMsg(fd, ERR_NEEDMOREPARAMS(_clients[fd].getNickname(), _host));
        else
        {
            limit = atoi(cmd.getCmd(3).c_str());
            if (limit && (limit > 0 && limit < 1000))
            {
                channel.setUserLimit(limit);
                msgChannel(fd, RPL_MODEISLIMIT(channel.getName(), _host, "+l", cmd.getCmd(3)), channel.getName(), true);
            }
            else
            {
                channel.setUserLimit(0);
                msgChannel(fd, RPL_MODEISLIMIT(channel.getName(), _host, "-l", "0"), channel.getName(), true);
            }
        }
    }
    else
    {
        channel.setUserLimit(0);
        msgChannel(fd, RPL_MODEISLIMIT(channel.getName(), _host, "-l", "0"), channel.getName(), true);
    }
}