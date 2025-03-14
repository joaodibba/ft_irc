#include "../server/Irc.hpp"

void Irc::leaveAllChannels(Client *client)
{
	for (vector<Channel *>::iterator it = _serverChannels.begin(); it != _serverChannels.end(); it++)
	{
		if (((*it)->is_member(client)))
		{
			(*it)->send_private_message(client, RPL_PART(client->getNick(), client->getUser(), (*it)->get_channel_name(), "Leaving"));
			(*it)->remove_client(client);
			if ((*it)->size() == 0)
			{
				//TODO: if client last on the channel, give operator role to another user?
				delete *it;
				it = _serverChannels.erase(it);
				it--;
			}
		}
	}
}

// TODO Ensure this command follows the RFC https://www.rfc-editor.org/rfc/rfc2812.html#section-3.1.7
void Irc::quitCmd(istringstream &ss, Client *client)
{
	(void)ss;
	std::vector<Channel *> toDelete;

	for (std::vector<Channel *>::iterator it = _serverChannels.begin(); it != _serverChannels.end(); ++it) {
		(*it)->leave_channel(client);
		if ((*it)->size() == 0)
			toDelete.push_back(*it);
	}

	for (size_t i = 0; i < toDelete.size(); ++i) {
		std::vector<Channel *>::iterator it = std::find(_serverChannels.begin(), _serverChannels.end(), toDelete[i]);
		if (it != _serverChannels.end()) {
			delete *it;
			_serverChannels.erase(it);
		}
	}

	std::cout << "Client " << client->getNick() << " has quit" << std::endl;
	std::cout << "Client socket: " << client->getSock() << std::endl;
	epfds->deleteFd(client->getSock());

	_clients.erase(client->getSock());
}
