#include <iostream>
#include "Server.hpp"
#include <cstdlib>
#include <signal.h>
#ifndef DEBUG
# define DEBUG 0
#endif
int inputValidation(int argc, char **argv)
{
	if (argc != 3)
	{
		std::cerr << "bad input" << std::endl;
		return (-1);
	}
	for (size_t i = 0; argv[1][i]; i++)
	{
		if (!std::isdigit(argv[1][i]))
		{
			std::cerr << "bad port" << std::endl;
			return (-1);
		}
	}
	int port = std::atoi(argv[1]);
	if (port < 1 || port > 65535)
	{
		std::cerr << "bad port" << std::endl;
		return (-1);
	}
	return (0);
}

void sigint_handler(int sig)
{
	std::cerr << "I AM GRACEFULLY SHUTTING DOWN BECAUSE I SAY SO" << std::endl;
	std::cerr << "LOOK HOW GRACEFUL I AM" << std::endl;
	if (sig == SIGINT)
		Server::s_active = false;
}

int main(int argc, char **argv)
{	
	signal(SIGINT, sigint_handler);

	if (inputValidation(argc, argv) < 0)
		return (1);
	Client botcl;
	BehaviourBot karen(&botcl);
	Server test(std::atoi(argv[1]), std::string(argv[2]), &karen);
	
	test.serverloop();
}
