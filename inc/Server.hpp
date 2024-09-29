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

using namespace std;

class Server{
    private:
        int     _port;
        string  _password;
        string  _host;
    public:
        Server(string port, string password);
        void initServer();
};

#endif