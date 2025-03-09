#include "../server/Irc.hpp"

void Irc::nickCmd(istringstream &ss, Client* client) {
	string str;
	ss >> str;

	if (!client->isAuthenticated() && client->getPassWord().empty())
		return sendMsg(client->getSock(), NOTICE_MSG("Empty pass, please set a pass first"));
	if (findClient(str))
		sendMsg(client->getSock(), ERR_NICKNAMEINUSE(str));
	else {
		if (str.empty())
			return sendMsg(client->getSock(), ERR_NONICKNAMEGIVEN("*"));
		if (str.find_first_of(" ,*?!@.#&:$") != string::npos)
			return sendMsg(client->getSock(), ERR_ERRONEUSNICKNAME("*", str));
	
		string oldNick = client->getNick();
		client->setNick(str);
		sendMsg(client->getSock(), RPL(oldNick, client->getUser(), "NICK", "", ":", client->getNick()));
	}
}
