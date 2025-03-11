#include "channeluser.hpp"

ChannelUser::ChannelUser(Client *client)
    : _client(client), _operator(false), _invited(false) {}

ChannelUser::~ChannelUser() {}

Client *ChannelUser::get_client() const
{
    return _client;
}

bool ChannelUser::is_operator() const
{
    return _operator;
}

void ChannelUser::set_operator(const bool is_operator)
{
    _operator = is_operator;
}

bool ChannelUser::is_invited() const
{
    return _invited;
}

void ChannelUser::set_invited(const bool is_invited)
{
    _invited = is_invited;
}
