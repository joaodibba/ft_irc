#pragma once

#include "../client/Client.hpp"
#include <ctime>

#define INVITE_DURATION 60

class ChannelInvite {
private:
    const Client *sender;
    const Client *receiver;

    const time_t created_at;
    const time_t expires_at;

    bool valid;

public:
    ChannelInvite(const Client *sender, const Client *receiver);

    const Client* get_sender() const;
    const Client* get_receiver() const;

    time_t get_created_at() const;

    time_t get_expires_at() const;

    bool is_sender(const Client *client) const;
    bool is_receiver(const Client *client) const;

    bool is_expired() const;
    bool is_valid() const;

    void invalidate();
};
