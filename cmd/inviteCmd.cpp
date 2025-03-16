#include "../server/Irc.hpp"

// TODO Ensure this command follows the RFC https://www.rfc-editor.org/rfc/rfc2812.html#section-3.2.7
void Irc::inviteCmd(istringstream &ss, Client *client)
{
	// ss >> targetNick / channelName

	string targetNick;
	string channelName;
	Client *targetClient;
	Channel *targetChannel;

	if (ssLength(ss) < 2)
		return sendMsg(client->getSock(), ERR_NEEDMOREPARAMS(client->getNick(), "INVITE"));

	if ((ss >> targetNick) && !(targetClient = findClient(targetNick)))
		return sendMsg(client->getSock(), ERR_NOSUCHNICK(client->getNick(), targetNick));

	if ((ss >> channelName) && !(targetChannel = findChannel(channelName)))
		return sendMsg(client->getSock(), ERR_NOSUCHCHANNEL(client->getNick(), channelName));

	if (client == targetClient)
		return sendMsg(client->getSock(), ERR_USERONCHANNEL(client->getNick(), targetNick, channelName));

	if (!targetChannel->is_member(client))
		return sendMsg(client->getSock(), ERR_NOTONCHANNEL(client->getNick(), channelName));

	if (!targetChannel->is_operator(client))
		return sendMsg(client->getSock(), ERR_CHANOPRIVSNEEDED(client->getNick(), channelName));

	if (targetChannel->is_member(targetClient))
		return sendMsg(client->getSock(), ERR_USERONCHANNEL(client->getNick(), targetClient->getNick(), channelName));

	if (targetChannel->is_full())
		return sendMsg(client->getSock(), ERR_CHANNELISFULL(client->getNick(), channelName));

	targetChannel->invite(client, targetClient);
	sendMsg(client->getSock(), RPL_INVITING(client->getNick(), targetNick, channelName));
	sendMsg(targetClient->getSock(), RPL(client->getNick(), client->getNick(), "INVITE", targetNick, " ", channelName));
}
