#include "Irc.hpp"

void logger(int type, int data)
{
	static int mode;
	if (!type)
		mode = !mode;
	if (type == 1 && mode)
		std::cout << BLUE << data << " Input received, Waiting for event..." END << std::endl;
	else if (type == 2 && mode)
		std::cout << "Fds received: " << data << std::endl;
	else if (type == 3)
		std::cout << GREEN "Received socket n: " << data << "with event ";
	//else if (type == 4 && mode)
	//{
	//	string eventString;
	//	if (data & EPOLLIN)
	//		eventString = "EPOLLIN";
	//	else if (data & EPOLLOUT)
	//		eventString = "EPOLLOUT"
	//	else if (data & EPOLLERR || EPOLLRDHUB || data & EPOLLHUP)
	//		EVENTsTRING = "EPOLLERR || EPOLLRDHUP || EPOLLHUP";
	//	else
	//		eventString = "INVALID";
	//	cout << eventString << "\n" END << endl;
	//}
	else if (type == 5 && mode)
		std::cout << RED "Reached uncommon place" END << std::endl;
}


int main(int ac, char **av)
{
	if (ac != 2 || !av[1][0])
		return (std::cerr << "./ircserv <port>" << std::endl), 1;

	static Irc irc;
	//logger(0, 0);
	return (irc.run_server(av));
}