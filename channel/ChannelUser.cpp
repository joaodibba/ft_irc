#include "ChannelUser.hpp"

ChannelUser::ChannelUser(Client *client)
    : _client(client), _operator(false) {}

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
