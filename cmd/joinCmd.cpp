
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

// TODO Ensure this command follows the RFC https://www.rfc-editor.org/rfc/rfc2812.html#section-3.2.1
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

        if (verifyChannelmodes(channel, client, ss))
            continue;

        if (channel->add_client(client))
        {
            cout << "Send to client fd: " << client->getSock() << endl;
            channel->send_message(RPL_JOIN(client->getNick(), client->getUser(), channelName, client->getRealName()));
        }
    }
}
