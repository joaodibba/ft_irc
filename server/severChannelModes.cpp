#include "Irc.hpp"

void Irc::applyInviteOnlyFlag(bool optr, Channel* targetChannel){
	if (optr)
		targetChannel->setChannelModes('i');
	else
		targetChannel->removeChannelModesFlag('i');
}

void Irc::applyTopicRestrictionFlag(bool optr, Channel *targetChannel){
	if (optr)
		targetChannel->setChannelModes('t');
	else
		targetChannel->removeChannelModesFlag('t');
}

bool Irc::applyPasswordFlag(istringstream &ss, string &modeFlag, Client *client, Channel *targetChannel) {
	string pass;
	bool operation = (modeFlag[0] == '-');
	if (!(ss >> pass))
		return (sendMsg(client->getSock(), ERR_NEEDMOREPARAMS(client->getNick(), string("MODE +k"))), 1);

	if (operation)
	{
		if (!targetChannel->isFlagSet('k'))
			return true;

		if (pass != targetChannel->getChannelPassword())
			return (sendMsg(client->getSock(), ERR_KEYSET(client->getNick())), 1);
		targetChannel->removeChannelModesFlag('k');
		targetChannel->setChannelPassword("");
	}
	else {
		if (!targetChannel->getChannelPassword().empty())
			return (sendMsg(client->getSock(), ERR_KEYSET(client->getNick())), 1);
		targetChannel->setChannelPassword(pass);
		targetChannel->setChannelModes('k');
	}
	cout << "flag conf: " << targetChannel->getChannelModes() << endl;;
	modeFlag += " " + pass;
	return false;
}

bool Irc::applyLimitRestrictionFlag(istringstream &ss, string &modeFlag, Client *client, Channel *targetChannel){
	if (modeFlag[0] == '-')
		return ((targetChannel->isFlagSet('l')) ? (targetChannel->removeChannelModesFlag('l'), 0) : 1);
	int nb;
	char *end;
	string number;
	stringstream converter;

	if (!(ss >> number))
		return (sendMsg(client->getSock(), ERR_NEEDMOREPARAMS(client->getNick(), string("mode +l"))), 1);
	errno = 0;
	nb = strtol(number.c_str(), &end, 10);
	size_t tmp = nb;
	if (nb <= 0 || errno == ERANGE || tmp == targetChannel->getMaxUsersNumber())
		return true;
	
	targetChannel->setMaxUsersNumber(nb);
	if (!targetChannel->isFlagSet('l'))
		targetChannel->setChannelModes('l');
	converter << nb;
	modeFlag += " " + converter.str();

	return false;
}

bool Irc::applyOperatorPrivilegeFlag(istringstream &ss, string &modeFlag, Client *client, Channel *targetChannel){
	string targetNick;
	bool operation = (modeFlag[0] == '-');
	cout << "operator: " << operation << endl;

	if (!(ss >> targetNick))
		return true;
	if (!findClient(targetNick))
		return (sendMsg(client->getSock(), ERR_NOSUCHNICK(client->getNick(), targetNick)), 1);
	if (!targetChannel->isPartOfChannel(targetNick))
		return (sendMsg(client->getSock(), ERR_USERNOTINCHANNEL(client->getNick(), targetNick, targetChannel->getChannelName())),1);

	if (operation) {
		if (!targetChannel->isOperator(targetNick))
			return true;
		else
			targetChannel->giveOrTakeOperatorPrivilege(targetNick);
	}
	else {
		if (targetChannel->isOperator(targetNick))
			return true;
		else
			targetChannel->giveOrTakeOperatorPrivilege(targetNick, true);
	}
	modeFlag += targetNick;
	cout << "modeFlag: " << modeFlag <<endl;
	return false;
}




