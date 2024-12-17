#include "Server.hpp"

size_t getChannelIndex(vector<Channel>& channels, string name)
{
    for (size_t i = 0; i < channels.size(); i++)
    {
        if (channels[i].getName() == name)
            return i;
    }
    return string::npos;
}
