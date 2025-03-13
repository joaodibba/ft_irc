
#include "../server/Irc.hpp"

static bool verifyChannelmodes(Channel *tarChannel, Client *client, istringstream &ss)
{
	string pass;
	ss >> pass;

	if (tarChannel->modes().is_invite_only() && !tarChannel->is_invited(client))
		return (sendMsg(client->getSock(), ERR_INVITEONLYCHAN(client->getNick(), tarChannel->get_channel_name())), 1);
	else if (tarChannel->modes().get_user_limit() && tarChannel->is_full())
		return (sendMsg(client->getSock(), ERR_CHANNELISFULL(client->getNick(), tarChannel->get_channel_name())), 1);
	else if (tarChannel->modes().is_password_protected() && (pass != tarChannel->modes().get_password()))
		return (sendMsg(client->getSock(), ERR_BADCHANNELKEY(client->getNick(), tarChannel->get_channel_name())), 1);

	return 0;
}


/**
 * @brief Handles the JOIN command
 *
 * Syntax:
 * - JOIN <channel>{,<channel>}
 *   (Note: This implementation currently supports only <channel>{,<channel>})
 *
 * Allows a client to join one or more channels. If the channel does not exist,
 * it is created and the client becomes the channel operator. If the client is
 * already in the channel, an error is returned. The function also checks channel
 * modes (e.g., invite-only, key-protected, full, etc.) before adding the client.
 *
 * @param ss Input string stream containing the command arguments.
 * @param client The client issuing the JOIN command.
 *
 * Numeric Replies:
 * - ERR_NEEDMOREPARAMS (461) - Missing parameters.
 * - ERR_NOSUCHCHANNEL (403) - The specified channel name is invalid.
 * - ERR_USERONCHANNEL (443) - The client is already in the channel.
 * - ERR_INVITEONLYCHAN (473) - The channel is invite-only (checked in verifyChannelmodes).
 * - ERR_CHANNELISFULL (471) - The channel is full (checked in verifyChannelmodes).
 * - ERR_BADCHANNELKEY (475) - The provided key is incorrect (checked in verifyChannelmodes).
 * - ERR_BANNEDFROMCHAN (474) - The client is banned from the channel (checked in verifyChannelmodes).
 * 
 * @see  https://www.rfc-editor.org/rfc/rfc2812.html#section-3.2.1
 * 
 */
void Irc::joinCmd(istringstream &ss, Client *client)
{
    string channelList;
    if (!(ss >> channelList))
        return sendMsg(client->getSock(), ERR_NEEDMOREPARAMS(client->getNick(), "JOIN"));

    istringstream listStream(channelList);
    string channelName;

    while (getline(listStream, channelName, ','))
    {
        if (channelName.empty() || channelName[0] != '#')
            return sendMsg(client->getSock(), ERR_NOSUCHCHANNEL(client->getNick(), channelName));

        Channel *channel = findChannel(channelName);

        if (channel && channel->is_member(client))
        {
            sendMsg(client->getSock(), ERR_USERONCHANNEL(client->getNick(), client->getNick(), channelName));
            continue;
        }

        if (!channel)
        {
            channel = createChannel(channelName);
            channel->set_operator(client, true);
        }

		//TODO: verify protected channel 475
		//TODO: invite-only 473
		//TODO: channel is full? 471
		//TODO: KICK command cases (invite revoked if kicked, ...)
        if (verifyChannelmodes(channel, client, ss))
            continue;

        if (channel->add_client(client))
        {
            cout << "Send to client fd: " << client->getSock() << endl;
            channel->send_message(RPL_JOIN(client->getNick(), client->getUser(), channelName, client->getRealName()));
        }
    }
}
