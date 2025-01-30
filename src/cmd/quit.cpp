#include "Server.hpp"
#include "Client.hpp"

//quit function
void    Server::cmdQuit(int fd, Command& cmd)
{
    if (cmd.getSize() < 2)
    {
        sendMsg(fd, ERR_NEEDMOREPARAMS(_clients[fd].getNickname(), _host));
        return;
    }
    msgAllClients(RPL_QUIT(_host, _clients[fd].getNickname(), _clients[fd].getUsername(), cmd.getCmd(1)));
    disconnectClient(fd);
}    
