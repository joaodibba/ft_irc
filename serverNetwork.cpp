#include "Irc.hpp"

bool Irc::isNewClient(int targetFd){
	return ((targetFd == _serverSock) ? true : false);
}

/**
 * @brief Sets the given file descriptor to non-blocking mode.
 *
 * This function sets the file descriptor pointed to by the given pointer to non-blocking mode
 * using the fcntl system call. If the operation fails, it throws a runtime_error.
 *
 * @param ptr Pointer to the file descriptor to be set to non-blocking mode.
 *
 * @throws std::runtime_error if the operation fails.
 */
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
	epfds->addFd(newSock, EPOLLIN | EPOLLERR | EPOLLHUP ); // OBS EPOLLOUT incluido para teste
	_clients.insert(make_pair(newSock, (new Client(newSock))));
	cout << MAGENTA "Opening connection, fd; " << newSock << END << endl;
}

/**
 * @brief Initializes the network for the IRC server.
 * 
 * This function sets up the server socket, configures it for non-blocking mode,
 * and binds it to the specified port. It also sets the socket options and 
 * prepares it to listen for incoming connections. Additionally, it adds the 
 * server socket to the epoll instance for event-driven I/O.
 * 
 * @throws std::runtime_error if socket creation, setting socket options, 
 * binding, or listening fails.
 */
void Irc::initNetwork(void)
{
	struct sockaddr_in address;
	int addrlen = sizeof(address);
	bzero(&address, addrlen);

	address.sin_family = AF_INET; // indica que o socket e do tipo internet (ipv4)
	address.sin_addr.s_addr = htons(INADDR_ANY); // indica que o socket pode receber pacotes de qualquer endereco
	address.sin_port = htons(_port); // indica a porta que o socket vai escutar
	
	_serverSock = socket(AF_INET, SOCK_STREAM, 0); // cria um socket do tipo TCP'
	cout << CYAN "Server socket created: " << _serverSock << endl;
	if (_serverSock == -1)
		throw runtime_error("Failed to create socket");
	
	epfds = new EpollManager();
	//EPOLLIN: Indica que esta pronto para leitura
	//EPOLLET: Indica que sera notificado quando houver alteracao de status
	epfds->addFd(_serverSock, EPOLLIN | EPOLLET);

	int enable = 1;
	setNonBloking(&_serverSock); 
	if (setsockopt(_serverSock, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0) // configura o socket para reutilizacao de endereco local
		throw runtime_error("Failed to set setsockopt to SO_REUSEADDR");
	if (bind(_serverSock, (struct sockaddr *)&address, addrlen) < 0) // associa o socket ao endereco e a porta especificadas 	
		throw runtime_error("Failed to bind socket to a port");
	if (listen(_serverSock, BACKLOG) < 0) // coloca o socket do servidor no modo escuta, permitindo aceitar novos clientes no limite de BACKLOG
		throw runtime_error("Failed to listen to socket");
}