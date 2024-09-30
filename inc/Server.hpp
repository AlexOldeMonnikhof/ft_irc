#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <sstream>
#include <fcntl.h>
#include <unistd.h>
#include <netinet/in.h>
#include <vector>
#include <poll.h>
#include <map>

#include "Client.hpp"

#define WAIT_FOREVER -1

#define BUFFER_LENGTH 2048

using namespace std;

class Client;

class Server{
    private:
        int                 _port;
        int                 _socket;
        string              _password;
        string              _host;
        vector<pollfd>      _fds;
        map<int, Client>    _clients;
    public:
        Server(string port, string password);
        void    parseServer(string port, string password);
        void    initServer();
        void    addClient();
        void    disconnectClient(vector<pollfd>::iterator& iter);
        string  parseBuffer(string buffer);
        void    registerClient(string buffer);
        void    handleMsgClient(int fd);
        void    iterateClientRevents();
        void    mainLoop();
};

#endif