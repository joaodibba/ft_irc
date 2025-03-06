#include "Irc.hpp"


/**
 * @brief Checks if the given file descriptor corresponds to a new client connection.
 * 
 * @param targetFd The file descriptor to check.
 * 
 * @return true If the file descriptor matches the server socket, indicating a new client connection.
 * @return false If the file descriptor does not match the server socket.
 */
bool Irc::isNewClient(int targetFd){
	return ((targetFd == _serverSock) ? true : false);
}

/**
 * @brief Sets the given file descriptor to non-blocking mode.
 *
 * This function sets the file descriptor to non-blocking mode
 * using the fcntl system call. If the operation fails, it throws a runtime_error.
 *
 * @param fd the file descriptor to be set to non-blocking mode.
 *
 * @throws std::runtime_error if the operation fails.
 */
void Irc::setNonBlocking(int fd) {
	if (fcntl(fd, F_SETFL, O_NONBLOCK) == -1)
		throw runtime_error("Failed to set the socket to nonBlocking");
}

/**
 * @brief Accepts a new client connection.
 *
 * This function is called when a new client tries to connect to the server.
 * It accepts the connection and adds the new client to the epoll instance for event-driven I/O.
 * 
 * @param serverFd The file descriptor of the server socket.
 * 
 * @throws std::runtime_error if the connection cannot be accepted.
*/
void Irc::acceptClient(int serverFd){
	int newSock;
	struct sockaddr_in address;
	socklen_t addrlen = sizeof(address);
	bzero(&address, addrlen);
	if ((newSock = accept(serverFd, (struct sockaddr *)&address, &addrlen)) < 0)
		throw runtime_error("Failed to accept connection");

	setNonBlocking(newSock);
	epfds->addFd(newSock, EPOLLIN | EPOLLERR | EPOLLHUP ); // OBS EPOLLOUT incluido para teste //!FIXME
	_clients.insert(make_pair(newSock, (new Client(newSock))));
	cout << MAGENTA "Opening connection, fd: " << newSock << END << endl;
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
	address.sin_addr.s_addr = INADDR_ANY; // indica que o socket pode receber pacotes de qualquer endereco
	address.sin_port = htons(_port); // indica a porta que o socket vai escutar
	
	_serverSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); // cria um socket do tipo TCP'
	if (_serverSock == -1)
		throw runtime_error("Failed to create socket");
	cout << CYAN "Server socket created: " << _serverSock << endl;

	epfds = new EpollManager();
	//EPOLLIN: Indica que esta pronto para leitura
	//EPOLLET: Indica que sera notificado quando houver alteracao de status
	epfds->addFd(_serverSock, EPOLLIN | EPOLLET);

	int enable = 1;
	setNonBlocking(_serverSock); 
	if (setsockopt(_serverSock, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(enable)) < 0) // configura o socket para reutilizacao de endereco local
		throw runtime_error("Failed to set setsockopt to SO_REUSEADDR");

	if (bind(_serverSock, (struct sockaddr *)&address, addrlen) < 0) // associa o socket ao endereco e a porta especificadas 	
		throw runtime_error("Failed to bind socket to a port");
	cout << CYAN "Server socket binded to port: " << _port << END << endl;

	if (listen(_serverSock, BACKLOG) < 0) // coloca o socket do servidor no modo escuta, permitindo aceitar novos clientes no limite de BACKLOG
		throw runtime_error("Failed to listen to socket");
	cout << CYAN "Server socket listening on port: " << _port << END << endl;
}