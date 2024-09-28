#ifndef SERVER_HPP
#define SERVER_HPP

using namespace std;

#include <string>

class Server{
    private:
        int     _port;
        string  _password;
    public:
        Server(string port, string password);
}

#endif