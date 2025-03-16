#include "ChannelMode.hpp"
#include "../client/Client.hpp"
#include "Channel.hpp"


ChannelMode::ChannelMode()
    : _inviteOnly(false), _topicRestricted(false), _passwordProtected(false), _limited(false), _userLimit(0) {}

ChannelMode::~ChannelMode() {}

bool ChannelMode::is_invite_only() const
{
    return _inviteOnly;
}

void ChannelMode::set_invite_only(const bool invite_only)
{
    _inviteOnly = invite_only;
}

bool ChannelMode::is_topic_restricted() const
{
    return _topicRestricted;
}

void ChannelMode::set_topic_restricted(const bool topic_restricted)
{
    _topicRestricted = topic_restricted;
}

bool ChannelMode::is_password_protected() const
{
    return _passwordProtected;
}

void ChannelMode::set_password_protected(const bool password_protected)
{
    _passwordProtected = password_protected;
}

bool ChannelMode::is_user_limited() const
{
    return _limited;
}

size_t ChannelMode::get_user_limit() const
{
    return _userLimit;
}

void ChannelMode::set_limit(const bool flag)
{
	_limited = flag;
}

void ChannelMode::set_user_limit(const size_t user_limit)
{
    _userLimit = user_limit;
}

string ChannelMode::get_password() const
{
    return _password;
}

void ChannelMode::set_password(const string &password)
{
    _password = password;
}

bool ChannelMode::apllyLimitRestriction(istringstream& ss, bool Flag, Client* client)
{
    if (!Flag) {
        set_limit(false);
        set_user_limit(0);
        return 0;
    }
    std::string number;
    if (!(ss >> number))
        return sendMsg(client->getSock(), ERR_NEEDMOREPARAMS(client->getNick(), "MODE +l")), 1;

    char* end;
    errno = 0;
    long nb = strtol(number.c_str(), &end, 10);
    size_t tmp = static_cast<size_t>(nb);

    if (errno == ERANGE || nb <= 0 || *end != '\0')
        return sendMsg(client->getSock(), ERR_INVALIDMODEPARAM(client->getNick(), "MODE", "+l", number)), 1;
    if (tmp == get_user_limit())
        return 1;
	
    set_user_limit(tmp);
    set_limit(true);
    
    return 0;
}
