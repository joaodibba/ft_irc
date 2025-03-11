#include "../server/Irc.hpp"

// TODO Ensure this command follows the RFC https://www.rfc-editor.org/rfc/rfc2812.html#section-3.1.3
void Irc::userCmd(istringstream &ss, Client *client)
{
	int length = ssLength(ss);
	string user;
	string pass;
	string str;

	if (length != 4)
		return sendMsg(client->getSock(), ERR_NEEDMOREPARAMS(client->getNick(), "USER"));
	if (client->getNick().empty())
		return sendMsg(client->getSock(), NOTICE_MSG("Empty nick, please set a nick first"));
	if (client->getAuthenticated())
		return sendMsg(client->getSock(), ERR_ALREADYREGISTRED(client->getNick()));
	// ss >> user; // FIXME

	if (!(str == "0" && ss >> str && str == "*" && ss >> str && str == ":realname"))
		sendMsg(client->getSock(), NOTICE_MSG("Unwaned input, username still changed"));

	client->setUser(user);
	client->setAuthenticated(true);
	//sendMsg(client->getSock(), NOTICE_MSG("\x03" "Welcome to the server!"));
	sendMsg(client->getSock(), RPL_WELCOME(client->getNick()));
}
