#include "Server.hpp"
#include "Client.hpp"
#include "error.hpp"

class Client;

void    Server::parseServer(string port, string password)
{
    stringstream s(port);
    if (!(s >> _port) || _port < 1024 || _port > 6553)
        throw std::runtime_error("Invalid port");
    if (!password.length())
        throw std::runtime_error("Invalid password");
    _password = password;
    _host = "IRCServer";
}

void    Server::initServer()
{
    struct sockaddr_in address;
    int options = 1;

    _socket = socket(AF_INET, SOCK_STREAM, 0);
    if (_socket < 0)
        throw std::runtime_error("Failed to create socket");
	fcntl(_socket, F_SETFL, O_NONBLOCK);
	setsockopt(_socket, SOL_SOCKET, SO_REUSEADDR, &options, sizeof(int));
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(_port);
	if (::bind(_socket, (struct sockaddr *)&address, sizeof(address)) < 0)
		throw std::runtime_error("Failed to bind socket");
	if (listen(_socket, SOMAXCONN) < 0)
		throw std::runtime_error("Failed to listen on socket");
	_fds.push_back((pollfd){_socket, POLLIN, 0});
}

void    Server::addClient()
{
    int newSocket = accept(_socket, NULL, NULL);
    if (newSocket < 0)
        throw std::runtime_error("Failed to accept new client");
    fcntl(newSocket, F_SETFL, O_NONBLOCK);
    _fds.push_back((pollfd){newSocket, POLLIN, 0});
    cout << "ACCEPTED" << endl;
    _clients[newSocket] = Client(newSocket);
}

void    Server::disconnectClient(vector<pollfd>::iterator& iter)
{
    cout << "client disconnected. fd = " << iter->fd << endl;
    _clients.erase(iter->fd);
    close(iter->fd);
    iter--;
    _fds.erase(iter + 1);
}

void    Server::errorMsg(int fd, int error, Command& cmd)
{
    string client = _clients[fd].getNickname(), msg;

    if (error == ERR_NONICKNAMEGIVEN) // 431
        msg = "ERR_NONICKNAMEGIVEN (431)\n\rError: No nickname given\n\r";
    if (error == ERR_ERRONEUSNICKNAME) // 432
        msg = "ERR_ERRONEUSNICKNAME (432)\n\r" + cmd.getCmd(1) + ": Erroneus nickname\n\r";
    if (error == ERR_NICKNAMEINUSE) // 433
        msg = "ERR_NICKNAMEINUSE (433)\n\r" + cmd.getCmd(1) + ": Nickname is already in use\n\r";
    if (error == ERR_NOTREGISTERED) // 451
        msg = "ERR_NOTREGISTERED (451)\n\rError: You have not registered\n\r";
    if (error == ERR_NEEDMOREPARAMS) // 461
        msg = "ERR_NEEDMOREPARAMS (461)\n\r" + cmd.getCmd(0) + ": Not enough parameters\n\r";
    if (error == ERR_ALREADYREGISTERED) // 462
        msg = "ERR_ALREADYREGISTERED (462)\n\rError: You may not reregister\n\r";
    if (error == ERR_PASSWDMISMATCH) // 464
        msg = "ERR_PASSWDMISMATCH (464)\n\rError: Password incorrect\n\r";
    send(fd, msg.c_str(), msg.length(), 0);
}

void    Server::msgPASS(int fd, Command& cmd)
{
    if (_clients[fd].getRegister() & 0b100)
    {
        errorMsg(fd, ERR_NOTREGISTERED, cmd);
        return;
    }
    if (cmd.getSize() < 2)
    {
        errorMsg(fd, ERR_NEEDMOREPARAMS, cmd);
        return;
    }
    if (cmd.getCmd(1) == _password && cmd.getSize() == 2)
        _clients[fd].setRegister(PASSWORD);
    else
    {
        errorMsg(fd, ERR_PASSWDMISMATCH, cmd);
        return;
    }
    if (_clients[fd].getRegister() == 7)
        cout << "client is now registered" << endl;
}


bool    Server::isValidName(string nick)
{
    for (size_t i = 0; i < nick.size(); i++)
    {
        char c = nick[i];
        if (i == 0 && isdigit(nick[i]))
            return false;
        if (!isalnum(c) && c != '[' && c != ']' && c != '{' && c != '}' && c != '\\' && c != '|')
            return false;
    }
    return true;
}

bool    Server::nickInUse(int fd, string nick)
{
    for (vector<pollfd>::iterator iter = _fds.begin() + 1; iter < _fds.end(); iter++)
    {
        if (iter->fd == fd)
            continue;
        if (_clients[iter->fd].getNickname() == nick)
            return true;
    }
    return false;
}

void    Server::msgNICK(int fd, Command& cmd)
{
    if (cmd.getSize() < 2)
    {
        errorMsg(fd, ERR_NONICKNAMEGIVEN, cmd);
        return;
    }
    if (!isValidName(cmd.getCmd(1)))
    {
        errorMsg(fd, ERR_ERRONEUSNICKNAME, cmd);
        return;
    }
    if (nickInUse(fd, cmd.getCmd(1)))
    {
        errorMsg(fd, ERR_NICKNAMEINUSE, cmd);
        return;
    }
    _clients[fd].setNickname(cmd.getCmd(1));
    _clients[fd].setRegister(NICKNAME);
    if (_clients[fd].getRegister() == 7)
        cout << "client is now registered" << endl;
}

void    Server::msgUSER(int fd, Command& cmd)
{
    if (cmd.getSize() < 5)
    {
        errorMsg(fd, ERR_NEEDMOREPARAMS, cmd);
        return ;
    }
    _clients[fd].setUsername(cmd.getCmd(1));
    _clients[fd].setHostname(cmd.getCmd(2));
    _clients[fd].setServername(cmd.getCmd(3));
    _clients[fd].setRealname(cmd.getCmd(4));
    _clients[fd].setRegister(USERNAME);
    if (_clients[fd].getRegister() == 7)
        cout << "client is now registered" << endl;
}

void    Server::registerClient(int fd, Command& cmd)
{
    if (cmd.getCmd(0) == "PASS")
        msgPASS(fd, cmd);
    if (cmd.getCmd(0) == "NICK")
        msgNICK(fd, cmd);
    if (cmd.getCmd(0) == "USER")
        msgUSER(fd, cmd);
}

void    Server::cmdsClient(int fd, Command& cmd)
{
    if (cmd.getCmd(0) == "PASS" || cmd.getCmd(0) == "NICK" || cmd.getCmd(0) == "USER")
    {
        errorMsg(fd, ERR_ALREADYREGISTERED, cmd);
        return;
    }
}

void    Server::handleMsgClient(int fd)
{
    char buffer[BUFFER_LENGTH];
    bzero(buffer, BUFFER_LENGTH);
    ssize_t bytesRecv = recv(fd, buffer, BUFFER_LENGTH - 1, 0);
    Command cmd(buffer);
    if (_clients[fd].getRegister() != 7)
        registerClient(fd, cmd);
    else
        cmdsClient(fd, cmd);
}

void    Server::iterateClientRevents()
{
    for (vector<pollfd>::iterator iter = _fds.begin() + 1; iter < _fds.end(); iter++)
    {
        if (!iter->revents)
            continue;
        if (iter->revents & POLLHUP)
        {
            disconnectClient(iter);
            continue;
        }
        if (iter->revents & POLLIN)
        {
            handleMsgClient(iter->fd);
            continue;
        }
    }
}

void    Server::mainLoop()
{
    while (true)
    {
        poll(_fds.data(), _fds.size(), WAIT_FOREVER);
        if (_fds[0].revents == POLLIN)
        {
            addClient();
        }
        iterateClientRevents();
    }
}

Server::Server(string port, string password)
{
    parseServer(port, password);
    initServer();
    cout << _host << " listening on port " << _port << " with password " << _password << endl;
    mainLoop();
}
