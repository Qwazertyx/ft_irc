#include "../../incl/ircserv.hpp"

int         Server::cmdPart(std::vector<std::string> params, Client &cl)
{
    if (cl.getState() != REGISTERED)
    {
        cl.reply("451 :" + cl.getNickname() + " You need to register first");
        return -1;
    }
    if (params.size() < 2)
    {
        cl.reply(errorparam(cl, "Part"));
        return -1;
    }
    try
    {
        std::string chan_name = erasebr(params[1]);
        std::vector<Channel>::iterator chan = findChannelIt(chan_name);
        if (chan->getFdOp() == cl.getFd())
            chan->setFdOp(0);
        chan->eraseClient(cl);
        if (chan->getClients().empty())
            _channels.erase(chan);
    }
    catch(const std::exception& e)
    {
        cl.reply("403 " + cl.getNickname() + " " + params.at(1) + " :No such channel");
    }
    return 0;
}