#pragma once

#include "../client/Client.hpp"

class ChannelUser
{
private:
    Client *_client;
    bool _operator;

public:
    explicit ChannelUser(Client *client);
    ~ChannelUser();

    Client *get_client() const;

    bool is_operator() const;
    
    void set_operator(bool is_operator);
};