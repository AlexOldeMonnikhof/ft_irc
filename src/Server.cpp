#include "Server.hpp"

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
    int socketFd, options = 1;
    struct sockaddr_in address;

    socketFd = socket(AF_INET, SOCK_STREAM, 0);
    if (socketFd < 0)
        throw std::runtime_error("Failed to create socket");
	fcntl(socketFd, F_SETFL, O_NONBLOCK);
	setsockopt(socketFd, SOL_SOCKET, SO_REUSEADDR, &options, sizeof(int));
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(_port);
	if (bind(socketFd, (struct sockaddr *)&address, sizeof(address)) < 0)
		throw std::runtime_error("Failed to bind socket");
	if (listen(socketFd, SOMAXCONN) < 0)
		throw std::runtime_error("Failed to listen on socket");
	_fds.push_back((pollfd){socketFd, POLLIN, 0});
}

void    Server::addClient()
{

}

void    Server::mainLoop()
{
    while (true)
    {
        poll(_fds.data(), _fds.size(), FOREVER);
        if (_fds[0].revents == POLLIN)
        {
            addClient();
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
