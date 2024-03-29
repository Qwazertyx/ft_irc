#include "../../incl/ircserv.hpp"

std::string     RPL_NAMREPLY(Client &cl, std::string chan_name, std::string users)
{
    std::string ret = "353 " + cl.getNickname() + " = " + chan_name + " :" + users;
    std::cout << "name reply " << ret << std::endl;
    return (ret);
}

std::string     RPL_JOIN(Client cl, Channel channel, std::string topic) 
{
    if (channel.getFdOp() == cl.getFd())
        return (("332 @" + cl.getNickname() + " " + channel.getName() + " " + topic));
	else
        return ("332 " + cl.getNickname() + " " + channel.getName() + " " + topic);
}

std::string     RPL_ENDOFNAMES(Client &cl, std::string chan_name) 
{
    return ("366 " + cl.getNickname() + " " + chan_name + " :End of /Names list.");
}

std::string test(std::string num, std::string nick, std::string msg)
{
    if (nick.empty())
        nick = "*";
    return (":IRC " + num + " " + nick + " " + msg + "\n");
}

void        join(Channel &chan, Client &cl, bool alreadyCreated)
{
    std::string users = "";
    for(unsigned int i = 0; i < chan.getClients().size(); i++)
    {
        if (chan.getClients()[i].getFd() == chan.getFdOp())
            users += "@" + chan.getClients()[i].getNickname() + " ";
        else
            users += chan.getClients()[i].getNickname() + " ";
    }
    chan.broadcast(cl.getPrefix() + " JOIN :" + chan.getName());

    cl.reply(RPL_NAMREPLY(cl, chan.getName(), users));
    if (alreadyCreated)
    {
        cl.reply(RPL_TOPIC(cl, chan.getName(), chan.getTopic()));
        cl.reply(RPL_ENDOFNAMES(cl, chan.getName()));
    }
}

int         Server::cmdJoin(std::vector<std::string> params, Client &cl)
{
    if (cl.getState() != REGISTERED)
    {
        cl.reply("451 : You need to finalise your registration first");
        return -1;
    }
    if (params.size() < 2)
    {
        cl.reply(errorparam(cl, "Join"));
        return -1;
    }
    std::string name = erasebr(params[1]);
    std::cout << "name = " << name<< std::endl;
    if (!name[0] || name[0] != '#')
    {
        cl.reply("403 " + name +" :Channel must start with '#'");
        return -1;
    }
    try
    {
        std::vector<Channel>::iterator chan = findChannelIt(name);
        if (isClientInChannel(*chan, cl.getFd()))
        {
            std::cout << "is already in channel" << std::endl;
            return -1;
        }
		if (findChannel(name).getClients().size() >= findChannel(name).getLimit() && findChannel(name).getLimit() != 0)
		{
			cl.reply("471 " + cl.getNickname() + " " + name + " :Cannot join channel (full)");
			return -1;
		}
		if (findChannel(name).getPassword() != "" && params.size() == 3)
		{
			if (erasebr(params.at(2)) != findChannel(name).getPassword())
			{
				cl.reply("bad password");
				return -1;
			}
		}
		if (findChannel(name).getPassword() != "" && params.size() < 3)
		{
			cl.reply("475 " + cl.getNickname() + " " + name + " :this channel needs a password to be joined");
			return -1;
		}
        if (chan->canJoinInvite(cl))
        {
            cl.reply("473 " + cl.getNickname() + " " + name + " :Cannot join channel (invite only)");
            return -1;
        }
        chan->addClient(cl);
        join(*chan, cl, true);
    }
    catch(const std::exception& e)
    {
        std::cout << "New channel opened : " << name << std::endl;
        Channel new_chan(name);
        new_chan.addClient(cl);
        new_chan.setFdOp(cl.getFd());
        _channels.push_back(new_chan);
        join(new_chan, cl, false);
    }
    _channels[0].debug();
    return 0;
}