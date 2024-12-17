#include "Server.hpp"



//ERR_NOSUCHCHANNEL (403) ELSEWHERE
//ERR_CHANNELISFULL (471) LATER
//ERR_INVITEONLYCHAN (473) LATER
//RPL_TOPIC (332) LATER
//RPL_TOPICWHOTIME (333) LATER
//RPL_NAMREPLY (353) LATER
//RPL_ENDOFNAMES (366) LATER


//IMPLEMENT THIS
void    Server::privmsgChannel(int fd, Command& cmd, string channel)
{
    if (!channelExist(channel))
    {
        sendMsg(fd, ERR_NOSUCHCHANNEL(_host, channel, _clients[fd].getNickname()));
        return;
    }
    else
    {
        size_t index = getChannelIndex(_channels, channel);
        size_t clientSize = _channels[index].getClientsSize();
        cout << "size: " << clientSize << endl;
        vector<string>  clients = _channels[index].getClients();
        for (size_t i = 0; i < clientSize; i++)
        {
            sendMsg(getClientFd(clients[i]), "PRIVMSG " + channel + " :" + cmd.getCmd(2) + "\r\n");
            cout << "sending message to " << clients[i] << endl;
        }
    }
}

void    Server::privmsgClient(int fd, Command& cmd, string nick)
{
    int targetFd = getClientFd(nick);
    if (!targetFd)
        sendMsg(fd, ERR_NOSUCHNICK(_host, nick));
    else
        sendMsg(targetFd, "PRIVMSG " + nick + " :" + cmd.getCmd(2) + "\r\n"); // CHANGE THIS WITH RIGHT MESSAGE
}

// ERR_NOSUCHNICK (401)
// ERR_CANNOTSENDTOCHAN (404)
// ERR_NOSUCHCHANNEL (403)
// ERR_NOTEXTTOSEND (412)
void    Server::cmdPrivmsg(int fd, Command& cmd)
{
    vector<string>  targets;
    if (cmd.getSize() < 3)
    {
        sendMsg(fd, ERR_NEEDMOREPARAMS(_clients[fd].getNickname(), _host));
        return;
    }
    targets = splitVector(cmd.getCmd(1), ',');
    for (size_t i = 0; i < targets.size(); i++)
    {
        if (targets[i][0] == '#')
            privmsgChannel(fd, cmd, targets[i]);
        else
            privmsgClient(fd, cmd, targets[i]);
    }
}
