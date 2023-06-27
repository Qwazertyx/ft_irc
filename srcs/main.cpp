/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vsedat <vsedat@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/27 12:35:36 by vsedat            #+#    #+#             */
/*   Updated: 2023/06/27 13:20:55 by vsedat           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"

int main(int ac, char **av)
{
	// PORT RANGE: 1024 to 65535
	try
	    if (ac != 3)
            throw std::runtime_error("Usage: ./ircserv <port> <password>");
	{
		Server server(av[1], av[2]);
		server.start();
		return 0;
	}
	catch (const std::exception &ex) {
		std::cerr << ex.what() << std::endl;
		return 1;
    }
}