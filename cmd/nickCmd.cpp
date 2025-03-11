#include "../server/Irc.hpp"

// TODO Ensure this command follows the RFC https://www.rfc-editor.org/rfc/rfc2812.html#section-3.1.2
void Irc::nickCmd(istringstream &ss, Client *client)
{
	string str;
	ss >> str;

	if (client->getAuthenticated())
		return;
	if (!client->getAuthenticated() && client->getPassWord().empty())
		return sendMsg(client->getSock(), NOTICE_MSG("Empty pass, please set a pass first"));
	if (findClient(str))
		sendMsg(client->getSock(), ERR_NICKNAMEINUSE(str));
	else
	{
		if (str.empty())
			return sendMsg(client->getSock(), ERR_NONICKNAMEGIVEN("*"));
		if (str.find_first_of(" ,*?!@.#&:$") != string::npos)
			return sendMsg(client->getSock(), ERR_ERRONEUSNICKNAME("*", str));

		const string oldNick = client->getNick();
		client->setNick(str);
		sendMsg(client->getSock(), RPL(oldNick, client->getUser(), "NICK", "", ":", client->getNick()));
	}
}
