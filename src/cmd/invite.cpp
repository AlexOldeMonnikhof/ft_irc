#include "Server.hpp"
#include "Channel.hpp"

void    Server::cmdInvite(int fd, Command& cmd)
{
    if (cmd.getSize() < 3)
    {
        sendMsg(fd, ERR_NEEDMOREPARAMS(_clients[fd].getNickname(), _host));
        return;
    }
    string nick = cmd.getCmd(1);
    string channelName = cmd.getCmd(2);
    size_t index = getChannelIndex(_channels, channelName);
    int targetFd = getClientFd(nick);
    if (!targetFd)
    {
        sendMsg(fd, ERR_NOSUCHNICK(_host, nick));
        return;
    }
    if (!channelExist(channelName))
    {
        sendMsg(fd, ERR_NOSUCHCHANNEL(_host, channelName, _clients[fd].getNickname()));
        return;
    }
    if (_channels[index].clientInChannel(nick))
    {
        sendMsg(fd, ERR_USERONCHANNEL(_host, channelName, nick));
        return;
    }
    if (!_channels[index].clientInChannel(_clients[fd].getNickname()))
    {
        sendMsg(fd, ERR_NOTONCHANNEL(_host, channelName, _clients[fd].getNickname()));
        return;
    }
    if (!_channels[index].isOperator(_clients[fd].getNickname()))
    {
        sendMsg(fd, ERR_CHANOPRIVSNEEDED(_host, _clients[fd].getNickname(), channelName));
        return;
    }
    _channels[index].inviteUser(nick);
    sendMsg(fd, RPL_INVITING(_host, _clients[fd].getNickname(), nick, channelName));
    sendMsg(targetFd, RPL_INVITE(_clients[fd].getNickname(), _clients[fd].getUsername(), _host, nick, channelName));
    cout << "User " << _clients[fd].getNickname() << " invited " << nick << " to channel " << channelName << endl;
}

void Server::modeInviteOnly(int fd, Channel& channel, bool adding)
{
    channel.setInviteOnly(adding);
    string mode;
    if (adding)
        mode = "+i";
    else
        mode = "-i";
    sendMsg(fd, RPL_CHANNELMODEIS(channel.getName(), _host, mode, ""));
    if (adding)
        cout << "channel " << channel.getName() << " is now invite only: " << endl;
    else
        cout << "channel " << channel.getName() << " is no longer invite only: " << endl;
}
