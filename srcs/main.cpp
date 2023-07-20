/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vsedat <vsedat@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/27 12:35:36 by vsedat            #+#    #+#             */
/*   Updated: 2023/07/20 12:50:04 by vsedat           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incl/ircserv.hpp"

bool	g_interrupt = false;

void	signalHandler(int const signal) 
{
	(void)signal;
	g_interrupt = true;
	std::cout << "\b\b";
}

int	parseport(char *ag, int &port) 
{
	char	*buffer;

	if (*ag == '\0')
		return (1);
	port = strtol(ag, &buffer, 10);
	if (*buffer != '\0')
	{
		std::cout << "Error: port must contain only digits" << std::endl;
		return (1);
	}
	if (port < 0 || port > 65535)
	{
		std::cout << "Error: port must be between ]0; 65535[" << std::endl;
		return (1);
	}
	return (0);
}

int main(int ac, char **av)
{
	// PORT RANGE: 0 to 65535
	int	port;
	if (ac != 3)
	{
		std::cout << "Usage: ./ircserv <port> <password>" << std::endl;
		return (1);
	}
	signal(SIGINT, signalHandler);
	if (parseport(av[1], port))
		return (1);

	try
	{
		Server server(port, av[2]);
		server.launch();
	}
	catch (const std::exception &ecp) {
		std::cerr << ecp.what() << std::endl;
    }
	return 0;
}