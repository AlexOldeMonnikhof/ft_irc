#include "Server.hpp"
#include "Client.hpp"

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

void    Server::handleMsgClient()
{
    cout << "message send by client" << endl;
}

void    Server::iterateClientRevents()
{
    for (vector<pollfd>::iterator iter = _fds.begin() + 1; iter < _fds.end(); iter++)
    {
        if (!iter->revents)
            continue ;
        if ((iter->revents & POLLHUP) == POLLHUP)
        {
            disconnectClient(iter);
            cout << "iter points to fd " << iter->fd << endl;
            continue ;
        }
        if (((iter->revents & POLLIN) == POLLIN))
        {
            handleMsgClient();
            iter->revents = 0;
        }
    }
}
//iter->revent= begin->rev = 0 && iter-.rev = iter->beg+1

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
