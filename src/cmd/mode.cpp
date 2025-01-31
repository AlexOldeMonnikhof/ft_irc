#include "Server.hpp"
#include "Channel.hpp"

void Server::cmdMode(int fd, Command& cmd)
{
    if (cmd.getSize() < 3)
    {
        sendMsg(fd, ERR_NEEDMOREPARAMS(_clients[fd].getNickname(), _host));
        return;
    }

    std::string channel = cmd.getCmd(1);
    if (!channelExist(channel))
    {
        sendMsg(fd, ERR_NOSUCHCHANNEL(_host, channel, _clients[fd].getNickname()));
        return;
    }

    size_t i = getChannelIndex(_channels, channel);
	if (!_channels[i].clientInChannel(_clients[fd].getNickname()))
	{
		sendMsg(fd, ERR_NOTONCHANNEL(_host, _channels[i].getName(), _clients[fd].getNickname()));
		return;
	}

    if (!_channels[i].isOperator(_clients[fd].getNickname()))
    {
        sendMsg(fd, ERR_CHANOPRIVSNEEDED(_host, _clients[fd].getNickname(), channel));
        return;
    }

    bool adding = false;
    std::string mode = cmd.getCmd(2);
    if (mode[0] == '+')
    {
        adding = true;
    } else if (mode[0] == '-')
    {
        adding = false;
    } else
    {
        std::cout << "invalid mode: " << mode << '\n';
        return ;
    }

    size_t j = 0;
    while (mode[++j])
    {
        if (mode[j] == 'i')
        {
            modeInviteOnly(fd, _channels[i], adding);
        } else if (mode[j] == 'o')
        {
            modeOperator(fd, _channels[i], adding, cmd);
        } else if (mode[j] == 'k')
        {
            modeKey(fd, _channels[i], adding, cmd);
        } else if (mode[j] == 't')
        {
            modeTopic(fd, _channels[i], adding);
        } else if (mode[j] == 'l')
        {
            modeUserLimit(fd, _channels[i], adding, cmd);
        }
    }
}
