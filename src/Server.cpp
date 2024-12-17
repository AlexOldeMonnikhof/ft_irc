#include "Server.hpp"
#include "Client.hpp"
#include "error.hpp"
#include "headers.hpp"

class Client;

void    sendMsg(int fd, string msg)
{
    send(fd, msg.c_str(), msg.length(), 0);
}

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
    _clients[newSocket] = Client(newSocket);
}

void    Server::disconnectClient(int fd)
{
    cout << "client disconnected. fd = " << fd << endl;
    _clients.erase(fd);
    close(fd);
}

void    Server::registerClient(int fd, Command& cmd)
{
    if (cmd.getCmd(0) == "PASS")
        cmdPASS(fd, cmd);
    else if (!(_clients[fd].getRegister() & 0b100))
    {
        sendMsg(fd, ERR_NOTREGISTERED(_clients[fd].getNickname(), _host));
        return;
    }
    else if (cmd.getCmd(0) == "NICK")
        cmdNICK(fd, cmd);
    else if (!(_clients[fd].getRegister() & 0b10))
    {
        sendMsg(fd, ERR_NOTREGISTERED(_clients[fd].getNickname(), _host));
        return;
    }
    else if (cmd.getCmd(0) == "USER")
        cmdUSER(fd, cmd);
}

vector<string> splitVector(const string &s, char delimiter)
{
    vector<string> tokens;
    stringstream ss(s);
    string token;
    while (getline(ss, token, delimiter))
        tokens.push_back(token);
    return tokens;
}

void    Server::cmdsClient(int fd, Command& cmd)
{
    if (cmd.getCmd(0) == "PASS" || cmd.getCmd(0) == "NICK" || cmd.getCmd(0) == "USER")
    {
        if (_clients[fd].getRegister() == 7)
            sendMsg(fd, ERR_ALREADYREGISTERED(_clients[fd].getNickname(), _host));
        else
            registerClient(fd, cmd);
    }
    if (cmd.getCmd(0) == "JOIN")
        cmdJoin(fd, cmd);
    if (cmd.getCmd(0) == "PART")
        cmdPart(fd, cmd);
    if (cmd.getCmd(0) == "PRIVMSG")
        cmdPrivmsg(fd, cmd);
    if (cmd.getCmd(0) == "hello")
    {
        cout << "all channels and their clients" << endl;
        {
            for (size_t i = 0; i < _channels.size(); i++)
            {
                cout << "channel " << _channels[i].getName() << endl;
                _channels[i].printClients();
            }
        }
    }
}

void    Server::handleMsgClient(int fd)
{
    char buffer[BUFFER_LENGTH];
    bzero(buffer, BUFFER_LENGTH);
    ssize_t bytesRecv = recv(fd, buffer, BUFFER_LENGTH - 1, 0);
    if (bytesRecv <= 0 || fd == -1)
    {
        cout << "client disconnected" << endl;
        disconnectClient(fd);
        return ;
    }
    cout << "bytes" << bytesRecv << endl;
    buffer[BUFFER_LENGTH] = '\0';
    Command cmd(buffer);
    if (cmd.getV().empty())
    {
        cout << "empty" << endl;
        return;
    }
    cmdsClient(fd, cmd);
}

void    Server::mainLoop()
{
    while (true)
    {
        //protect
        poll(_fds.data(), _fds.size(), WAIT_FOREVER);
        for (size_t i = 0; i < _fds.size(); i++)
        {
            if (_fds[i].revents & POLLIN)
            {
                if (_fds[i].fd == _socket)
                {
                    cout << "adding client" << endl;
                    addClient();
                }
                else
                {
                    cout << "handling msg client" << endl;
                    handleMsgClient(_fds[i].fd);
                }
            }
        }
    }
}

Server::Server(string port, string password)
{
    parseServer(port, password);
    initServer();
    cout << _host << " listening on port " << _port << " with password " << _password << endl;
    mainLoop();
}
