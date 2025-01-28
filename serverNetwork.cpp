#include "Irc.hpp"

bool Irc::isNewClient(int targetFd){
	return ((targetFd == _serverSock) ? true : false);
}

void Irc::setNonBloking(int *ptr) {
	int fd = fcntl(*ptr, F_SETFL, O_NONBLOCK);
	if (fd == -1)
		throw runtime_error("Failed to set the soket to nonBlocking");
}

void Irc::acceptClient(int serverFd){
	int newSock;
	struct sockaddr_in address;
	socklen_t addrlen = sizeof(address);
	bzero(&address, addrlen);

	if ((newSock = accept(serverFd, (struct sockaddr *)&address, &addrlen)) < 0)
		throw runtime_error("Failed to accept connection");

	setNonBloking(&newSock);
	epfds->addFd(newSock, EPOLLIN | EPOLLERR | EPOLLHUP);
	_clients.insert(make_pair(newSock, (new Client(newSock))));
	cout << MAGENTA "Opening connection, fd; " << newSock << END << endl;
}

void Irc::initNetwork(void)
{
	struct sockaddr_in address;
	int addrlen = sizeof(address);
	bzero(&address, addrlen);

	address.sin_family = AF_INET;
	address.sin_addr.s_addr = htons(INADDR_ANY);
	address.sin_port = htons(_port);

	_serverSock = socket(AF_INET, SOCK_STREAM, 0);
	if (_serverSock == -1)
		throw runtime_error("Failed to create socket");
	
	epfds = new EpollManager();
	epfds->addFd(_serverSock, EPOLLIN | EPOLLET);

	int enable = 1;
	setNonBloking(&_serverSock);
	if (setsockopt(_serverSock, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0)
		throw runtime_error("Failed to set setsockopt to SO_REUSEADDR");
	if (bind(_serverSock, (struct sockaddr *)&address, addrlen) < 0)
		throw runtime_error("Failed to bind socket to a port");
	if (listen(_serverSock, BACKLOG) < 0)
		throw runtime_error("Failed to listen to socket");
}