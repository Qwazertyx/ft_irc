#include "../incl/ircserv.hpp"

Client::Client(int sockfd, std::string hostname) : _sockfd(sockfd), _hostname(hostname), _isoper(false)
{
    _state = HANDSHAKE;
    _msg = "";
}

Client::~Client() {}

std::string  Client::getPrefix()
{
    std::string prefix = ":" + _nickname + (_username.empty() ? "" : "!" + _username) + (_hostname.empty() ? "" : "@" + _hostname);
    return prefix;
}

void    Client::reply(std::string msg) 
{
    std::string prefix = _nickname + (_username.empty() ? "" : "!" + _username) + (_hostname.empty() ? "" : "@" + _hostname);
    std::string paquet = ":" + prefix + " " + msg + "\r\n";
    std::cout << "---> " << paquet << std::endl;
    if (send(_sockfd, paquet.c_str(), paquet.length(), 0) < 0)
        throw(std::out_of_range("Error while sending"));
}

void    Client::welcome() 
{
    if (_state != LOGIN || _nickname.empty() || _username.empty())
    {
        std::cout << "Waiting registration... " << _nickname << std::endl;
        return ;
    }
    _state = REGISTERED;
    reply("001 " + _nickname + " :Welcome " +_nickname +  " into our irc network");
    std::cout << _nickname << " is registered" << std::endl;
}

int     Client::getFd() const {return _sockfd;}
std::string  Client::getNickname() const {return _nickname;}
std::string  Client::getUsername() const {return _username;}
std::string  Client::getRealname() const {return _realname;}
std::string  Client::getHostname() const {return _hostname;}
std::string  Client::getMsg() const {return _msg;}
State	Client::getState() const {return _state;}
bool	Client::getisoper() const {return _isoper;}

void	Client::setNickname(std::string newName) {_nickname = newName;}
void	Client::setUsername(std::string newName) {_username = newName;}
void	Client::setRealname(std::string newName) {_realname = newName;}
void	Client::setHostname(std::string newName) {_hostname = newName;}
void	Client::setMsg(std::string newMsg) {_msg = newMsg;}
void    Client::addMsg(std::string buff) 
{
    _msg += buff;
}
void	Client::setisoper(bool isoper) {_isoper = isoper;}

void  Client::setState(State new_state)
{
    _state = new_state;
}