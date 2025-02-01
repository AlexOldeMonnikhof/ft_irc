#include "Server.hpp"
#include "error.hpp"

void    Server::modeKey(int fd, Channel& channel, bool adding, Command& cmd)
{
    if (adding)
    {
        if (cmd.getSize() < 4)
        {
            sendMsg(fd, ERR_NEEDMOREPARAMS(_clients[fd].getNickname(), _host));
        } else
        {
            channel.setPassword(cmd.getCmd(3));
            msgChannel(fd, RPL_MODEIS(channel.getName(), _host, "+k " + cmd.getCmd(3)), channel.getName(), true);
        }
    } else
    {
        channel.setPassword("");
        msgChannel(fd, RPL_MODEIS(channel.getName(), _host, "-k"), channel.getName(), true);
    }
}
