#include "../server/Irc.hpp"
#include "../channel/ChannelUser.hpp"

class ChannelUser;

void Irc::infoChannel(istringstream &ss, Client *client)
{
	std::string channelName;
    if (!(ss >> channelName))
        return sendMsg(client->getSock(), ERR_NEEDMOREPARAMS(client->getNick(), "INFO"));

    Channel *channel = findChannel(channelName);
    if (!channel)
        return sendMsg(client->getSock(), ERR_NOSUCHCHANNEL(client->getNick(), channelName));

    std::ostringstream msg;

    msg << "---------------- CHANNEL INFO --------------------\r\n";
    msg << "Channel Name: " << channel->get_channel_name() << "\r\n";

    // // Topic (if you have one)
    // const std::string &topic = channel->get_topic(); // assuming you have this
    // if (!topic.empty())
    //     msg << "Topic: " << topic << "\r\n";
    // else
    //     msg << "Topic: <No topic set>\r\n";

    // List users
    std::map<int, ChannelUser *> users = channel->getUsers();
    msg << "Users (" << users.size() << "):\r\n";
    for (std::map<int, ChannelUser *>::iterator it = users.begin(); it != users.end(); ++it)
    {
        Client *userClient = it->second->get_client();
        std::string prefix = it->second->is_operator() ? "@" : "-";
        msg << prefix << userClient->getNick() << "\r\n";
    }

    msg << "--------------------------------------------------\r\n";

    sendMsg(client->getSock(), msg.str());
}