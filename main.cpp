#include "Irc.hpp"

void logger(int type, int data)
{
	static int mode;
	if (!type)
		mode = !mode;
	if (type == 1 && mode)
		cout << BLUE << data << " Input received, Waiting for event..." END << endl;
	else if (type == 2 && mode)
		cout << "Fds received: " << data << endl;
	else if (type == 3)
		cout << GREEN "Received socket n: " << data << "with event ";
	else if (type == 4 && mode)
	{
		string eventString;
		if (data & EPOLLIN)
			eventString = "EPOLLIN";
		else if (data & EPOLLOUT)
			eventString = "EPOLLOUT"
		else if (data & EPOLLERR || EPOLLRDHUB || data & EPOLLHUP)
			EVENTsTRING = "EPOLLERR || EPOLLRDHUP || EPOLLHUP";
		else
			eventString = "INVALID";
		cout << eventString << "\n" END << endl;
	}
	else if (type == 5 && mode)
		cout << RED "Reached uncommon place" END << endl;
}


int main(int ac, char **av)
{
	if (ac=!3 || !av[1][0] || !av[2][0])
		return (cerr << "./ircserv <port> <password" << endl), 1;

	static Irc irc;
	loger(0, 0);
	irc.run_server(av);
	return (0);
}