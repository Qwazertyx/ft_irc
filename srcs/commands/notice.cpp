#include "../../incl/ircserv.hpp"

std::string RPL_NOTICE(Client cl, std::string recipient, std::string message)
{
    return (":" + cl.getNickname() + " NOTICE " + recipient + " :" + message);
}

std::string      getMessageNotice(std::vector<std::string> params)
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

int Server::chanNotice(std::vector<std::string> params, Client &cl)
{
    if (params.size() < 3)
    {
        cl.reply("need more params");
        return -1;
    }
    std::string  message = getMessageNotice(params);
    try
    {
        std::vector<Channel>::iterator chan = findChannelIt(params[1]);
        if (isClientInChannel(*chan, cl.getFd()))
            chan->broadcast(RPL_NOTICE(cl, params[1], message), cl);
        else
            cl.reply("404 " + cl.getNickname() + " you are not in the channel " + chan->getName());
    }
    catch(const std::exception& e)
    {
       cl.reply(e.what());
    }
    return 0;
}

int Server::cmdNotice(std::vector<std::string> params, Client &cl)
{
    std::cout << "ENTER IN NOTICE" << std::endl;
    if (cl.getState() != REGISTERED)
    {
        cl.reply("451 :" + cl.getNickname() + " You need to register first");
        return -1;
    }
    if (params.size() < 3)
	{
        std::cout << "parameters size = "<< params.size() << std::endl;
        cl.reply("461 " + cl.getNickname() + " " + "Notice" + " :Not enough parameters");
		return -1;
	}
    if (params[1].at(0) == '#')
        return (chanNotice(params, cl));
    try
    {
        Client  recipient = findClient(params[1]);      
        std::string  msg = getMessageNotice(params);
        std::string paquet = RPL_NOTICE(cl, recipient.getNickname(), msg);
        std::cout << paquet << recipient.getFd() << std::endl;
        paquet += "\r\n";
        if (send(recipient.getFd(), paquet.c_str(), paquet.length(), 0) < 0)
            throw std::out_of_range("error while sending in notice");
    }
    catch(const std::exception& e)
    {
        cl.reply("401 " + cl.getNickname() + " :User could not be found");
        std::cerr << e.what() << '\n';
    }
    return 0;
}