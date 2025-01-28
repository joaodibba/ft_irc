#include "Irc.hpp"

Irc::Irc(void)
{
//	cmds["JOIN"] = &Irc::joinCmd;
//	cmds["TOPIC"] = &Irc::topicCmd;
//	cmds["PRIVMSG"] = &Irc::privmsgCmd;
//	cmds["PASS"] = &Irc::passCmd;
//	cmds["NICK"] = &Irc::nickCmd;
//	cmds["USER"] = &Irc::userCmd;
//	cmds["PART"] = &Irc::partCmd;
//	cmds["MODE"] = &Irc::modeCmd;
//	cmds["INVITE"] = &Irc::inviteCmd;
//	cmds["QUIT"] = &Irc::quitCmd;
//	cmds["KICK"] = &Irc::kickCmd;
	//std::cout << "Server started (Ctrl+c to quit)" << std::endl;
}

int Irc::run_server(char **av)
{
	_port = atoi(av[1]);
	//_serverPassWord = av[2];
	 //initNetwork();
	 // Create a socket
	_serverSock = socket(AF_INET, SOCK_STREAM, 0);
	if (_serverSock == -1)
		return (std::cerr << "Failed to create socket" << std::endl), 1;

	std::cout << "Socket created" << std::endl;

	//Server address
	struct sockaddr_in serverAddr;
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(_port); // Port to network byte order
	serverAddr.sin_addr.s_addr = INADDR_ANY; // Listen to any address

	// Bind
	if (bind(_serverSock, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0)
	  		return (std::cerr << "Failed to bind socket to a port" << std::endl), 1;

	std::cout << "Bind done" << std::endl;

	// Listen
	if (listen(_serverSock, 3) < 0)
	  		return (std::cerr << "Failed to listen to socket" << std::endl), 1;

	std::cout << "Waiting for incoming connections on port" << _port << "..." << std::endl;

	fd_set read_fds;
	FD_ZERO(&read_fds);
	FD_SET(_serverSock, &read_fds);

	int max_fd = _serverSock;

	while (1)
	{
		fd_set tmp_fds = read_fds;
		int activity = select(max_fd + 1, &tmp_fds, NULL, NULL, NULL);
		if (activity < 0)
			return (std::cerr << "Select error" << std::endl), 1;

		if (FD_ISSET(_serverSock, &tmp_fds))
		{
			int newSock;
			struct sockaddr_in address;
			socklen_t addrlen = sizeof(address);
			bzero(&address, addrlen);

			if ((newSock = accept(_serverSock, (struct sockaddr *)&address, &addrlen)) < 0)
				return (std::cerr << "Failed to accept connection" << std::endl), 1;

			std::cout << "New connection, socket fd is " << newSock << ", ip is " << inet_ntoa(address.sin_addr) << ", port is " << ntohs(address.sin_port) << std::endl;

			FD_SET(newSock, &read_fds);
			if (newSock > max_fd)
				max_fd = newSock;
		}

		// Handle data from existing clients
		for (int i = 0; i <= max_fd; i++) {
			if (FD_ISSET(i, &tmp_fds)) {
				char buffer[512];
				int bytes_read = recv(i, buffer, sizeof(buffer) - 1, 0);
//    			if (bytes_read < 0) {
//    				std::cout << "Client disconnected!\n";
//    				close(i);
//    				FD_CLR(i, &read_fds);
//				}
				if (bytes_read > 0) {
					buffer[bytes_read] = '\0';
					std::cout <<"User with FD: " << i << " sent: " << buffer << "\n";
				}
			}
		}
	}

	close(_serverSock);
 	return (0);
}