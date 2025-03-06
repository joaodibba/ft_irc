
#include "../Irc.hpp"

static bool verifyChannelmodes(Channel* tarChannel, Client* client, istringstream& ss)
{
	string pass;
	ss >> pass;

	if (tarChannel->isFlagSet('i') && !tarChannel->isUserInvited(client->getNick()))
		return (sendMsg(client->getSock(), ERR_INVITEONLYCHAN(client->getNick(), tarChannel->getChannelName())), 1);
	else if (tarChannel->isFlagSet('l') && tarChannel->isChannelFull())
		return (sendMsg(client->getSock(), ERR_CHANNELISFULL(client->getNick(), tarChannel->getChannelName())), 1);
	else if (tarChannel->isFlagSet('k') && (pass != tarChannel->getChannelPassword()))
		return (sendMsg(client->getSock(), ERR_BADCHANNELKEY(client->getNick(), tarChannel->getChannelName())), 1);

	return 0;
}

void Irc::joinCmd(istringstream &ss, Client* client)
{
	string msg;
	string channelName;
	if (!(ss >> channelName))
		return sendMsg(client->getSock(), ERR_NEEDMOREPARAMS(client->getNick(), "JOIN"));

	if (channelName.empty() || channelName[0] != '#')
		return sendMsg(client->getSock(), ERR_NOSUCHCHANNEL(client->getNick(), channelName));

	Channel* tarChannel;
	if ((tarChannel = findChannel(channelName)))
	{
		if (tarChannel->isPartOfChannel(client->getNick()))
			return;
		if (!verifyChannelmodes(tarChannel, client, ss))
		{
			tarChannel->setChannelUsers(false, client);	
			cout << "Send to client fd: " << client->getSock() << endl;
			tarChannel->sendAll(RPL_JOIN(client->getNick(), client->getUser(), channelName, string("realname"))); //!FIXME realname is not implemented?
		}
		return;
	}
	tarChannel = createChannel(channelName);
	tarChannel->setChannelUsers(true, client);

	cout << "Send to client fd: " << client->getSock() << endl;	
	tarChannel->sendAll(RPL_JOIN(client->getNick(), client->getUser(), channelName, string("realname"))); //!FIXME realname is not implemented?
}