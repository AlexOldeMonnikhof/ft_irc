#include "Server.hpp"
#include "Command.hpp"

void    Server::cmdPASS(int fd, Command& cmd)
{
    if (_clients[fd].getRegister() & 0b100)
    {
        sendMsg(fd, ERR_NOTREGISTERED(_clients[fd].getNickname(), _host));
        return;
    }

    if (cmd.getSize() < 2)
    {
        sendMsg(fd, ERR_NEEDMOREPARAMS(_clients[fd].getNickname(), _host));
        return;
    }

    if (cmd.getCmd(1) == _password && cmd.getSize() == 2)
    {
        _clients[fd].setRegister(PASSWORD);
    } else
    {
        sendMsg(fd, ERR_PASSWDMISMATCH(_clients[fd].getNickname(), _host));
        return;
    }
}
