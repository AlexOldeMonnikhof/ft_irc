#include "Server.hpp"

Command::Command(string str)
{
    parseCmd(str);
}

void  Command::parseCmd(string str)
{
    std::string word = "";
    bool colonFound = false;

    for (size_t i = 0; i < str.size(); ++i)
    {
        if (colonFound)
            word += str[i];
        else if (std::isspace(str[i]))
        {
            if (!word.empty())
            {
                cmd.push_back(word);
                word.clear();
            }
        }
        else if (str[i] == ':' && !colonFound)
        {
                colonFound = true;
                cmd.push_back(word);
                word.clear();
        }
        else
            word += str[i];
    }
    word.erase(0, word.find_first_not_of(WHITE_SPACE));
    if (!word.empty())
        cmd.push_back(word);
    // for (size_t i = 0; i < cmd.size(); i++)
        // cout << "cmd " << i << " = " << cmd[i] << endl;
}

string  Command::getCmd(int i) const
{
    return cmd[i];
}

size_t  Command::getSize() const
{
    return cmd.size();
}
