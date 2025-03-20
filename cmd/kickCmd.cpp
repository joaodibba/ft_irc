#include "../server/Irc.hpp"

/**
 * @brief Handles the KICK command for removing a user from a channel.
 *
 * Syntax:
 * - KICK <channel> <nick> [<message>]
 *   (Note: This implementation currently does not support passing multiple channels and nicks in a single command)
 * 
 * @param ss Input string stream containing the command arguments.
 * @param client The client issuing the JOIN command.
 *
 * Numeric Replies:
 * - ERR_NEEDMOREPARAMS (461) - Not enough parameters.
 * - ERR_NOSUCHCHANNEL (403) - The channel does not exist.
 * - ERR_CHANOPRIVSNEEDED (482) - The client does not have operator privileges on the channel.
 * - ERR_USERNOTINCHANNEL (441) - The target is not on that channel.
 * - ERR_NOTONCHANNEL (442) - The client is not on that channel.
 * - ERR_NOSUCHNICK (401) - The target does not exist.
 * 
 * @see  https://www.rfc-editor.org/rfc/rfc2812.html#section-3.2.8
 * 
 */
void Irc::kickCmd(istringstream &ss, Client *client)
{
	string channelName;
	string nickName;
	string reason;

	if (!(ss >> channelName) || !(ss >> nickName) || !(ss >> reason))
		return sendMsg(client->getSock(), ERR_NEEDMOREPARAMS(client->getNick(), "KICK"));

	Channel *channel = findChannel(channelName);
	if (!channel)
		return sendMsg(client->getSock(), ERR_NOSUCHCHANNEL(client->getNick(), channelName));

	if (!channel->is_member(client))
		return sendMsg(client->getSock(), ERR_NOTONCHANNEL(client->getNick(), channelName));

	if (!channel->is_operator(client))
		return sendMsg(client->getSock(), ERR_CHANOPRIVSNEEDED(client->getNick(), channelName));

	Client *targetClient = findClient(nickName);
	if (!targetClient)
		return sendMsg(client->getSock(), ERR_NOSUCHNICK(client->getNick(), nickName));

	if (!channel->is_member(targetClient))
		return sendMsg(client->getSock(), ERR_USERNOTINCHANNEL(client->getNick(), targetClient->getNick(), channelName));

	// get the rest of the message
	string tmp;
	getline(ss, tmp);
	reason += tmp;

	channel->send_message(RPL(client->getNick(), client->getUser(), "KICK", (channelName + " " + targetClient->getNick()), " ", reason));
	channel->leave_channel(targetClient);
    if (channel->size() == 0)
        deleteChannel(channelName);
}