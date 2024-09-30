#ifndef CLIENT_HPP
#define CLIENT_HPP

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

#define FOREVER -1

#define NICKNAME 0
#define USERNAME 1
#define PASSWORD 2

using namespace std;

class Client{
    private:
        int             _socket;
        int             _register;
        string          _nickname;
        string          _username;
        string          _host;
    public:
        Client(){};
        Client(int socket);
        int     getRegister() const;
        void    setRegister(int value);
};

#endif