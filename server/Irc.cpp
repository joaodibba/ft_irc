#include "Irc.hpp"

Irc::Irc(void)
{
	cmds["JOIN"] = &Irc::joinCmd;
	cmds["TOPIC"] = &Irc::topicCmd;
	cmds["PRIVMSG"] = &Irc::privmsgCmd;
	cmds["PASS"] = &Irc::passCmd;
	cmds["PART"] = &Irc::partCmd;
	cmds["MODE"] = &Irc::modeCmd;
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

void Irc::setPort(int port)
{

    if (port <= 0 || port >= 65535)
        throw std::runtime_error("Invalid port!");

    _port = port;
}

void Irc::setPassword(string password)
{
    for (size_t i = 0; i < password.size(); i++) {
        if (iswspace(password[i]))
            throw std::runtime_error("Invalid password! No whitespaces allowed.");
        if (password[i] == ',')
            throw std::runtime_error("Invalid password! No commas allowed.");
    }
    _serverPassWord = password;
}

void Irc::saveData(void) const {

	// Save Clients
    std::string filename_client = "clients.csv";
    std::ofstream outfile_client(filename_client.c_str(), std::ios::app);
    if (!outfile_client) {
        std::cerr << "Error: Unable to open file for writing: " << filename_client << std::endl;
        return;
    }

    // CSV Header
    outfile_client << "fd_cl,nick,authenticated,buffer" << std::endl;

    for (std::map<int, Client*>::const_iterator it = _clients.begin(); it != _clients.end(); ++it) {
        if (!it->second) continue; // Ensure pointer is valid

        outfile_client << it->first << ","
                       << it->second->getNick() << ","
                       << it->second->isAuthenticated() << ","
                       << "\"" << it->second->_buffer << "\"" // Wrap in quotes in case of commas
                       << std::endl;
    }
    outfile_client.close();

    // Save Requests
    std::string filename_requests = "requests.csv";
    std::ofstream outFile_requests(filename_requests.c_str(), std::ios::app);
    if (!outFile_requests) {
        std::cerr << "Error: Unable to open file for writing: " << filename_requests << std::endl;
        return;
    }

    // CSV Header
    outFile_requests << "fd_client,string" << std::endl;

    for (std::map<int, std::string>::const_iterator it_r = requests.begin(); it_r != requests.end(); ++it_r) {
        outFile_requests << it_r->first << ",\"" << it_r->second << "\"" << std::endl;
    }
    outFile_requests.close();

    // Save Server Channels
    std::string filename_serverChannel = "server_channels.csv";
    std::ofstream outFile_serverChannel(filename_serverChannel.c_str(), std::ios::app);
    if (!outFile_serverChannel) {
        std::cerr << "Error: Unable to open file for writing: " << filename_serverChannel << std::endl;
        return;
    }

    // CSV Header
    outFile_serverChannel << "ChannelName,Topic,Mode,MaxUsers,Password" << std::endl;

    for (std::vector<Channel*>::const_iterator it_sc = _serverChannels.begin(); it_sc != _serverChannels.end(); ++it_sc) {
        Channel *channel = *it_sc;
        if (!channel) continue; // Ensure pointer is valid

        outFile_serverChannel
            << channel->getChannelName() << ","
            << "\"" << channel->getChannelTopic() << "\"," // Wrap in quotes in case of commas
            << channel->getChannelModes() << ","
            << channel->getMaxUsersNumber() << ","
            << "\"" << channel->getChannelPassword() << "\"" // Password might have commas
            << std::endl;
    }
    outFile_serverChannel.close();
}
