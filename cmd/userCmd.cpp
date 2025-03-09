#include "../server/Irc.hpp"

void Irc::userCmd(istringstream &ss, Client *client) {
	int length = ssLength(ss);
	string user;
	string pass;
	string str;

	if (length != 4)
		return sendMsg(client->getSock(), ERR_NEEDMOREPARAMS(client->getNick(), "USER"));
	if (client->getNick().empty())
		return sendMsg(client->getSock(), NOTICE_MSG("Empty nick, please set a nick first"));
	if (client->isAuthenticated())
		return sendMsg(client->getSock(), ERR_ALREADYREGISTRED(client->getNick()));
	// ss >> user;

	if (!(str == "0" && ss >> str && str == "*" && ss >> str && str == ":realname"))
		sendMsg(client->getSock(), NOTICE_MSG("Unwaned input, username still changed"));
	
	client->setUser(user);
	client->authenticated();
	sendMsg(client->getSock(), NOTICE_MSG("\x03" "Welcome to the server!"));
}
