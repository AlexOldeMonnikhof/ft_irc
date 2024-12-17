#include "Server.hpp"

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
