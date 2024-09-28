#include "Server.hpp"

Server::Server(string port, string password)
{
    int socketFd;
    
    if (socketFd = socket(AF_INET, SOCK_STREAM, 0) < 0)
        exit(1);
    if (setsockopt())
}