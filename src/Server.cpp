#include "Server.hpp"
#include "Client.hpp"
#include "error.hpp"
#include "headers.hpp"

class Client;

//UTIL
void    sendMsg(int fd, string msg)
{
    send(fd, msg.c_str(), msg.length(), 0);
}

bool    Server::channelExist(string channel)
{
    for (size_t i = 0; i < _channels.size(); i++)
    {
        if (_channels[i].getName() == channel)
            return true;
    }
    return false;
}

int     Server::getClientFd(string nick)
{
    for (map<int, Client>::iterator iter = _clients.begin(); iter != _clients.end(); iter++)
    {
        if (iter->second.getNickname() == nick)
            return iter->first;
    }
    return 0;
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

void    Server::disconnectClient(vector<pollfd>::iterator& iter)
{
    cout << "client disconnected. fd = " << iter->fd << endl;
    _clients.erase(iter->fd);
    close(iter->fd);
    iter--;
    _fds.erase(iter + 1);
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

void    Server::registerClient(int fd, Command& cmd)
{
    if (cmd.getCmd(0) == "PASS")
        msgPASS(fd, cmd);
    else if (!(_clients[fd].getRegister() & 0b100))
    {
        sendMsg(fd, ERR_NOTREGISTERED(_clients[fd].getNickname(), _host));
        return;
    }
    else if (cmd.getCmd(0) == "NICK")
        msgNICK(fd, cmd);
    else if (!(_clients[fd].getRegister() & 0b10))
    {
        sendMsg(fd, ERR_NOTREGISTERED(_clients[fd].getNickname(), _host));
        return;
    }
    else if (cmd.getCmd(0) == "USER")
        msgUSER(fd, cmd);
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

void    Server::cmdsClient(int fd, Command& cmd)
{
    if (cmd.getCmd(0) == "PASS" || cmd.getCmd(0) == "NICK" || cmd.getCmd(0) == "USER")
    {
        sendMsg(fd, ERR_ALREADYREGISTERED(_clients[fd].getNickname(), _host));
        return;
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
    Command cmd(buffer);
    if (_clients[fd].getRegister() != 7)
        registerClient(fd, cmd);
    else
        cmdsClient(fd, cmd);
}

void    Server::mainLoop()
{
    while (true)
    {
        poll(_fds.data(), _fds.size(), WAIT_FOREVER);
        if (_fds[0].revents == POLLIN)
            addClient();
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
}

Server::Server(string port, string password)
{
    parseServer(port, password);
    initServer();
    cout << _host << " listening on port " << _port << " with password " << _password << endl;
    mainLoop();
}
