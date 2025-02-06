#include "Irc.hpp"

static std::vector<std::string> split(const std::string& str,
										const std::string& delimiters,
										bool keepEmpty);

Irc::Irc(void)
{
	//cmds["JOIN"] = &Irc::joinCmd;
	// cmds["TOPIC"] = &Irc::topicCmd;
	// cmds["PRIVMSG"] = &Irc::privmsgCmd;
	cmds["PASS"] = &Irc::passCmd;
	//cmds["NICK"] = &Irc::nickCmd;
	// cmds["USER"] = &Irc::userCmd;
	// cmds["PART"] = &Irc::partCmd;
	// cmds["MODE"] = &Irc::modeCmd;
	// cmds["INVITE"] = &Irc::inviteCmd;
	// cmds["QUIT"] = &Irc::quitCmd;
	// cmds["KICK"] = &Irc::kickCmd;
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
		throw runtime_error("Invalid password!");
	
	_port = num;
	_serverPassWord = av[2];
	for (size_t i = 0; i < _serverPassWord.size(); i++)
		if (iswspace(_serverPassWord[i]))
			throw runtime_error("Invalid password!");
}

//void Irc::joinCmd(istringstream &ss, Client* client){
//  	ss.str("Func Join executed");
//  }

void Irc::passCmd(istringstream &ss, Client* client){
	std::vector <string> splited_input;
	splited_input = split(ss.str(), " ", false);
	for (size_t i = 0; i < splited_input.size(); i++)
		cout << splited_input[i] << endl;
	if(splited_input.size() == 2){
		client->setPassWord(splited_input[1]);
	}
}

static std::vector<std::string> split(const std::string& str,
										const std::string& delimiters,
										bool keepEmpty) {
	std::vector<std::string> tokens;
	std::string::size_type pos = 0;
	std::string::size_type prev = 0;

	while ((pos = str.find_first_of(delimiters, prev)) != std::string::npos) {
		if (keepEmpty || pos > prev) {
			tokens.push_back(str.substr(prev, pos - prev));
		}
		prev = pos + 1;
	}

	if (prev < str.length()) {
		tokens.push_back(str.substr(prev));
	} else if (keepEmpty && prev == str.length()) {
		tokens.push_back("");
	}

	return tokens;
}

//void Irc::nickCmd(istringstream &ss, Client* client){
//  (void)client;
//  vector<string> ss_contents = split(ss.str(), "' '\t", true);
//  if (ss_contents.size() > 1) {
//    ss_contents = split(ss_contents[1], "' ''\t'", true);
//  }
//  for (size_t it = 0; it <= ss_contents.size(); it++)
//	  cout << ss_contents[it] << endl;
//  string nick = ss_contents[0];
//  cout << "What user sent: " << ss.str() << endl;
//  getline(ss, nick);
//  nick = nick.substr(1, nick.size() - 1);
//  string::iterator end_pos = remove(nick.begin(), nick.end(), ' ');
//  nick.erase(end_pos, nick.end());
//  cout << "Nick: " << nick << endl;
//}

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

