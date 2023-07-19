/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ircserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vsedat <vsedat@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/02 13:46:51 by vsedat            #+#    #+#             */
/*   Updated: 2023/07/19 14:03:33 by vsedat           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IRCSERV_HPP
# define IRCSERV_HPP

# include <iostream>
# include <algorithm>
# include <sys/types.h>
# include <arpa/inet.h>
# include <map>
# include <sys/socket.h>
# include <netdb.h>
# include <poll.h>
# include <unistd.h>
# include <fcntl.h>
# include <cstring>
# include <strings.h>
# include <string>
# include <signal.h>
# include <vector>
# include <cerrno>
# include <cstdlib>
# include <sstream>

extern bool g_interrupt;

enum State
{
	HANDSHAKE,
	LOGIN,
	REGISTERED
};

class Channel;

// Client Class

class Client 
{
	private:
		int			_sockfd;
		std::string		_nickname;
		std::string		_username;
		std::string		_realname;
		std::string		_hostname;
		std::string		_msg;
		State		_state;
		bool		_isoper;
		std::vector<Channel>	_channels;

	public:
		Client(int fd, std::string host);
		~Client();

	// Client functions

		void		reply(std::string msg);
		void		welcome();

	// Getter functions

		int			getFd() const;
		std::string		getNickname() const;
		std::string		getUsername() const;
		std::string		getRealname() const;
		std::string		getHostname() const;
		std::string		getMsg() const;
		std::string		getPrefix();
		State		getState() const;
		bool		getisoper() const;

	// Setter functions
	
		void		setNickname(std::string newName);
		void		setUsername(std::string newName);
		void		setRealname(std::string newName);
		void		setHostname(std::string newName);
		void		setMsg(std::string newMsg);
		void		addMsg(std::string buff);
		void		setState(State newState);
		void		setisoper(bool isoper);
};

// Channel Class

class Channel 
{
	private:
		std::string						_name;
		std::string						_topic;
		int							_fdOp;
		size_t						_limit;
		std::string						_password;
		std::vector<Client>			_clients;

	public:
		Channel(std::string _name);
		~Channel();

	// Getter

		int						getFdOp() const; 
		size_t					getLimit() const;
		std::string					getName() const;
		std::string					getTopic() const;
		std::string					getPassword() const;
		std::vector<Client>		&getClients();

	// Setter

		void					setTopic(std::string newTopic);
		void					setPassword(std::string pass);
		void					setFdOp(int fd);
		void					setLimit(size_t limit);

	// Channel functions
	
		void					addClient(Client &client);
		void					eraseClient(Client &cl);
		void					broadcast(std::string message);
		void					broadcast(std::string message, Client &cl);
		void					debug();
};

// Server Class

class Server 
{
	private:
		std::string								_host;
		std::string								_password;
		std::string								_operPassword;
		int									_port;
		int									_sock;

		std::vector<std::string>					_cmd;
		std::vector<pollfd>					_pollfds;
		std::vector<Client>					_clients;
		std::vector<Channel>				_channels;

	public:
		Server(int port, const std::string &password);
		~Server();

	// Server Init

		int									createSocket();
		void								launch();

	// Server Display

		void								handleMessage(int fd);
		void								displayClient();

	// Server Receipt 

		std::vector<std::string>					splitCmd(std::string msg);
		void								parseCmd(std::string str, Client &cl);
		std::string								readMsg(int fd);

	// Manage Clients

		void								newClient();
		void								eraseClient(int fd);
		void								eraseClientChannel(Client &cl);
		void								clientDisconnect(int fd);

	// Server Utils

		int									chanMessage(std::vector<std::string> params, Client &cl);
		int									chanNotice(std::vector<std::string> params, Client &cl);
		bool								already_used(std::string name, Client cl);

		Client								&findClient(int fd);
		Client								&findClient(std::string nickname);
		std::vector<Client>::iterator		findClientIt(int fd);
		

		bool     							isChannel(std::string name);
		Channel								&findChannel(std::string name);
		std::vector<Channel>::iterator		findChannelIt(std::string name);

	// IRC Commands

		int									cmdPass(std::vector<std::string> pass, Client &cl);
		int									cmdNick(std::vector<std::string> pass, Client &cl);
		int									cmdUser(std::vector<std::string> pass, Client &cl);
		int									cmdPrvMsg(std::vector<std::string> pass, Client &cl);
		int									cmdPing(std::vector<std::string> args, Client &cl);
		int									cmdJoin(std::vector<std::string> args, Client &cl);
		int									cmdOper(std::vector<std::string> args, Client &cl);
		int									cmdKill(std::vector<std::string> args, Client &cl);
		int									cmdPart(std::vector<std::string> args, Client &cl);
		int									cmdList(std::vector<std::string> args, Client &cl);
		int									cmdNames(std::vector<std::string> args, Client &cl);
		int									cmdTopic(std::vector<std::string> args, Client &cl);
		int									cmdKick(std::vector<std::string> args, Client &cl);
		int									cmdNotice(std::vector<std::string> args, Client &cl);
		int									cmdMode(std::vector<std::string> args, Client &cl);
};

// Utils.cpp

	std::string								erasebr(std::string str);
	std::string								ERROR_NEED_MORE_PARAMETERS(Client &client, std::string cmd);
	std::string								ERROR_NO_SUCH_CHANNEL_EXISTS(Client cl, std::string channel);
	std::string								ERROR_CHANNEL_OPERATOR_NEEDED(Client cl, std::string channel);
	std::string								RPL_TOPIC(Client cl, std::string channel, std::string topic);
	
	bool								isClientInChannel(Channel &chan, int fd);
	bool								is_client_not_in_Channel(Channel &chan, std::string name);
	bool								is_operator_in_Channel(Client cl, Channel chan);

	std::vector<std::string> 				split(std::string str, const char *delim);

#endif
