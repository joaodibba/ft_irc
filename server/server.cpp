#include "Irc.hpp"

bool running = true;

void handler(int signal)
{
	(void)signal;
	running = false;
	cout << endl;
}

/**
 * @brief Receives a request from a client, processes it, and prepares for further actions.
 * 
 * This function listens for incoming data from a client connection (represented by the file descriptor `targetFd`), 
 * appends the received data to the client's buffer, and processes the request once a complete line (terminated by a newline character) is received.
 * If the client disconnects or an error occurs while reading, the function will quit the client connection.
 * After successfully receiving and processing a complete request, the function marks the file descriptor for writing (EPOLLOUT) to handle the response later.
 * 
 * @param targetFd The file descriptor representing the client connection from which data is received.
 * 
 * @note The client’s buffer is cleared after processing the request.
 * @note If no newline character is found in the received data, the function will return and wait for more data to arrive.
 */
void Irc::receiveRequest(int targetFd)
{

	char buffer[BUFFER_SIZE];
	memset(buffer, 0, sizeof(buffer));

	istringstream ss;
	Client *client = findClient(targetFd);

	ssize_t bytesRead = recv(targetFd, &buffer, sizeof(buffer), 0);
	if (bytesRead <= 0)
		return quitCmd(ss, client);

	client->setBuffer(client->getBuffer() + string(buffer, bytesRead));
	if (client->getBuffer().find('\n') == string::npos)
		return;

	requests.insert(make_pair(targetFd, client->getBuffer()));

	epfds->modFd(targetFd, EPOLLOUT);

	client->setBuffer("");
}

void Irc::sendResponse(int targetFd)
{
	Client *client = findClient(targetFd);
	map<int, string>::iterator it = requests.find(targetFd);

	istringstream RequestSs(it->second);
	string tmpLine;
	string cmdName;

	cout << WHITE << RequestSs.str() << END << endl;
	while (getline(RequestSs, tmpLine))
	{
		istringstream lineSs(tmpLine);
		lineSs >> cmdName;
		if (cmdName == "CAP")
			continue;
		if (cmdName == "INFO")
		{
			sendMsg(client->getSock(), RPL_INFO());
			continue;
		}
		if (!client->isAuthenticated() && cmdName != "PASS" && cmdName != "NICK" &&
			cmdName != "USER" && cmdName != "QUIT")
		{
			sendMsg(client->getSock(), ERR_NOTREGISTERED(client->getNick()));
			continue;
		}
		if (this->cmds.find(cmdName) != this->cmds.end())
			(this->*(this->cmds[cmdName]))(lineSs, client);
		else
			sendMsg(client->getSock(), ERR_UNKNOWNCOMMAND(client->getNick(), cmdName));
	}
	requests.erase(it);
	epfds->modFd(targetFd, EPOLLIN);
}

int Irc::run_server(char **av)
{
	struct epoll_event evs[MAX_EVENTS];
	try
	{
		signal(SIGINT, handler);
		setPort(atoi(av[1]));
		setPassword(av[2]);
		initNetwork();

		int event_count = 0;
		int j = 0;

		while (running)
		{
			for (std::vector<Channel *>::iterator it = _serverChannels.begin(); it != _serverChannels.end(); ++it)
				(*it)->revoke_invites();
			logger(1, j);
			event_count = epoll_wait(epfds->getEpSock(), evs, MAX_EVENTS, -1);
			if (event_count == -1)
				throw runtime_error("epoll_wait");
			logger(2, event_count);
			for (int i = 0; i < event_count; i++)
			{
				logger(3, evs[i].data.fd);
				logger(4, evs[i].events);
				if (isNewClient(evs[i].data.fd) && evs[i].events & EPOLLIN) // se o fd do evs for o do servidor, significa que fd e um novo cliente
					acceptClient(evs[i].data.fd);
				else if (evs[i].events & EPOLLIN)
					receiveRequest(evs[i].data.fd);
				else if (evs[i].events & EPOLLOUT)
					sendResponse(evs[i].data.fd);
				else
					break;
			}
			// saveClients();
			// saveRequests();
			// saveChannels();
			// saveChannelUsers();
			// saveChannelInvites();
			j++;		
		}
		logger(5, 0);
	}
	catch (const exception &e)
	{
		if (running)
			cerr << "Error: " << e.what() << "  " << '\n';
	}
	return 0;
}