#include "../../incl/ircserv.hpp"
#include <algorithm>
#include <vector>

int Server::cmdInvite(std::vector<std::string> params, Client &cl)
{
    cl.reply("1");
	if (params.size() < 3)
	{
        cl.reply("2");
		cl.reply(errorparam(cl, "Invite"));
        cl.reply("3");
		return (-1);
	}
    std::cout << "4" << std::endl;
    try {
        Channel inviteChan = findChannel(params[1]);
        cl.reply("11234");
        Client  invitedCl = findClient(params[2]);
      cl.reply("1123");
        if (inviteChan.isInside(cl))
            throw std::runtime_error("You are not in channel");
       cl.reply("113");
        if (!inviteChan.isInvited(cl))
            throw std::runtime_error("User is already invited");
        cl.reply("112");
        if (!inviteChan.isInside(invitedCl))
            throw std::runtime_error("443 " + cl.getNickname() + " : User is already in the channel");
        cl.reply("111");
        inviteChan.addInvited(invitedCl);
       cl.reply("10");
        cl.reply("invited " + params[2]);
    }
	catch (const std::exception &ecp) {
        cl.reply(ecp.what());
        return (-1);
    }
    cl.reply("12");
    return (0);
}