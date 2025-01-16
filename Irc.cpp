#include "Irc.hpp"

Irc::Irc(void)
{
	cmds["JOIN"] = &Irc::joinCmd;
	cmds["TOPIC"] = &Irc::topicCmd;
	cmds["PRIVMSG"] = &Irc::privmsgCmd;
	cmds["PASS"] = &Irc::passCmd;
	cmds["NICK"] = &Irc::nickCmd;
	cmds["USER"] = &Irc::userCmd;
	cmds["PART"] = &Irc::partCmd;
	cmds["MODE"] = &Irc::modeCmd;
	cmds["INVITE"] = &Irc::inviteCmd;
	cmds["QUIT"] = &Irc::quitCmd;
	cmds["KICK"] = &Irc::kickCmd;
	cout << CYAN "Server started (Ctrl+c to quit)" END << endl;
}

Irc:: ~Ird(void)
{
	for (map<int, Cliente*>::iterator it = _clients.begin(); it != _clients.end(); it++)
		deleteClient(it);
	for (vector<Channel*>iterator it = _serverChannels.begin(); it != _serverChannels.end(); it++)
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
	
	_prot = num;

	_servPassWord = av[2];
	for (size_t i = 0; i < _serverPassWord.size(); i++)
		if (iswspace_PassWord[i])
			throw runtime_error("Invalid password!");
}

 void Irc::saveData(void) const {
	// Sae Client
	string filename_client = "file_client.txt";
	ofstream outfile_client(filename_client.c_str());
	if (!outFile_client) {
		cerr<< "Error ao abri o arquivo para escrita: " << filename_client << endl;
		return;
	}
	outFile_clinet << "fd_cl | nick | autenticated | buffer"  << endl;
	map<int, Client*>::const_iterator it;
	for (it = _client.begin(); it != _client.end(); it++){
		outFile_client \
		<< it->first << " | " \
		<< it->second->getNick() << " | " \
		<< it->second->isAuthenticated() << " | " \
		<< it->second->_buffer << " | " \
		<< endl;
	}
	outFile_client.close();


 }