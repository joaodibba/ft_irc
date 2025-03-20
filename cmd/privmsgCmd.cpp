#include "../server/Irc.hpp"

void Irc::sendToUser(const Client *sender, const string &recipient, const string &message)
{
    Client *targetClient = findClient(recipient);

    // ERR_NOSUCHNICK (401) - No such user
    if (!targetClient)
        return sendMsg(sender->getSock(), ERR_NOSUCHNICK(sender->getNick(), recipient));

    // Send private message to user
    sendMsg(targetClient->getSock(), RPL_PRIVMSG(sender->getNick(), sender->getUser(), recipient, message));
}

void Irc::sendToChannel(Client *sender, const string &channelName, const string &message)
{
    Channel *channel = findChannel(channelName);

    // ERR_NOSUCHCHANNEL (401) - Channel does not exist
    if (!channel)
        return sendMsg(sender->getSock(), ERR_NOSUCHCHANNEL(sender->getNick(), channelName));

    // ERR_CANNOTSENDTOCHAN (404) - User is not in the channel
    if (!is_member(sender))
        return sendMsg(sender->getSock(), ERR_CANNOTSENDTOCHAN(sender->getNick(), channelName));

    // Send message to all clients in the channel (excluding sender)
    channel->send_private_message(sender, RPL_PRIVMSG(sender->getNick(), sender->getUser(), channelName, message));
}

/**
 * @brief Handles the PRIVMSG command to send a private message to a user or a channel.
 * 
 * PRIVMSG <target> :<message>
 * 
 * @param ss input string stream containing the command arguments
 * @param client the client sending the message
 * 
 * @return
 * ERR_NORECIPIENT (411) if no recipient is specified.
 * ERR_NOTEXTTOSEND (412) if no message is provided.
 * ERR_NOSUCHNICK (401) if the recipient is a user that does not exist. - Example: "JohnDoe"
 * ERR_WILDTOPLEVEL (414) if a wildcard is used improperly in the recipient. - Example: "#*"
 * ERR_NOTOPLEVEL (413) if the recipient is incorrectly formatted. - Example: ".user" is not valid.
 * ERR_TOOMANYTARGETS (407) if multiple recipients are specified but not supported.
 * ERR_CANNOTSENDTOCHAN (404) if the recipient is a channel and messages cannot be sent. - Example: moderated, restricted
 * 
 * @see https://www.rfc-editor.org/rfc/rfc2812.html#section-3.3.1
 * 
 */
void Irc::privmsgCmd(istringstream &ss, Client *client)
{
    string recipient;
    ss >> recipient;

    // ERR_NORECIPIENT (411) - No recipient specified
    if (recipient.empty())
        return sendMsg(client->getSock(), ERR_NORECIPIENT(client->getNick(), "PRIVMSG"));

    // Extract message (everything after ':')
    string message;
    getline(ss, message);

    // Remove leading ':' if present
    if (!message.empty() && message[0] == ' ')
        message.erase(0, 1);
    if (!message.empty() && message[0] == ':')
        message.erase(0, 1);

    // ERR_NOTEXTTOSEND (412) - No message provided
    if (message.empty())
        return sendMsg(client->getSock(), ERR_NOTEXTTOSEND(client->getNick()));

    // ERR_WILDTOPLEVEL (414) - Wildcards in recipient
    if (recipient.find('*') != string::npos || recipient.find('?') != string::npos)
        return sendMsg(client->getSock(), ERR_WILDTOPLEVEL(client->getNick(), recipient));

    // ERR_NOTOPLEVEL (413) - Invalid top-level recipient
    if (recipient.find('.') == 0) // Example: ".user" is not valid
        return sendMsg(client->getSock(), ERR_NOTOPLEVEL(client->getNick(), recipient));

    // Determine if it's a channel or a user
    if (recipient[0] == '#')
    {
        sendToChannel(client, recipient, message);
    }
    else
    {
        sendToUser(client, recipient, message);
    }
}
