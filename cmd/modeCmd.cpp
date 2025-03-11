#include "../server/Irc.hpp"

/**
 * @brief Handles the MODE command for setting channel modes.
 * 
 * Syntax:
 * MODE <channel> *( ( "-" / "+" ) *<modes> *<modeparams> )  
 * 
 * @param ss Input string stream containing the command arguments.
 * @param client The client issuing the command.
 * 
 * Numeric Replies:
 * - ERR_NEEDMOREPARAMS (461) - Missing parameters.
 * - ERR_NOCHANMODES (477) - The channel does not support modes.
 * - ERR_CHANOPRIVSNEEDED (482) - User is not a channel operator.
 * - ERR_UNKNOWNMODE (472) - Invalid mode given.
 * - ERR_NOSUCHCHANNEL (403) - Channel does not exist.
 * - RPL_CHANNELMODEIS (324) - Displays the current channel modes.
 */

// TODO Ensure this command follows the RFC https://www.rfc-editor.org/rfc/rfc2812.html#section-3.2.3
void Irc::modeCmd(istringstream &ss, Client *client)
{
    string channelName, modes, param;
    ss >> channelName >> modes;

    // ERR_NEEDMOREPARAMS (461) - Not enough parameters
    if (channelName.empty())
        return sendMsg(client->getSock(), ERR_NEEDMOREPARAMS(client->getNick(), "MODE"));

    // Handle channel modes
    Channel *channel = findChannel(channelName); // Use 'channelName' for the channel name

    // ERR_NOSUCHCHANNEL (403) - Channel does not exist
    if (!channel)
        return sendMsg(client->getSock(), ERR_NOSUCHCHANNEL(client->getNick(), channelName));

    // if (!channel->is_member(client)) // FIXME check if user is in channel

    // ERR_CHANOPRIVSNEEDED (482) - User is not an operator
    if (!channel->is_operator(client)) // Use 'client' instead of 'sender'
        return sendMsg(client->getSock(), ERR_CHANOPRIVSNEEDED(client->getNick(), channelName));

    bool adding = true; // True for +, false for -

    for (size_t i = 0; i < modes.size(); i++)
    {

        char mode = modes[i];
        switch (mode)
        {
        case '+':
            adding = true;
            break;
        case '-':
            adding = false;
            break;
        case 'o': // Operator privilege
            channel->set_operator(client, adding);
            return channel->send_message(RPL_CHANNELMODEIS(client->getNick(), channelName, modes));
        case 'i': // Invite-only channel
            channel->modes().set_invite_only(adding);
            return channel->send_message(RPL_CHANNELMODEIS(client->getNick(), channelName, modes));
        case 't': // Topic restrictions
            channel->modes().set_topic_restricted(adding);
            return channel->send_message(RPL_CHANNELMODEIS(client->getNick(), channelName, modes));
        case 'k': // Channel key (password)
            channel->modes().set_password_protected(adding);
            return channel->send_message(RPL_CHANNELMODEIS(client->getNick(), channelName, modes));
        case 'l': // User limit
            channel->modes().set_user_limit(adding);
            return channel->send_message(RPL_CHANNELMODEIS(client->getNick(), channelName, modes));
        default:
            return sendMsg(client->getSock(), ERR_UNKNOWNMODE(client->getNick(), string(1, mode)));
        }
    }
}

/**
 * ! FIXME
 * Channel modes:
 * • 'i' (invite-only): Users must be explicitly invited to join.
 * • 't' (topic lock): Only operators can change the channel topic.
 * • 'k' (keyed/password): A user must provide the correct password to join.
 * • 'm' (moderated): Only operators and voiced users can send messages.
 * • 'n' (no external messages): Users from outside the channel cannot send messages.
 * • 's' (secret) or 'p' (private): Hides the channel from whois queries and channel lists in varying degrees, depending on the IRC server.
 * • 'l' (limit): Defines a maximum number of users allowed in the channel.
 */