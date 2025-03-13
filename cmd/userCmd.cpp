#include "../server/Irc.hpp"

void trim(std::string &str)
{
	str.erase(0, str.find_first_not_of(" \t\n"));
	str.erase(str.find_last_not_of(" \t\n") + 1);
}

// TODO Ensure this command follows the RFC https://www.rfc-editor.org/rfc/rfc2812.html#section-3.1.3
void Irc::userCmd(istringstream &ss, Client *client)
{
	int length = ssLength(ss);
	string user;
	string pass;
	string str;
	string realname;
	string mode;
	string unused;

	// User command must have at least 4 parameters
	// Eg.: USER guest 0 * :Ronnie (4 parameters) or USER guest 0 * :Ronnie the Bear (5 parameters)
	if (length < 4)
		return sendMsg(client->getSock(), ERR_NEEDMOREPARAMS(client->getNick(), "USER"));
	if (client->getNick().empty())
		return sendMsg(client->getSock(), NOTICE_MSG("Empty nick, please set a nick first"));
	if (client->getAuthState() >= USER_AUTH)
		return sendMsg(client->getSock(), ERR_ALREADYREGISTRED(client->getNick()));
	// ss >> user; // FIXME

	// Extract username, mode, unused fields
	if (!(ss >> user >> mode >> unused)) {
		sendMsg(client->getSock(), NOTICE_MSG("Unwanted input, USER command malformed"));
		return;
	}

	// Extract realname (which may contain spaces)
	std::string remaining;
	getline(ss, remaining);

	trim(remaining);

	// Ensure realname starts with ':'
	if (remaining.empty() || remaining[0] != ':') {
		sendMsg(client->getSock(), NOTICE_MSG("Unwanted input, realname missing"));
		return;
	}

	// Remove the ':' from realname
	realname = remaining.substr(1);

	// Validate mode and unused field
	if (mode != "0" || unused != "*") {
		sendMsg(client->getSock(), NOTICE_MSG("Unwanted input, incorrect USER format"));
		return;
	}

	client->setUser(user);
	client->setRealName(realname);
	client->setAuthState(USER_AUTH);
	//sendMsg(client->getSock(), NOTICE_MSG("\x03" "Welcome to the server!"));
	sendMsg(client->getSock(), RPL_WELCOME(client->getNick()));
}
