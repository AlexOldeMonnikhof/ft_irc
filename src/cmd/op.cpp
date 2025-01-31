#include "Server.hpp"
#include "Channel.hpp"
#include "Command.hpp"

void    Server::modeOperator(int fd, Channel& channel, bool adding, Command& cmd)
{
    if (cmd.getSize() < 4)
    {
        sendMsg(fd, ERR_NEEDMOREPARAMS(_clients[fd].getNickname(), _host));
    }

    std::string target = cmd.getCmd(3);
    if (!channel.isOperator(_clients[fd].getNickname()))
    {
        sendMsg(fd, ERR_CHANOPRIVSNEEDED(_host, _clients[fd].getNickname(), channel.getName()));
        return;
    }

    if (!channel.clientInChannel(target))
    {
        sendMsg(fd, ERR_USERNOTINCHANNEL(_host, channel.getName()));
        return;
    }

    if (adding)
    {
        channel.setOperator(target);
    } else
    {
        channel.removeOperator(target);
    }

    std::string mode = (adding ? "+o" : "-o");
    msgChannel(fd, RPL_MODEISOP(channel.getName(), _host, mode, target), channel.getName(), true);
}
