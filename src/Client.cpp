#include "Client.hpp"

Client::Client(int socket)
{
    _socket = socket;
    _register = 6;
    // setRegister(0);
    // cout << _register << endl;
}

int Client::getRegister() const
{
    return (_register);
}

void    Client::setRegister(int value)
{
    _register = _register | value;
}
