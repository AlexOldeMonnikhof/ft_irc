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
#include <string.h>

#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/poll.h>
#include "Client.hpp"
#include "error.hpp"

#define WAIT_FOREVER -1

#define BUFFER_LENGTH 2048

#define WHITE_SPACE "\f\n\r\t\v "

using namespace std;

class Client;

class Command{
    private:
        vector<string>  cmd;
    
    public:
        Command(){};
        Command(string str);

        void    parseCmd(string str);

        string  getCmd(int i) const;
        size_t  getSize() const;

};

//PASS NICK USER

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


        void    msgPASS(int fd, Command& cmd);
        bool    isValidNickname(string nick);
        bool    nickInUse(string nick);
        void    msgNICK(int fd, Command& cmd);
        void    msgUSER(int fd, Command& cmd);


        void    errorMsg(int fd, int error, Command& cmd);

        void    registerClient(int fd, Command& cmd);
        void    cmdsClient(int fd, Command& cmd);
        void    handleMsgClient(int fd);
        string  parseBuffer(string buffer);

        void    iterateClientRevents();
        void    mainLoop();
};

#endif