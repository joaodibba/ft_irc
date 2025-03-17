#include "../server/Irc.hpp"

/**
 * @brief Handles the QUIT command, allowing a client to disconnect from the server.
 * 
 * QUIT [:<message>]
 * 
 * @details
 * This command removes the client from all channels they are part of, notifies other members 
 * of their departure, and deletes the client from the server. If a message is provided, it 
 * will be broadcasted to the remaining members.
 * 
 * @param ss Input string stream containing an optional quit message.
 * @param client Pointer to the client issuing the QUIT command.
 * 
 * @see https://www.rfc-editor.org/rfc/rfc2812.html#section-3.1.7
 */
void Irc::quitCmd(istringstream &ss, Client *client)
{
	if (!client)
		return;

	string message = "Left the server"; // Default message
	string user = client->getUser();
	string nick = client->getNick();

	if (!ss.eof())
	{
		getline(ss, message);
		if (!message.empty() && message[0] == ':')
			message.erase(0, 1);
	}

	// Remove the client from all channels
	for (std::vector<Channel *>::iterator it = _serverChannels.begin(); it != _serverChannels.end();)
	{
		(*it)->send_private_message(client, RPL_QUIT(user, nick, message));
		(*it)->leave_channel(client);

		if ((*it)->size() == 0)
		{
			delete *it;
			it = _serverChannels.erase(it);
		}
		else
			++it;
	}

	// Remove client from server
	const int sock = client->getSock();
	map<int, Client *>::iterator it = _clients.find(sock);

	if (it != _clients.end())
	{
		if (it->second)
			delete it->second;
		_clients.erase(it);
	}

	epfds->deleteFd(sock);
}
