#include <iostream>
#include "Server.hpp"
#include <cstdlib>

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
#include <cstdlib>
int main(int argc, char **argv)
{
	if (inputValidation(argc, argv) < 0)
		return (1);
	Server test(std::atoi(argv[1]), std::string(argv[2]));

	test.serverloop();
}
