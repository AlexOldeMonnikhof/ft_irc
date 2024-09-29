#include "Server.hpp"

#include <iostream>
using namespace std;

int main(int argc, char **argv)
{
    if (argc != 3)
    {
        cout << "Usage: ./ircsercv port password" << endl;
        return 1;
    }
    try
    {
        Server server(argv[1], argv[2]);
    }
    catch (const std::exception& e)
    {
        cout << e.what() << endl;
        return 1;
    }
    return 0;
}