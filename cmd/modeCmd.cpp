#include "../server/Irc.hpp"
#include <string>

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
 * 
 * @see https://www.rfc-editor.org/rfc/rfc2812.html#section-3.2.3
 * 
 */

void Irc::modeCmd(istringstream &ss, Client *client)
{
	if (ssLength(ss) < 2)
		return sendMsg(client->getSock(), ERR_NEEDMOREPARAMS(client->getNick(), "MODE"));
    string channelName, modes, param, flags;
<<<<<<< HEAD
    ss >> channelName >> modes; //FIXME: MODE #channel -> trows no error
=======
    ss >> channelName >> modes;	
>>>>>>> main

    // ERR_NEEDMOREPARAMS (461) - Not enough parameters
    if (channelName.empty())
        return sendMsg(client->getSock(), ERR_NEEDMOREPARAMS(client->getNick(), "MODE"));

    // Handle channel modes
    Channel *channel = findChannel(channelName); // Use 'channelName' for the channel name

    // ERR_NOSUCHCHANNEL (403) - Channel does not exist
    if (!channel)
        return sendMsg(client->getSock(), ERR_NOSUCHCHANNEL(client->getNick(), channelName));

    if (!channel->is_member(client))
		return sendMsg(client->getSock(), ERR_NOTONCHANNEL(client->getNick(), channelName));

    // ERR_CHANOPRIVSNEEDED (482) - User is not an operator
    if (!channel->is_operator(client)){ // Use 'client' instead of 'sender'
        return sendMsg(client->getSock(), ERR_CHANOPRIVSNEEDED(client->getNick(), channelName));
	}
	
	Client* targetClient;
    bool adding = true; // True for +, false for -
	flags = "okl";
	for (size_t i = 0; i < modes.size(); i++)
    {
		if (modes.size() < 2 && flags.find(modes))
			return sendMsg(client->getSock(), ERR_NEEDMOREPARAMS(client->getNick(), "MODE"));
	    char mode = modes[i];
        switch (mode)
        {
			case '+':
				adding = true;
				break;
			case '-':
				adding = false;
				break;
			case 'i': // Invite-only channel
				channel->modes().set_invite_only(adding);
				return channel->send_message(RPL_CHANNELMODEIS(client->getNick(), channelName, modes));
			case 't': // Topic restrictions
				channel->modes().set_topic_restricted(adding);
				return channel->send_message(RPL_CHANNELMODEIS(client->getNick(), channelName, modes));
			case 'k': // Channel key (password)
				if (!(ss >> param))
					return sendMsg(client->getSock(), ERR_NEEDMOREPARAMS(client->getNick(), "MODE -k or +k"));
				if (adding) 
				{
					if (!channel->modes().is_password_protected() || channel->modes().get_password().empty()) {
						channel->modes().set_password(param);
						channel->modes().set_password_protected(true);
					} else
						return sendMsg(client->getSock(), ERR_INVALIDMODEPARAM(client->getNick(), channelName, "k", "Password already set"));
				} else {
					if (channel->modes().is_password_protected() && channel->modes().get_password() == param) {
						channel->modes().set_password("");
						channel->modes().set_password_protected(false);
				
					} else if (!channel->modes().is_password_protected())
						return sendMsg(client->getSock(), ERR_INVALIDMODEPARAM(client->getNick(), channelName, "k", "Password Is not set"));
					else
						return sendMsg(client->getSock(), ERR_INVALIDMODEPARAM(client->getNick(), channelName, "k", "Incorrect password"));
				}
				return channel->send_message(RPL_CHANNELMODEIS(client->getNick(), channelName, modes));
			case 'o': {// Operator privilege
				if (!(ss >> param))
					return sendMsg(client->getSock(), ERR_NEEDMOREPARAMS(client->getNick(), "MODE"));
				if (!(targetClient=findClient(param)))
					return sendMsg(client->getSock(), ERR_NOSUCHNICK(client->getNick(), param));
				if (!channel->is_member(targetClient))
					return sendMsg(client->getSock(), ERR_NOTONCHANNEL(client->getNick(), channelName));
				if (!adding && channel->countOperators() == 1 && channel->is_operator(targetClient))
					return sendMsg(client->getSock(), ERR_CANNOTREMOVEOP(client->getNick(), channelName, "Cannot remove the last operator from the channel"));
				channel->set_operator(targetClient, adding);
				return channel->send_message(RPL_CHANNELMODEIS(client->getNick(), channelName, modes));
			}
			case 'l': // User limit
				if (!channel->modes().apllyLimitRestriction(ss, adding, client, channelName))
					return channel->send_message(RPL_CHANNELMODEIS(client->getNick(), channelName, modes));
				break;
			default:
				return sendMsg(client->getSock(), ERR_UNKNOWNMODE(client->getNick(), string(1, mode)));
        }
    }
}
