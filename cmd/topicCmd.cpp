#include "../server/Irc.hpp"


/**
 * @brief Handles the TOPIC command
 *
 * Syntax:
 * - Set Topic:   TOPIC <channel> <new_topic>
 *
 * @param ss Input string stream containing the command arguments.
 * @param client The client issuing the command.
 *
 * Numeric Replies:
 * - ERR_NEEDMOREPARAMS (461) - Missing parameters.
 * - ERR_NOSUCHCHANNEL (403) - The specified channel does not exist.
 * - ERR_NOTONCHANNEL (442) - User is not a member of the channel.
 * - ERR_CHANOPRIVSNEEDED (482) - User is not an operator, but the channel is mode +t (only ops can change the topic).
 */

void Irc::topicCmd(istringstream &ss, Client* client)
{
    string channelName;
    string topic;
    ss >> channelName;
    ss >> topic;

    if (channelName.empty())
        return sendMsg(client->getSock(), ERR_NEEDMOREPARAMS(client->getNick(), "TOPIC"));

    Channel *channel = findChannel(channelName);

    if (!channel)
        return sendMsg(client->getSock(), ERR_NOSUCHCHANNEL(client->getNick(), channelName));

    if (!channel->isPartOfChannel(client->getNick()))
        return sendMsg(client->getSock(), ERR_NOTONCHANNEL(client->getNick(), channelName));

    if (topic.empty())
    {
        sendMsg(client->getSock(), RPL_TOPIC(client->getNick(), channelName, channel->getChannelTopic()));
        return;
    }

    if (channel->isFlagSet('t') && !channel->isOperator(client->getNick()))
        return sendMsg(client->getSock(), ERR_CHANOPRIVSNEEDED(client->getNick(), channelName));

    channel->setChannelTopic(topic);
    channel->sendAll(RPL_TOPIC(client->getNick(), channelName, topic));
}
