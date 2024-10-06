#ifndef SERVER_HPP
#define SERVER_HPP

#include "headers.hpp"
#include "Client.hpp"
#include "Channel.hpp"
#include "error.hpp"

#define WAIT_FOREVER -1

#define BUFFER_LENGTH 2048

#define WHITE_SPACE "\f\n\r\t\v "

using namespace std;

class Client;
class Channel;

class Command{
    private:
        vector<string>  _cmd;
    public:
        Command(){};
        Command(string str);

        void    parseCmd(string str);

        void    setCmd(int i, string arg);
        string  getCmd(int i) const;
        size_t  getSize() const;
};

//PASS NICK USER

class Server{
    private:
        int                     _port;
        int                     _socket;
        string                  _password;
        string                  _host;
        vector<pollfd>          _fds;
        map<int, Client>        _clients;
        map<string, Channel>    _channels;
    public:
        Server(string port, string password);
        void    parseServer(string port, string password);
        void    initServer();
        void    addClient();
        void    disconnectClient(vector<pollfd>::iterator& iter);


        void    msgPASS(int fd, Command& cmd);
        bool    isValidName(string nick);
        bool    nickInUse(int fd, string nick);
        void    msgNICK(int fd, Command& cmd);
        void    msgUSER(int fd, Command& cmd);

        template <typename V>
        void    parseJoinVectors(V& channels, string str);

        void    cmdJoin(int fd, Command& cmd);


        void    errorMsg(int fd, int error, Command& cmd);

        void    registerClient(int fd, Command& cmd);
        void    cmdsClient(int fd, Command& cmd);
        void    handleMsgClient(int fd);
        string  parseBuffer(string buffer);

        void    iterateClientRevents();
        void    mainLoop();
};

void    sendMsg(int fd, string msg);

#endif