#include "Channel.hpp"
#include "ChannelUser.hpp"

Channel::Channel(const string &name) : _channelName(name) {}

Channel::~Channel()
{
    map<int, ChannelUser *>::iterator it = _users.begin();
    for (; it != _users.end(); it++)
    {
        delete it->second;
    }
    _users.clear();
}

void Channel::set_channel_topic(const string &channel_topic)
{
    _channelTopic = channel_topic;
}

string Channel::get_channel_topic() const
{
    return _channelTopic;
}

void Channel::set_channel_name(const string &channel_name)
{
    _channelName = channel_name;
}

string Channel::get_channel_name() const
{
    return _channelName;
}

ChannelMode &Channel::modes()
{
    return _modes;
}

const ChannelMode &Channel::modes() const
{
    return _modes;
}

const std::map<int, ChannelUser *> &Channel::users() const
{
    return _users;
}

bool Channel::add_client(Client *client)
{
    if (is_full())
        return false;

    const int client_sock = client->getSock();

    if (_users.find(client_sock) != _users.end())
    {
        cerr << "Client already in channel.\n";
        sendMsg(client->getSock(), "Already in this channel\n\r");
        return false;
    }

    ChannelUser *user = new ChannelUser(client);
    _users[client_sock] = user;
    return true;
}

bool Channel::remove_client(const Client *client)
{
    const int client_sock = client->getSock();
    if (_users.find(client_sock) != _users.end())
    {
        delete _users[client_sock];
        _users.erase(client_sock);
        return true;
    }
    return false;
}

bool Channel::is_member(const Client *client) const
{
    if (!client)
        return false;
    const int client_sock = client->getSock();
    const std::map<int, ChannelUser *>::const_iterator it = _users.find(client_sock);
    return (it != _users.end());
}

bool Channel::is_operator(const Client *client) const
{
    const int client_sock = client->getSock();
    const std::map<int, ChannelUser *>::const_iterator it = _users.find(client_sock);
    if (it != _users.end())
    {
        return it->second->is_operator();
    }
    return false;
}

void Channel::set_operator(const Client *client, const bool is_operator)
{
    int client_sock = client->getSock();
    std::map<int, ChannelUser *>::iterator it = _users.find(client_sock);

    if (it == _users.end())
    {
        std::cerr << "Client not found in channel when setting operator.\n";
        return;
    }

    std::cout << "Client Name: " << it->second->get_client()->getUser() << std::endl;
    it->second->set_operator(is_operator);
}

bool Channel::is_invited(const Client *client) const
{
    for (vector<ChannelInvite>::const_iterator it = _invites.begin(); it != _invites.end(); ++it)
    {
        if (it->is_receiver(client) && it->is_valid())
            return true;
    }
    return false;
}

void Channel::invite(const Client *sender, const Client *receiver)
{
    _invites.push_back(ChannelInvite(sender, receiver));
}

void Channel::revoke_invites()
{
    for (std::vector<ChannelInvite>::iterator it = _invites.begin(); it != _invites.end(); ++it)
    {
        ChannelInvite &invite = *it;
        const Client *sender = invite.get_sender();
        const Client *receiver = invite.get_receiver();

        if (!sender || !receiver)
        {
            invite.invalidate();
            continue;
        }

        if (is_full() || invite.is_expired() ||
            !is_member(sender) || is_member(receiver))
		{
            invite.invalidate();
			//sendMsg(sender->getSock(), "Invite revoked\n\r");
			//sendMsg(receiver->getSock(), "Invite revoked\n\r");
		}
    }
}

bool Channel::is_full() const
{
    if (_modes.get_user_limit() == 0)
        return false;
    return _users.size() >= _modes.get_user_limit();
}

size_t Channel::size() const
{
    return _users.size();
}

void Channel::send_private_message(Client *sender, const string &message)
{
    std::map<int, ChannelUser *>::iterator it = _users.begin();
    for (; it != _users.end(); ++it)
    {
        const Client *receiver = it->second->get_client();
        const int sender_sock = sender->getSock();
        if (receiver && sender_sock && receiver->getSock() != sender_sock)
            send(receiver->getSock(), message.c_str(), message.size(), 0);
    }
}

void Channel::send_message(const string &message)
{

    std::map<int, ChannelUser *>::iterator it = _users.begin();
    cout << WHITE << message << END << endl;
    for (; it != _users.end(); ++it)
    {
        const Client *client = it->second->get_client();
        const int client_sock = client->getSock();
        if (client && client_sock)
            send(client_sock, message.c_str(), message.size(), 0);
    }
}

map<int, ChannelUser *> Channel::getUsers()
{
    return this->_users;
}

void Channel::leave_channel(Client *client)
{
    if (client && is_member(client))
        remove_client(client);

    std::map<int, ChannelUser *>::iterator it = _users.begin();
    for (; it != _users.end(); it++)
    {
        if ((*it).second->is_operator())
            return;
    }

    if (!_users.empty() && _users.begin()->second)
        _users.begin()->second->set_operator(true);
}

const vector<ChannelInvite> & Channel::get_invites() const {
    return _invites;
}

bool Channel::canSendMessage(Client *sender){

    if (!(is_member(sender)))
        return false;

    return true;
}

size_t Channel::countOperators(){

    size_t count = 0;

    for(std::map<int, ChannelUser *>::iterator it = _users.begin(); it != _users.end(); ++it)
        if ((*it).second->is_operator()) count++;

    return count;
}
