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
    cmds["INFO_CHANNEL"] = &Irc::infoChannel;
    cout << CYAN "Server started (Ctrl+c to quit)" END << endl;
}

Irc::~Irc(void)
{
    for (map<int, Client *>::iterator it = _clients.begin(); it != _clients.end(); it++)
        deleteClient((*it).second);
    for (vector<Channel *>::iterator it = _serverChannels.begin(); it != _serverChannels.end(); it++)
        delete *it;
    if (epfds)
        delete epfds;
    cout << CYAN "Sever terminated" END << endl;
}

void Irc::setPort(int port)
{
    if (port <= 0 || port > MAX_TCP_PORT)
        throw std::runtime_error("Invalid port! Port must be between 1 and 65535.");
    _port = port;
}

void Irc::setPassword(string password)
{
    for (size_t i = 0; i < password.size(); i++)
    {
        if (iswspace(password[i]))
            throw std::runtime_error("Invalid password! No whitespaces allowed.");
        if (password[i] == ',')
            throw std::runtime_error("Invalid password! No commas allowed.");
    }
    _serverPassWord = password;
}
