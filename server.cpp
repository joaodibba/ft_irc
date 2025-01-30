#include "Irc.hpp"

bool running = true;
 void handler(int signal) {
	(void)signal;
	running = false;
	cout << endl;
 }

 void Irc::receiveRequest(int targetFd) {
	char buffer[30000];
	bzero(buffer, sizeof(buffer));
	istringstream ss;
	Client *client = findClient(targetFd);

	if (recv(targetFd, &buffer, sizeof(buffer), 0) <= 0)
		quitCmd(ss, client);
	client->_buffer += string (buffer);
	if (client->_buffer.find('\n') == string::npos)
		return;
	requests.insert(make_pair(targetFd, client->_buffer));
	epfds->modFd(targetFd, EPOLLOUT);
	client->_buffer.clear();	
 }

 void Irc::sendResponse(int targetFd){
	Client* client = findClient(targetFd);
	map<int, string>::iterator it = requests.find(targetFd);

	istringstream RequestSs(it->second);
	string tmpLine;
	string cmdName;

	cout << WHITE << RequestSs.str() << END << endl;
	while (getline(RequestSs, tmpLine)) {
		istringstream lineSs(tmpLine);
		lineSs >> cmdName;
		cout << "Command: " << cmdName << endl; //my
		if (cmdName == "CAP")
			continue;
		if (!client->isAuthenticated() && cmdName != "PASS" && cmdName != "NICK" &&
			cmdName != "USER" && cmdName != "QUIT") {
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

 int Irc::run_server(char **av) {
	struct epoll_event evs[MAX_EVENTS];
	try {
		signal(SIGINT, handler);
		setPortAndPassword(av);
		initNetwork();

		int event_count = 0;
		int j = 0;

		while (running) {
			logger(1,j);
			event_count = epoll_wait(epfds->getEpSock(), evs, MAX_EVENTS, -1);
			if (event_count == -1)
				throw runtime_error("epoll_wait");
			logger(2, event_count);
			for (int i = 0 ; i < event_count; i++) {
				logger(3, evs[i].data.fd);
				logger(4, evs[i].events);
				cout << "Listen Fd: " << evs[i].data.fd << endl;
				// se o fd do evs for o do servidor, significa que fd e um novo cliente
				if (isNewClient(evs[i].data.fd) && evs[i].events & EPOLLIN)
					acceptClient(evs[i].data.fd);
				else if (evs[i].events & EPOLLIN)
					receiveRequest(evs[i].data.fd);
				else if (evs[i].events & EPOLLOUT)
					sendResponse(evs[i].data.fd);
				else
					break;
			}
			saveData();
			j++;
		}
		logger(5,0);
	}
	catch(const exception& e) {
		if (running)
			cerr << "Error: " << e.what() << "  " << '\n';
	}
	return 0;
 }