#include "../../incl/ircserv.hpp"

std::string  NO_SET_TOPIC(Client cl, std::string channel) 
{
	return ("331 " + cl.getNickname() + " " + channel +" :No topic is set");
}

std::string  errornot_onchan(Client cl, std::string channel) 
{
	return ("442 " + cl.getNickname() + " " + channel + " :You're not on that channel");
}

std::string      getTopicStr(std::vector<std::string> params)
{
    std::string topic;

    unsigned int i = 3;
    topic = params[2];
    if (topic[0] == ':')
        topic = topic.substr(1);
    while (i < params.size())
    {
        topic += " ";
        topic += params[i];
        i++;
    }
    topic = erasebr(topic);
    return topic;
}

int		Server::cmdTopic(std::vector<std::string> args, Client &cl)
{
	if (cl.getState() != REGISTERED)
    {
        cl.reply("451 :" + cl.getNickname() + " You need to register first");
        return -1;
    }
	if (args.size() < 2)
	{
		cl.reply(errorparam(cl, "Topic"));
		return -1;
	}
	std::string chan_name = erasebr(args[1]);
	if (chan_name.empty())
	{
		cl.reply(errorparam(cl, "Topic"));
		return (-1);
	}
	try 
	{
		std::vector<Channel>::iterator chan = findChannelIt(chan_name);
		if (isClientInChannel(*chan, cl.getFd()))
		{
			if (args.size() == 2)
			{
				if (chan->getTopic().empty())
					cl.reply(NO_SET_TOPIC(cl, chan_name));
				else
					cl.reply(RPL_TOPIC(cl, chan_name, chan->getTopic()));
				return (0);
			}
			else if (is_operator_in_Channel(cl, *chan) || chan->getTopicOperator() == false)
			{
				chan->setTopic(getTopicStr(args));
				chan->broadcast(RPL_TOPIC(cl, chan_name, chan->getTopic()));
				return (0);
			}
			else
				cl.reply(errornotop(cl, chan_name));
		}
		else
			cl.reply(errornot_onchan(cl, chan_name));
	}
	catch (const std::exception& e) 
	{
		cl.reply(errornotchannel(cl, chan_name));
	}
	return (-1);
}