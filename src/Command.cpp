#include "Server.hpp"

Command::Command(string str)
{
    parseCmd(str);
}

void  Command::parseCmd(string str)
{
    std::string ret;
	bool inWord = false, column = false;

	for (size_t i = 0; i < str.length(); i++)
	{
		if (i == str.length() - 1)
		{
			ret += '\0';
			break;
		}
		if (isspace(str[i]) && inWord == false)
			continue;
		else if (str[i] == ':')
		{
			column = true;
			continue;
		}
		else if (isspace(str[i]) && column == true)
		{
			ret += str[i];
			continue;
		}
		else if (isspace(str[i]))
			inWord = false;
		else
			inWord = true;
		ret += str[i];
	}
	char *token = strtok((char *)ret.c_str(), " ");
	while (token)
	{
		cmd.push_back(token);
		token = strtok(nullptr, " ");
	}
	// cmd.push_back(nullptr);
	for (size_t i = 0; i < cmd.size(); i++)
	{
		cout << "cmd " << i << " = " << cmd[i] << endl;
	}
}

string	Command::getCmd(int i) const
{
	return cmd[i];
}

size_t	Command::getSize() const
{
	return cmd.size();
}
