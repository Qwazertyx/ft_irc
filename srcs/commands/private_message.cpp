#include "../../incl/ircserv.hpp"

std::string RPL_PRIVMSG(Client cl, std::string recipient, std::string message)
{
    return (":" + cl.getNickname() + " Private Message " + recipient + " :" + message);
}

std::string      getMessage(std::vector<std::string> params)
{
    std::string message;

    unsigned int i = 3;
    message = params[2];
    if (message[0] == ':')
        message = message.substr(1);
    while (i < params.size())
    {
        message += " ";
        message += params[i];
        i++;
    }
    message = erasebr(message);
    return message;
}

int Server::chanMessage(std::vector<std::string> params, Client &cl)
{
    if (params.size() < 3)
    {
        cl.reply("need more parameters");
        return -1;
    }
    std::string  message = getMessage(params);
    try
    {
        std::vector<Channel>::iterator chan = findChannelIt(params[1]);
        if (isClientInChannel(*chan, cl.getFd()))
            chan->broadcast(RPL_PRIVMSG(cl, params[1], message), cl);
        else
            cl.reply("404 " + cl.getNickname() + " you are not in the channel " + chan->getName());
    }
    catch(const std::exception& e)
    {
       cl.reply(e.what());
    }
    return 0;
}

int Server::cmdPrvMsg(std::vector<std::string> params, Client &cl)
{
    std::cout << "Enter In Private Message" << std::endl;
    if (cl.getState() != REGISTERED)
    {
        cl.reply("you need to register first (Private Message)");
        return -1;
    }
    if (params.size() < 3)
	{
        cl.reply("461 " + cl.getNickname() + " " + "Private Message" + " :Not enough parameters");
		return -1;
	}
    if (params[1].at(0) == '#')
        return (chanMessage(params, cl));
    try
    {
        Client  recipient = findClient(params[1]);      
        std::string  msg = getMessage(params);
        std::cout << "message = " << "[" << msg << "]" << std::endl;
        std::string paquet = RPL_PRIVMSG(cl, recipient.getNickname(), msg);
        std::cout << paquet << recipient.getFd() << std::endl;
        paquet += "\r\n";
        if (send(recipient.getFd(), paquet.c_str(), paquet.length(), 0) < 0)
            throw std::out_of_range("error while sendig in private message");
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    return 0;
}