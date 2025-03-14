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
	//const map<int, Client *>::iterator it = _clients.find(client->getSock());
	//cout << YELLOW << "Closing connection, fd: " << it->first << END << endl;
	//leaveAllChannels(client);
	/*
		Go trhough every channel and see if client is member
		If client is member:
			See if its the last member of the Channel, if it is:
				delete the channel
			See if there is another operator, if there is
				dp nothing
			else
				set operator on the last member on the map _users
	*/
	std::vector<Channel *>::iterator it = _serverChannels.begin();
	for(; it != _serverChannels.end(); ++it){
		(*it)->leave_channel(client);
		if ((*it)->size() == 0){
			delete *it;
		}

	}
	deleteClient(client);
	_clients.erase(client->getSock());
	// ! FIXME deleteClient(it); ???
	// delete it->second;
	// epfds->deleteFd(it->first);
	// _clients.erase(it->first);
}
