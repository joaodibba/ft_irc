#include "ChannelMode.hpp"

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

size_t ChannelMode::get_user_limit() const
{
    return _userLimit;
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
