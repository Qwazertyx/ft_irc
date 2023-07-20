#include "../incl/ircserv.hpp"

std::string	erasebr(std::string str) 
{
	if (str.empty())
		return "";
	if (str[str.size() - 1] == '\r')
		return str.substr(0, (str.size() - 1));
	return str;
}

std::string	ERROR_NEED_MORE_PARAMETERS(Client &client, std::string cmd) 
{
	return ("461 " + client.getNickname() + " " + cmd + " :Not enough parameters");
}

std::string  ERROR_NO_SUCH_CHANNEL_EXISTS(Client cl, std::string channel) 
{
	return ("403 " + cl.getNickname() + " " + channel +" :No such channel");
}

std::string  ERROR_CHANNEL_OPERATOR_NEEDED(Client cl, std::string channel) 
{
	return ("482 " + cl.getNickname() + " " + channel + " :You're not channel operator");
}

std::string  RPL_TOPIC(Client cl, std::string channel, std::string topic) 
{
	return ("332 " + cl.getNickname() + " " + channel + " " + topic);
}

bool	isClientInChannel(Channel &chan, int fd)
{
	for (unsigned int i = 0; i < chan.getClients().size(); i++)
	{
		if (chan.getClients()[i].getFd() == fd)
			return true;
	}
	return false;
}

bool	is_client_not_in_Channel(Channel &chan, std::string name)
{
	for (unsigned int i = 0; i < chan.getClients().size(); i++)
	{
		if (chan.getClients()[i].getNickname() == name)
			return true;
	}
	return false;
}

bool	is_operator_in_Channel(Client cl, Channel chan)
{
	if (chan.getFdOp() == cl.getFd())
		return true;
	return false;
}

std::vector<std::string> split(std::string str, const char *delim)
{
	std::vector<std::string> list;

	char 	*ptr = strtok((char *)str.c_str(), delim);

	while (ptr)
	{
		list.push_back(std::string(ptr));
		ptr = strtok(NULL, delim);
	}
	return list;
}