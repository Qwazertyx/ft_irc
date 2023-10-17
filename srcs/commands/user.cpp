#include "../../incl/ircserv.hpp"
#include <cstdio>

int Server::cmdUser(std::vector<std::string> args, Client &cl) 
{
	std::string tmp;
	std::string cmd = args.at(0);
	std::cout << "USERCMD\n";
	for (long unsigned int k = 0; k < args.size(); k++)
		std::cout << args[k] << std::endl;
	if (args.size() < 5)
	{
		cl.reply(errorparam(cl, "User"));
		return -1;
	}
	else if (!cl.getUsername().empty())
	{
		cl.reply("462 " + cl.getNickname() + " " + cmd + " :Unauthorized command (already registered)");
		return -1;
	}
	else if (args.size() >= 5)
	{
		if (args[1].find(":") != std::string::npos || args[2].find(":") != std::string::npos || args[3].find(":") != std::string::npos)
		{
			std::cerr << "ici\n";
			cl.reply("497 " + cl.getNickname() + " " + cmd + " :Wrong parameters");
			return -1;
		}
		if (args[2].size() > 1 || (!(args[2][0] >= '0' && args[2][0] <= '9') && args[3][0] != '*'))
		{
			cl.reply("497 " + cl.getNickname() + " " + cmd + " :Wrong parameters");
			return -1;
		}
		if (args[3].size() > 1 || args[3][0] != '*')
		{
			std::cerr << "non la\n";
			cl.reply("497 " + cl.getNickname() + " " + cmd + " :Wrong parameters");
			return -1;
		}
		cl.setUsername(args.at(1));
		size_t i;
		for (i = 1; i < 4; i++)
		{
			if (args.at(i)[0] == ':')
				break ;
		}
		if (args.at(i)[0] == ':')
			tmp = args.at(i).substr(1);
		else
			tmp = args.at(i);
		if (i + 1 == args.size())
		{
			cl.setRealname(tmp);
			cl.welcome();
			return 0;
		}
		i++;
		if (args[i - 1][0] == ':')
		{
			tmp += " ";
			for (; i < args.size() - 1; i++)
				tmp += args.at(i) + " ";
			tmp += args.at(i);
		}
		cl.setRealname(tmp);
	}
	cl.welcome();
	return 0;
}