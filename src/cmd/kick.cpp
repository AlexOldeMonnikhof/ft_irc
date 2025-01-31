#include "Server.hpp"
#include "Channel.hpp"
#include "Command.hpp"

void    Server::cmdKick(int fd, Command& cmd)
{
    if (cmd.getSize() < 3)
	{
		sendMsg(fd, ERR_NEEDMOREPARAMS(_clients[fd].getNickname(), _host));
		return;
	}

	size_t i = getChannelIndex(_channels, cmd.getCmd(1));
	std::string target = cmd.getCmd(2);
	if (!channelExist(cmd.getCmd(1)))
	{
		sendMsg(fd, ERR_NOSUCHCHANNEL(_host, cmd.getCmd(2), _clients[fd].getNickname()));
		return;
	}

    Channel& channel = _channels[i];
	if (!channel.clientInChannel(_clients[fd].getNickname()))
	{
		sendMsg(fd, ERR_NOTONCHANNEL(_host, channel.getName(), _clients[fd].getNickname()));
		return;
	}

	if (!channel.isOperator(_clients[fd].getNickname()))
    {
        sendMsg(fd, ERR_CHANOPRIVSNEEDED(_host, _clients[fd].getNickname(), channel.getName()));
		return;
	}

	std::vector<std::string> targets = splitVector(cmd.getCmd(2), ',');
	for (size_t i = 0; i < targets.size(); i++)
	{
		if (!getClientFd(targets[i]))
        {
            sendMsg(fd, ERR_NOSUCHNICK(_host, targets[i]));
            continue;
		}

		if (!channel.clientInChannel(targets[i]))
        {
            sendMsg(fd, ERR_USERNOTINCHANNEL(_host, channel.getName()));
			continue;
		}

        msgChannel(fd, RPL_KICK(_clients[fd].getNickname(), _clients[fd].getUsername(), _host, channel.getName(), targets[i]), channel.getName(), true);
        channel.part(getClientFd(targets[i]), targets[i]);
	}
}
