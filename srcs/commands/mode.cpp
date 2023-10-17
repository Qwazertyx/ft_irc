#include "../../incl/ircserv.hpp"

int	giveOprivilege(Client &cl, std::vector<std::string> args, Channel &chan) 
{
	int check = 0;
	size_t i = 0;
	std::cout << "HERE" << std::endl;
	if (args.size() < 4) 
	{
		cl.reply(errorparam(cl, "Mode"));
		return -1;
	}
	for (; i < chan.getClients().size(); i++)
	{
		if (chan.getClients().at(i).getNickname() == erasebr(args.at(3)))
		{
			std::cout << "I'm here" << std::endl;
			check = 1;
			break ;
		}
	}
	if (check == 1)
		chan.setFdOp(chan.getClients().at(i).getFd());
	else
		cl.reply("401 " + cl.getNickname() + " " + erasebr(args.at(3)) + " :No such nickname/channel");
	return 0;
}


int setPassword(std::string password, Channel &chan) 
{
	if (password.empty())
		return -1;
	chan.setPassword(password);
	return 0;
}

int removePassword(std::string password, Channel &chan) 
{
	if (password.empty())
		return -1;

	if (chan.getPassword() == password)
		chan.setPassword("");
	else
		std::cout << "Wrong password" << std::endl;
	return 0;
}


size_t	parseLimit(std::string arg) 
{
	char	*buff;

	std::string tmp = erasebr(arg);
	if (tmp.empty())
		return (0);
	long int limit = strtol(tmp.c_str(), &buff, 10);
	if (*buff != '\0')
		return (0);
	return (limit);
}

int	setLimit(size_t limit, Channel &chan)
{
	if (limit < chan.getClients().size() && limit != 0)
	{
		std::cout << "to many people on this channel to set this limit" << std::endl;
		return -1;
	}
	chan.setLimit(limit);
	return 0;
}

int	check_flag(std::vector<std::string> args, Client &cl, Channel &chan) 
{
	int i = 0;
	std::string flags[10] = {"+o","-o","+l","-l","+k","-k","+i","-i","+t","-t"};
	std::string flag = erasebr(args[2]);
	while (flag != flags[i] && i < 10)
		++i;
	std::cout << "IN CHECK_FLAG" << std::endl;
	switch (i) {
		case 0:

			return (giveOprivilege(cl, args, chan));
		case 1:
			chan.setFdOp(0);
			return 0;
		case 2:
			if (args.size() < 4) {
				cl.reply(errorparam(cl, "Mode"));
				return -1;
			}
			return (setLimit(parseLimit(args[3]), chan));
		case 3:
			return (setLimit(0, chan));
		case 4:
			if (args.size() < 4) {
				cl.reply(errorparam(cl, "Mode"));
				return -1;
			}
			return (setPassword(erasebr(args[3]), chan));
		case 5:
			if (args.size() < 4) {
				cl.reply(errorparam(cl, "Mode"));
				return -1;
			}
			return (removePassword(erasebr(args[3]), chan));
		case 6:
			chan.setInviteOnly(true);
			std::cout << chan.getName() << " is now in invite only" << std::endl;
			return 0;
		case 7:
			chan.setInviteOnly(false);
			std::cout << chan.getName() << " is no more in invite only" << std::endl;
			return 0;
		case 8:
			chan.setTopicOperator(true);
			std::cout << chan.getName() << " need operator privileges to change topic" << std::endl;
			return 0;
		case 9:
			chan.setTopicOperator(false);
			std::cout << chan.getName() << " don't need operator privileges to change topic" << std::endl;
			return 0;
		default:
			cl.reply("501 " + cl.getNickname() + " :Unknown Mode flag");
			return -1;
	}
	return 0;
}


int Server::cmdMode(std::vector<std::string> args, Client &cl) 
{
	if (cl.getState() != REGISTERED)
    {
        cl.reply("451 :" + cl.getNickname() + " You need to register first");
        return -1;
    }
	if (args.size() < 3 || args[1] == "")
	{
		cl.reply(errorparam(cl, "Mode"));
		return -1;
	}
	if (args[1].find(13) != std::string::npos)
		args[1].erase(args[1].find(13));

	if (args.size() < 2)
	{
		cl.reply(errorparam(cl, "Mode"));
		return -1;
	}
	if (isChannel(args.at(1)) == false)
	{
		if (erasebr(args[1]).at(0) != '#')
			return -1;
		cl.reply(errornotchannel(cl, erasebr(args[1])));
		return -1;
	}
	if (cl.getFd() != findChannel(args.at(1)).getFdOp())
	{
		cl.reply(errornotop(cl, args.at(1)));
		return -1;
	}
	check_flag(args, cl, findChannel(args.at(1)));
	return 1;
}