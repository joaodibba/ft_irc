#include "ChannelInvite.hpp"

ChannelInvite::ChannelInvite(const Client *sender, const Client *receiver)
    : sender(sender), receiver(receiver), created_at(time(NULL)), expires_at(created_at + INVITE_DURATION), valid(true){};

const Client* ChannelInvite::get_sender() const {
    return sender;
}

const Client* ChannelInvite::get_receiver() const {
    return receiver;
}


time_t ChannelInvite::get_created_at() const {
    return created_at;
}

time_t ChannelInvite::get_expires_at() const {
    return expires_at;
}

bool ChannelInvite::is_sender(const Client *client) const
{
    return client == sender ? true : false;
}

bool ChannelInvite::is_receiver(const Client *client) const
{
    return client == receiver ? true : false;
}

bool ChannelInvite::is_expired() const
{
    return time(NULL) > expires_at;
}

bool ChannelInvite::is_valid() const
{
    return valid;
}

void ChannelInvite::invalidate()
{
    valid = false;
    // sender = NULL;
    //receiver = NULL; is this needed?
}
