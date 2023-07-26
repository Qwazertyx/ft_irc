#include "../../incl/ircserv.hpp"

int Server::cmdUser(std::vector<std::string> args, Client &cl) 
{
	std::string tmp;
	std::string cmd = args.at(0);
	if (args.size() < 5)
	{
		cl.reply(errorparam(cl, "User"));
		return -1;
	}
	else if (cl.getUsername() == args.at(1))
	{
		cl.reply("462 " + cl.getNickname() + " " + cmd + " :Unauthorized command (already registered)");
		return -1;
	}
	else if (args.size() >= 5)
	{
		cl.setUsername(args.at(1));
		size_t i;
		for (i = 1; i < args.size() - 1; i++)
		{
			if (args.at(i)[0] == ':')
				break ;
		}
		if (args.at(1)[0] == ':')
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
		tmp += " ";
		for (; i < args.size() - 1; i++)
			tmp += args.at(i) + " ";
		tmp += args.at(i);
		cl.setRealname(tmp);
	}
	cl.welcome();
	return 0;
}