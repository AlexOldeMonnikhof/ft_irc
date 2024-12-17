#include "Server.hpp"

//IMPLEMENT RPL_NICKCHANGE
void    Server::cmdNICK(int fd, Command& cmd)
{
    if (cmd.getSize() < 2)
    {
        sendMsg(fd, ERR_NONICKNAMEGIVEN(_clients[fd].getNickname(), _host));
        return;
    }
    if (!isValidName(cmd.getCmd(1)))
    {
        sendMsg(fd, ERR_ERRONEUSNICKNAME(cmd.getCmd(1), _host));
        return;
    }
    if (nickInUse(fd, cmd.getCmd(1)))
    {
        sendMsg(fd, ERR_NICKNAMEINUSE(cmd.getCmd(1), _host));
        return;
    }
    _clients[fd].setNickname(cmd.getCmd(1));
    _clients[fd].setRegister(NICKNAME);
}