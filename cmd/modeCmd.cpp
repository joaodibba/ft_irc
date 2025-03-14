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

    if (!channel->is_member(client)) // FIXME check if user is in channel
		return sendMsg(client->getSock(), ERR_NOTONCHANNEL(client->getNick(), channelName));

    // ERR_CHANOPRIVSNEEDED (482) - User is not an operator
    if (!channel->is_operator(client)){ // Use 'client' instead of 'sender'
        return sendMsg(client->getSock(), ERR_CHANOPRIVSNEEDED(client->getNick(), channelName));
	}

	std::cout << "MODE " << channelName << " " << modes << std::endl;
	Client* targetClient;
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
			case 'i': // Invite-only channel
				channel->modes().set_invite_only(adding);
				return channel->send_message(RPL_CHANNELMODEIS(client->getNick(), channelName, modes));
			case 't': // Topic restrictions
				channel->modes().set_topic_restricted(adding);
				return channel->send_message(RPL_CHANNELMODEIS(client->getNick(), channelName, modes));
			case 'k': // Channel key (password)
				if (!(ss >> param))
					return sendMsg(client->getSock(), ERR_NEEDMOREPARAMS(client->getNick(), "MODE"));
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
					} else
						return sendMsg(client->getSock(), ERR_INVALIDMODEPARAM(client->getNick(), channelName, "k", "Incorrect password"));
				}
				return channel->send_message(RPL_CHANNELMODEIS(client->getNick(), channelName, modes));
			case 'o': {// Operator privilege
				if (!(ss >> param))
					return sendMsg(client->getSock(), ERR_NEEDMOREPARAMS(client->getNick(), "MODE"));
				std::cout << "TargetClient: " << param << std::endl;
	
				if (!(targetClient=findClient(param)))
					return sendMsg(client->getSock(), ERR_NOSUCHNICK(client->getNick(), param)); // Usuário não encontrado
		
				std::cout << "TargetClient Nick: " << targetClient->getNick() << std::endl;

				// Verifica se o target está no canal
				if (!channel->is_member(targetClient))
					return sendMsg(client->getSock(), ERR_NOTONCHANNEL(client->getNick(), channelName)); // 442: Usuário não está no canal

				// Define ou remove o operador corretamente
				channel->set_operator(targetClient, adding);
				
				return channel->send_message(RPL_CHANNELMODEIS(client->getNick(), channelName, modes));
			}
			case 'l': // User limit
				if (!channel->modes().apllyLimitRestriction(ss, adding, client))
					return channel->send_message(RPL_CHANNELMODEIS(client->getNick(), channelName, modes));
				break;
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
 * • 'l' (limit): Defines a maximum number of users allowed in the channel.
 *  .'o' (give permission to users)
 
 * • 'm' (moderated): Only operators and voiced users can send messages.
 * • 'n' (no external messages): Users from outside the channel cannot send messages.
 * • 's' (secret) or 'p' (private): Hides the channel from whois queries and channel lists in varying degrees, depending on the IRC server.
 */