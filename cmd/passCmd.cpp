#include "../server/Irc.hpp"

// TODO Ensure this command follows the RFC https://www.rfc-editor.org/rfc/rfc2812.html#section-3.1.1
void Irc::passCmd(istringstream &ss, Client *client)
{
	string str;

	if (!(ss >> str))
		return sendMsg(client->getSock(), ERR_NEEDMOREPARAMS(client->getNick(), "PASS"));
	if (client->getAuthenticated())
		return sendMsg(client->getSock(), ERR_ALREADYREGISTRED(client->getNick()));
	if (str != _serverPassWord)
	{
		sendMsg(client->getSock(), ERR_PASSWDMISMATCH(client->getNick()));
		// client should notice that is disconnected ?
		return quitCmd(ss, client);
	}
	// this might trow exception
	client->setPassWord(str);
}
