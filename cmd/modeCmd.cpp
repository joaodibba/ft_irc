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

void Irc::modeCmd(istringstream &ss, Client* client)
{
    string channelName, modes, param;
    ss >> channelName >> modes;

    // ERR_NEEDMOREPARAMS (461) - Not enough parameters
    if (channelName.empty())
        return sendMsg(client->getSock(), ERR_NEEDMOREPARAMS(client->getNick(), "MODE"));

    // Handle channel modes
    Channel* channel = findChannel(channelName); // Use 'channelName' for the channel name

    // ERR_NOSUCHCHANNEL (403) - Channel does not exist
    if (!channel)
        return sendMsg(client->getSock(), ERR_NOSUCHCHANNEL(client->getNick(), channelName));

    // ERR_CHANOPRIVSNEEDED (482) - User is not an operator
    if (!channel->isOperator(client->getNick()))  // Use 'client' instead of 'sender'
        return sendMsg(client->getSock(), ERR_CHANOPRIVSNEEDED(client->getNick(), channelName));

    bool adding = true;  // True for +, false for -

    for (size_t i = 0; i < modes.size(); i++) {
       
       char mode = modes[i];
        switch (mode) {
            case '+': 
                adding = true; 
                break;
            case '-': 
                adding = false; 
                break;
            case 'o':  // Operator privilege
                applyOperatorPrivilegeFlag(ss, modes, client, channel); // ! FIXME send only mode to applyOperatorPrivilegeFlag
                return channel->sendAll(RPL_CHANNELMODEIS(client->getNick(), channelName, modes));
            case 'i':  // Invite-only channel
                applyInviteOnlyFlag(adding, channel);
                return channel->sendAll(RPL_CHANNELMODEIS(client->getNick(), channelName, modes));
            case 't':  // Topic restrictions
               applyTopicRestrictionFlag(adding, channel);
               return channel->sendAll(RPL_CHANNELMODEIS(client->getNick(), channelName, modes));
            case 'k':  // Channel key (password)
                applyPasswordFlag(ss, modes, client, channel); // ! FIXME send only mode to applyPasswordFlag
                return channel->sendAll(RPL_CHANNELMODEIS(client->getNick(), channelName, modes));
            case 'l':  // User limit
                applyLimitRestrictionFlag(ss, modes, client, channel); // ! FIXME send only mode to applyLimitRestrictionFlag
                return channel->sendAll(RPL_CHANNELMODEIS(client->getNick(), channelName, modes));
            default:
                return sendMsg(client->getSock(), ERR_UNKNOWNMODE(client->getNick(), string(1, mode)));
        }
    }
}
 