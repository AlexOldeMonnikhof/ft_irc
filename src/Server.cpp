#include <cstring>
#include <fcntl.h>
#include <iostream>
#include <netinet/in.h>
#include <sstream>
#include <sys/socket.h>
#include <unistd.h>

#include "Server.hpp"
#include "error.hpp"

Server::Server(const std::string &port, const std::string &password)
{
    parseServer(port, password);
    initServer();
    std::cout << _host << " listening on port " << _port << " with password " << _password << '\n';
    mainLoop();
}

Server::Server(const Server &rhs) :
    _port(rhs._port),
    _socket(rhs._socket),
    _password(rhs._password),
    _host(rhs._host),
    _fds(rhs._fds),
    _clients(rhs._clients),
    _channels(rhs._channels)
{}

Server &Server::operator=(const Server &rhs)
{
    if (this != &rhs) {
        _port = rhs._port;
        _socket = rhs._socket;
        _password = rhs._password;
        _host = rhs._host;
        _fds = rhs._fds;
        _clients = rhs._clients;
        _channels = rhs._channels;
    }

    return *this;
}

Server::~Server() {}

void    sendMsg(int fd, const std::string &msg)
{
    send(fd, msg.c_str(), msg.length(), 0);
}

void    Server::msgAllClients(const std::string &msg)
{
    for (std::map<int, Client>::iterator iter = _clients.begin(); iter != _clients.end(); iter++)
    {
        sendMsg(iter->first, msg);
    }
}

void    Server::parseServer(const std::string &port, const std::string &password)
{
    std::stringstream s(port);
    if (!(s >> _port) || _port < 1024 || _port > 6553)
    {
        throw std::runtime_error("Invalid port");
    }

    if (!password.length())
    {
        throw std::runtime_error("Invalid password");
    }

    _password = password;
    _host = "IRCServer";
}

void    Server::initServer()
{
    struct sockaddr_in address;
    int options = 1;

    _socket = socket(AF_INET, SOCK_STREAM, 0);
    if (_socket < 0)
    {
        throw std::runtime_error("Failed to create socket");
    }

	fcntl(_socket, F_SETFL, O_NONBLOCK);
	setsockopt(_socket, SOL_SOCKET, SO_REUSEADDR, &options, sizeof(int));
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(_port);
	if (::bind(_socket, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
		throw std::runtime_error("Failed to bind socket");
    }

	if (listen(_socket, SOMAXCONN) < 0)
    {
		throw std::runtime_error("Failed to listen on socket");
    }

	_fds.push_back((pollfd){_socket, POLLIN, 0});
}

void    Server::addClient()
{
    int newSocket = accept(_socket, NULL, NULL);
    if (newSocket < 0)
    {
        throw std::runtime_error("Failed to accept new client");
    }

    fcntl(newSocket, F_SETFL, O_NONBLOCK);
    _fds.push_back((pollfd){newSocket, POLLIN, 0});
    _clients[newSocket] = Client(newSocket);
}

void    Server::disconnectClient(int fd)
{
    std::cout << "Client disconnected. fd = " << fd << '\n';
    for (std::vector<pollfd>::iterator it = _fds.begin(); it != _fds.end(); ++it)
    {
        if (it->fd == fd)
        {
            _fds.erase(it);
            break;
        }
    }

    _clients.erase(fd);
    close(fd);
}

void    Server::registerClient(int fd, Command& cmd)
{
    if (cmd.getCmd(0) == "PASS")
    {
        cmdPASS(fd, cmd);
    } else if (!(_clients[fd].getRegister() & 0b100))
    {
        sendMsg(fd, ERR_NOTREGISTERED(_clients[fd].getNickname(), _host));
        return;
    } else if (cmd.getCmd(0) == "NICK")
    {
        cmdNICK(fd, cmd);
    } else if (!(_clients[fd].getRegister() & 0b10))
    {
        sendMsg(fd, ERR_NOTREGISTERED(_clients[fd].getNickname(), _host));
        return;
    } else if (cmd.getCmd(0) == "USER")
    {
        cmdUSER(fd, cmd);
        if (_clients[fd].getRegister() == VALID_REGISTER)
        {
		    sendMsg(fd, RPL_WELCOME(_clients[fd].getNickname(), _host));
		    sendMsg(fd, RPL_YOURHOST(_clients[fd].getNickname(), _host));
		    sendMsg(fd, RPL_CREATED(_clients[fd].getNickname(), _host));
		    sendMsg(fd, RPL_MYINFO(_clients[fd].getNickname(), _host));
        }
    }
}

std::vector<std::string> splitVector(const std::string &s, char delimiter)
{
    std::vector<std::string> tokens;
    std::stringstream ss(s);
    std::string token;
    while (getline(ss, token, delimiter))
    {
        tokens.push_back(token);
    }

    return tokens;
}

void    Server::cmdsClient(int fd, Command& cmd)
{
    if (cmd.getCmd(0) == "PASS" || cmd.getCmd(0) == "NICK" || cmd.getCmd(0) == "USER")
    {
        if (_clients[fd].getRegister() != VALID_REGISTER)
        {
            registerClient(fd, cmd);
        } else
        {
            sendMsg(fd, ERR_ALREADYREGISTERED(_clients[fd].getNickname(), _host));
        }
        return;
    } else if (_clients[fd].getRegister() != VALID_REGISTER)
    {
        sendMsg(fd, ERR_NOTREGISTERED(_clients[fd].getNickname(), _host));
        return;
    }

    if (cmd.getCmd(0) == "JOIN")
    {
        cmdJoin(fd, cmd);
    } else if (cmd.getCmd(0) == "PART")
    {
        cmdPart(fd, cmd);
    } else if (cmd.getCmd(0) == "PRIVMSG")
    {
        cmdPrivmsg(fd, cmd);
    } else if (cmd.getCmd(0) == "MODE")
    {
        cmdMode(fd, cmd);
    } else if (cmd.getCmd(0) == "INVITE")
    {
        cmdInvite(fd, cmd);
    } else if (cmd.getCmd(0) == "TOPIC")
    {
        cmdTopic(fd, cmd);
    } else if (cmd.getCmd(0) == "KICK")
    {
        cmdKick(fd, cmd);
    } else if (cmd.getCmd(0) == "QUIT")
    {
        cmdQuit(fd, cmd);
    } else
    {
        sendMsg(fd, ERR_UNKNOWNCOMMAND(_host, _clients[fd].getNickname(), cmd.getCmd(0)));
    }
}

bool    checkIfHexChat(const std::string &str)
{
    if (str.find("CAP") != std::string::npos)
    {
        return true;
    }

    return false;
}

void    Server::handleHexChatRegister(int fd, const std::string &buffer)
{
    std::string buff = buffer;
    std::size_t pos = buff.find("PASS");
    if (pos == std::string::npos)
    {
        return;
    }

    std::size_t endpos = buff.find("\r\n", pos);
    std::string pass = buff.substr(pos, endpos - pos);
    Command Pass(pass);
    cmdPASS(fd, Pass);
    buff = buffer;
    pos = buff.find("NICK");
    if (pos == std::string::npos)
    {
        return;
    }

    endpos = buff.find("\r\n", pos);
    std::string nick = buff.substr(pos, endpos - pos);
    Command Nick(nick);
    cmdNICK(fd, Nick);
    buff = buffer;
    pos = buff.find("USER");
    if (pos == std::string::npos)
    {
        return;
    }

    std::string user = buff.substr(pos);
    Command User(user);
    cmdUSER(fd, User);
    if (_clients[fd].getRegister() == VALID_REGISTER)
    {
        sendMsg(fd, RPL_WELCOME(_clients[fd].getNickname(), _host));
        sendMsg(fd, RPL_YOURHOST(_clients[fd].getNickname(), _host));
        sendMsg(fd, RPL_CREATED(_clients[fd].getNickname(), _host));
        sendMsg(fd, RPL_MYINFO(_clients[fd].getNickname(), _host));
    }
}

void Server::handleMsgClient(int fd)
{
    static std::map<int, std::string> partial;
    char buffer[BUFFER_LENGTH + 1];
    memset(buffer, 0, sizeof(buffer));
    ssize_t bytesRecv = recv(fd, buffer, BUFFER_LENGTH, 0);
    if (bytesRecv <= 0)
    {
        partial.erase(fd);
        disconnectClient(fd);
        return;
    }

    size_t pos = 0;
    partial[fd].append(buffer, bytesRecv);
    while ((pos = partial[fd].find('\n')) != std::string::npos)
    {
        std::string line = partial[fd].substr(0, pos + 1);
        partial[fd].erase(0, pos + 1);
        if (checkIfHexChat(line))
        {
            handleHexChatRegister(fd, line);
        } else
        {
            Command cmd(line);
            if (!cmd.getV().empty())
                cmdsClient(fd, cmd);
        }
    }
}

void    Server::mainLoop()
{
    while (true)
    {
        if (poll(_fds.begin().base(), _fds.size(), WAIT_FOREVER) < 0)
        {
            throw std::runtime_error("Error: poll failed");
        }
        for (size_t i = 0; i < _fds.size(); i++)
        {
            if (_fds[i].revents == 0)
            {
                continue;
            }
            if ((_fds[i].revents & POLLIN) == POLLIN)
            {
                if (_fds[i].fd == _socket)
                {
                   std::cout << "New client connected" << '\n';
                    addClient();
                } else
                {
                    handleMsgClient(_fds[i].fd);
                }
            }
        }
    }
}
