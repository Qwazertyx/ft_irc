#include "../../incl/ircserv.hpp"

bool     Server::isChannel(std::string name)
{
	for (unsigned int i = 0; i < _channels.size(); i++)
	{
		if (_channels[i].getName() == name)
			return true;
	}
	return false;
}

int	Server::cmdKick(std::vector<std::string> args, Client &cl) 
{
	if (cl.getState() != REGISTERED)
    {
        cl.reply("451 :" + cl.getNickname() + " You need to register first");
        return -1;
    }
	std::string cmd = args.at(0);
	if (args.size() < 3)
	{
		cl.reply("461 " + cl.getNickname() + " " + cmd + " :Not enough parameters");
		return -1;
	}
	if (isChannel(args.at(1)) == false)
	{
		cl.reply("403 " + cl.getNickname() + " " + args.at(1) + " :No such channel");
		return -1;
	}
	if (isClientInChannel(findChannel(args.at(1)), cl.getFd()) == false)
	{
		cl.reply("442 " + cl.getNickname() + " " + args.at(1) + " :You're not on that channel");
		return -1;
	}
	if (cl.getFd() != findChannel(args.at(1)).getFdOp())
	{
		cl.reply(errornotop(cl, args.at(1)));
		return -1;
	}
	if (is_client_not_in_Channel(findChannel(args.at(1)), erasebr(args.at(2))) == false)
	{
		cl.reply("441 " + cl.getNickname() + " " + args.at(1) + " " + erasebr(args.at(2)) + " :They aren't on that channel");
		return -1;
	}
	std::vector<std::string> tmp;
	tmp.push_back("Part");
	tmp.push_back(args.at(1));
	if (args[2].find(13) != std::string::npos)
		args[2].erase(args[2].find(13), 1);
	cmdPart(tmp, findClient(args.at(2)));
	return 0;
}