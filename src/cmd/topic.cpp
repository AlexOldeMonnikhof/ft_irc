#include "Server.hpp"
#include "error.hpp"

void    Server::cmdTopic(int fd, Command& cmd)
{
	if (cmd.getSize() < 2 ||( cmd.getCmd(1).size() >= 1 && cmd.getCmd(1)[0] != '#'))
	{
		sendMsg(fd, ERR_NEEDMOREPARAMS(_clients[fd].getNickname(), _host));
		return;
	}

    size_t i = getChannelIndex(_channels, cmd.getCmd(1));
    std::string channelName = cmd.getCmd(1);
	if (!channelExist(channelName))
	{
		sendMsg(fd, ERR_NOSUCHCHANNEL(_host, channelName, _clients[fd].getNickname()));
		return;
	}

    if (!_channels[i].clientInChannel(_clients[fd].getNickname()))
    {
        sendMsg(fd, ERR_NOTONCHANNEL(_host, channelName, _clients[fd].getNickname()));
        return;
    }

	if (cmd.getSize() == 2)
	{
		sendMsg(fd, RPL_TOPIC(_clients[fd].getNickname(), _host, channelName, _channels[i].getTopic()));
		return;
	}

	if (_channels[i].topicOnlyOperators() && !_channels[i].isOperator(_clients[fd].getNickname()))
	{
		sendMsg(fd, ERR_CHANOPRIVSNEEDED(_clients[fd].getNickname(), _host, channelName));
		return;
	}

	_channels[i].setTopic(cmd.getCmd(2));
	msgChannel(fd, RPL_SETTOPIC(_clients[fd].getNickname(), _host, channelName, cmd.getCmd(2)), channelName, true);
}

void    Server::modeTopic(int fd, Channel &channel, bool adding)
{
    if (!channel.isOperator(_clients[fd].getNickname()))
    {
        sendMsg(fd, ERR_CHANOPRIVSNEEDED(_host, _clients[fd].getNickname(), channel.getName()));
        return;
    }

    channel.setTopicOnlyOperators(adding);
    std::string mode = (adding ? "+t" : "-t");
    msgChannel(fd, RPL_CHANNELMODEIS(channel.getName(), _host, mode, ""), channel.getName(), true);
}
