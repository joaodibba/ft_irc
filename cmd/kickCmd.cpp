#include "../server/Irc.hpp"

void Irc::kickCmd(istringstream &ss, Client *client)
{

	string channelName;
	string nickName;
	string reason;

	if (!(ss >> channelName) || !(ss >> nickName) || !(ss >> reason))
		return sendMsg(client->getSock(), ERR_NEEDMOREPARAMS(client->getNick(), "KICK"));

	Channel *channel = findChannel(channelName);
	if (!channel)
		return sendMsg(client->getSock(), ERR_NOSUCHCHANNEL(client->getNick(), channelName));

	if (!channel->is_member(client))
		return sendMsg(client->getSock(), ERR_NOTONCHANNEL(client->getNick(), channelName));

	if (!channel->is_operator(client))
		return sendMsg(client->getSock(), ERR_CHANOPRIVSNEEDED(client->getNick(), channelName));

	Client *targetClient = findClient(nickName);
	if (!targetClient)
		return sendMsg(client->getSock(), ERR_NOSUCHNICK(client->getNick(), nickName));

	if (!channel->is_member(targetClient))
		return sendMsg(client->getSock(), ERR_USERNOTINCHANNEL(client->getNick(), targetClient->getNick(), channelName));

	string tmp;
	getline(ss, tmp);
	reason += tmp;
	cout << "Send to client fd: " << client->getUser() << endl;
	channel->send_message(RPL(client->getNick(), client->getUser(), "KICK", (channelName + " " + targetClient->getNick()), " ", reason));
	channel->remove_client(targetClient);
	if (channel->size() == 0)
	{
		_serverChannels.erase(find(_serverChannels.begin(), _serverChannels.end(), channel));
		delete channel;
	}
}