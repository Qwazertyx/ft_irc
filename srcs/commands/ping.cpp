#include "../../incl/ircserv.hpp"

int Server::cmdPing(std::vector<std::string> args, Client &cl)
{
    if (args.size() < 2)
	{
		cl.reply(ERROR_NEED_MORE_PARAMETERS(cl, "Ping"));
		return (-1);
	}
    cl.reply("Pong " + args[1]);
    return (0);
}