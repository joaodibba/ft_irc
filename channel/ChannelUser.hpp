#pragma once

#include "../client/Client.hpp"

class ChannelUser
{
private:
    Client *_client;
    bool _operator = false;
    bool _invited = false;

public:
    explicit ChannelUser(Client *client);
    ~ChannelUser();

    Client *get_client() const;

    bool is_operator() const;

    void set_operator(bool is_operator);

    bool is_invited() const;

    void set_invited(bool is_invited);
};