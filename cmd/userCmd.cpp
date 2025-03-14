#include "../server/Irc.hpp"

void trim(std::string &str)
{
	str.erase(0, str.find_first_not_of(" \t\n"));
	str.erase(str.find_last_not_of(" \t\n") + 1);
}

/**
 * @brief Handles the USER command
 *
 * Syntax:
 * - USER <username> 0 * :<realname>
 *
 * Set the user's username and realname. The username should not contain spaces.
 * The realname should start with ':' and can contain spaces.
 * 
 * @param ss Input string stream containing the command arguments.
 * @param client The client issuing the command.
 *
 * Numeric Replies:
 * - ERR_NEEDMOREPARAMS (461) - Missing parameters.
 * - ERR_ALREADYREGISTRED (462) - User is already registered.
 *
 * @see https://www.rfc-editor.org/rfc/rfc2812.html#section-3.1.3
 * 
 */
void Irc::userCmd(istringstream &ss, Client *client)
{
	int length = ssLength(ss);

	// USER command must have at least 4 parameters
	if (length < 4)
		return sendMsg(client->getSock(), ERR_NEEDMOREPARAMS(client->getNick(), "USER"));

	if (client->getNick().empty())
		return sendMsg(client->getSock(), NOTICE_MSG("Error: Nickname not set. Use /NICK <nickname> first."));

	// Prevent duplicate registration
	if (client->getAuthState() >= USER_AUTH)
		return sendMsg(client->getSock(), ERR_ALREADYREGISTRED(client->getNick()));

	string user, mode, hostname;
	if (!(ss >> user >> mode >> hostname))
		return sendMsg(client->getSock(), NOTICE_MSG("Error: Malformed USER command. Expected format: 'USER <username> 0 * :<realname>'"));

	// Validate username (should not contain spaces)
	if (user.empty() || user.find(' ') != std::string::npos)
		return sendMsg(client->getSock(), NOTICE_MSG("Error: Invalid username. The username cannot be empty or contain spaces."));

	if (mode != "0" || hostname != "*")
		return sendMsg(client->getSock(), NOTICE_MSG("Error: Invalid USER command format. Expected 'USER <username> 0 * :<realname>'"));

	string realname;
	getline(ss, realname);
	trim(realname);

	// Ensure realname starts with ':' and is not empty
	if (realname.empty() || realname[0] != ':')
		return sendMsg(client->getSock(), NOTICE_MSG("Error: Missing realname. Expected format: 'USER <username> 0 * :<realname>'"));

	// Remove ':' but preserve spaces in realname
	realname = realname.substr(1);
	trim(realname);

	client->setUser(user);
	client->setRealName(realname);
	client->setAuthState(USER_AUTH);

	sendMsg(client->getSock(), RPL_WELCOME(client->getNick()));
}
