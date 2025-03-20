#include "../server/Irc.hpp"

/**
 * @brief Handles the INVITE command for inviting a user to a channel.
 * 
 * Syntax:
 * INVITE <nickname> <channel>
 * 
 * @param ss Input string stream containing the command arguments.
 * @param client The client issuing the command.
 * 
 * Numeric Replies:
 * - ERR_NEEDMOREPARAMS (461) - Missing parameters.
 * - ERR_NOSUCHNICK (401) - The target nickname does not exist.
 * - ERR_NOSUCHCHANNEL (403) - The channel does not exist.
 * - ERR_USERONCHANNEL (443) - The user is already on the channel.
 * - ERR_NOTONCHANNEL (442) - The client is not a member of the channel.
 * - ERR_CHANOPRIVSNEEDED (482) - The user is not a channel operator.
 * - ERR_CHANNELISFULL (471) - The channel is full and cannot accept more members.
 * - RPL_INVITING (341) - Invitation sent to the target user.
 * 
 * @see https://www.rfc-editor.org/rfc/rfc2812.html#section-3.2.7
 */

void Irc::inviteCmd(istringstream &ss, Client *client)
{
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
