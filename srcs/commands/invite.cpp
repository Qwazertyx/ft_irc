#include "../../incl/ircserv.hpp"
#include <algorithm>
#include <vector>

#include <cstdio>

int Server::cmdInvite(std::vector<std::string> params, Client &cl)
{
	if (params.size() < 3)
	{
		cl.reply(errorparam(cl, "Invite"));
		return (-1);
	}
    std::cout << "4" << std::endl;
    try {
        std::vector<Channel>::iterator inviteChan = findChannelIt(params[1]);
        int tl = params[2].size() - 1;
        if (params[2][tl] > 0 && params[2][tl] <= 31)
        {
            params[2].erase(tl, 1);
        }
        //std::vector<Client>::iterator  invitedCl = findClientIt(params[2]);
        Client invitedCl = findClient(params[2]);
        if (inviteChan->isInside(cl))
            throw std::runtime_error("442 " + params[1] + " :You are not in channel");
        if (!inviteChan->isInvited(invitedCl))
            throw std::runtime_error("443 :" + params[2] + " " + params[1] + " :User is already invited");
        if (!inviteChan->isInside(invitedCl))
            throw std::runtime_error("443 " + cl.getNickname() + " : User is already in the channel");
        inviteChan->addInvited(invitedCl);
        cl.reply("341  :invite " + params[2] + " " + params[1]);
        //invitedCl.reply("345 :You got invited on " + params[1] + " by " + cl.getNickname());
        invitedCl.reply("345 " + params[2] + " " + params[1] + " " + cl.getNickname() + " :" + params[2] + " has been invited by " + cl.getNickname());
    }
	catch (const std::exception &ecp) {
        cl.reply(ecp.what());
        return (-1);
    }
    return (0);
}