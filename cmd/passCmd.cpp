#include "../server/Irc.hpp"

/**
 * @brief used to set the password for the client
 * 
 * Syntax:
 * - PASS <password>
 * 
 * Authenticate the client with the server. The client must provide the correct password
 * to proceed with the registration process. If the password is incorrect, the client is
 * disconnected from the server.
 * 
 * @param ss input string stream with the password
 * @param client the client trying to authenticate
 * 
 * Numeric Replies:
 * - ERR_NEEDMOREPARAMS (461) - Missing parameters.
 * - ERR_ALREADYREGISTRED (462) - The client is already registered.
 * - ERR_PASSWDMISMATCH (464) - The provided password is incorrect.
 * 
 * @see https://www.rfc-editor.org/rfc/rfc2812.html#section-3.1.1
 *  
*/

void Irc::passCmd(istringstream &ss, Client *client)
{
	string str;

	if (client->getAuthState() >= USER_AUTH)
		return sendMsg(client->getSock(), ERR_ALREADYREGISTRED(client->getNick()));
	if (!(ss >> str))
		return sendMsg(client->getSock(), ERR_NEEDMOREPARAMS(client->getNick(), "PASS"));
	if (str != _serverPassWord)
	{
		sendMsg(client->getSock(), ERR_PASSWDMISMATCH(client->getNick()));
		return quitCmd(ss, client);
	}
	client->setPassWord(str);
	client->setAuthState(PASS_AUTH);
}
