#include "Server.hpp"

void    Server::cmdUSER(int fd, Command& cmd)
{
    if (cmd.getSize() < 5)
    {
        sendMsg(fd, ERR_NEEDMOREPARAMS(_clients[fd].getNickname(), _host));
        return ;
    }
    _clients[fd].setUsername(cmd.getCmd(1));
    _clients[fd].setHostname(cmd.getCmd(2));
    _clients[fd].setServername(cmd.getCmd(3));
    _clients[fd].setRealname(cmd.getCmd(4));
    _clients[fd].setRegister(USERNAME);
    cout << "client is now registered" << endl;
}