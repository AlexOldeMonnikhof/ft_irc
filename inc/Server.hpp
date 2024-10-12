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
        vector<Channel>         _channels;
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

        void    cmdJoin(int fd, Command& cmd);
        void    cmdPart(int fd, Command& cmd);

        void    privmsgChannel(int fd, Command& cmd, string channel);
        void    privmsgClient(int fd, Command& cmd, string nick);
        void    cmdPrivmsg(int fd, Command& cmd);

        void    registerClient(int fd, Command& cmd);
        void    cmdsClient(int fd, Command& cmd);
        void    handleMsgClient(int fd);

        bool    channelExist(string channel);
        int     getClientFd(string nick);

        void    mainLoop();
};

void    sendMsg(int fd, string msg);
vector<string>    splitVector(const string &s, char delimiter);

#endif