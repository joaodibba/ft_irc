#include "Irc.hpp"

Irc::Irc(void)
{
	cmds["JOIN"] = &Irc::joinCmd;
	// cmds["TOPIC"] = &Irc::topicCmd;
	// cmds["PRIVMSG"] = &Irc::privmsgCmd;
	cmds["PASS"] = &Irc::passCmd;
	// cmds["PART"] = &Irc::partCmd;
	// cmds["MODE"] = &Irc::modeCmd;
	cmds["NICK"] = &Irc::nickCmd;
	cmds["USER"] = &Irc::userCmd;
	cmds["INVITE"] = &Irc::inviteCmd;
	cmds["QUIT"] = &Irc::quitCmd;
	cmds["KICK"] = &Irc::kickCmd;
	cout << CYAN "Server started (Ctrl+c to quit)" END << endl;
}

Irc::~Irc(void)
{
	for (map<int, Client*>::iterator it = _clients.begin(); it != _clients.end(); it++)
		deleteClient(it);
	for (vector<Channel*>::iterator it = _serverChannels.begin(); it != _serverChannels.end(); it++)
		delete *it;
	if(epfds)
		delete epfds;
	cout << CYAN "Sever terminated" END << endl;
}

void Irc::setPortAndPassword(char **av)
{
	char *end;
	int num = strtol(av[1], &end, 10);

	if (*end || num <= 0 || num >= 65535)
		throw runtime_error("Invalid port!");
	
	_port = num;
	_serverPassWord = av[2];
	for (size_t i = 0; i < _serverPassWord.size(); i++)
		if (iswspace(_serverPassWord[i]))
			throw runtime_error("Invalid password!");
}

void Irc::saveData(void) const {
	// Save Client
	string filename_client = "file_client.txt";
	ofstream outFile_client(filename_client.c_str());
    if (!outFile_client) {
    	cerr << "Erro ao abrir o arquivo para escrita: " << filename_client << endl;
    	return;
	}
    outFile_client << "fd_cl | nick | autenticated | buffer" << endl;
	map<int, Client*>::const_iterator it;
    for (it = _clients.begin(); it != _clients.end(); ++it) {
        outFile_client 	\
		<< it->first << " | " \
		<< it->second->getNick() << " | " \
		<< it->second->isAuthenticated() << " | " \
		<< it->second->_buffer << " | " \
		<< endl;
    }
   	outFile_client.close();
	
	// Save Requests
	string filename_requests =  "file_request.txt";
	ofstream outFile_requests(filename_requests.c_str());
    if (!outFile_requests) {
    	cerr << "Erro ao abrir o arquivo para escrita: " << filename_requests << endl;
    	return;
	}
    outFile_requests << "fd_client | string" << endl;
	map<int, string>::const_iterator it_r;
    for (it_r = requests.begin(); it_r != requests.end(); ++it_r) {
        outFile_requests \
		<< it_r->first << " | " \
		<< it_r->second << endl;
    }
	outFile_requests.close();
	
	// Save ServerChannel
	string filename_severChannel = "file_ServerChannel.txt";
	ofstream outFile_serverChannel(filename_severChannel.c_str());
    if (!outFile_serverChannel) {
    	cerr << "Erro ao abrir o arquivo para escrita: " << filename_severChannel << endl;
    	return;
	}
    outFile_serverChannel << "ChaneelName | topic | operator | Mode | Num Users | Password" << endl;
   	vector<Channel*>::const_iterator it_sc;
	for (it_sc = _serverChannels.begin(); it_sc != _serverChannels.end(); ++it_sc) {
        Channel *channel = *it_sc;
		outFile_serverChannel \
		<< channel->getChannelName() << " | " \
		<< channel->getChannelTopic() << " | " \
		<< channel->getChannelModes() << " | " \
		<< channel->getMaxUsersNumber() << " | " \
		<< channel->getChannelPassword() << " | " \
		<< endl;

	}
	outFile_serverChannel.close();
}

