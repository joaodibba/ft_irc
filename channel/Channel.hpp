#pragma once

#include "ChannelMode.hpp"
#include "../client/Client.hpp"
#include "../server/Irc.hpp"

class ChannelUser;

class Channel
{
private:
    string _channelName;
    string _channelTopic;
    ChannelMode _modes;
    map<int, ChannelUser *> _users; // client socket file descriptor and pointer to ChannelUser

public:
    explicit Channel(const string &name);
    ~Channel();

    string get_channel_name() const;

    void set_channel_name(const string &channel_name);

    string get_channel_topic() const;

    void set_channel_topic(const string &channel_topic);

    ChannelMode &modes();

    const ChannelMode &modes() const;

    bool add_client(Client *client);

    bool remove_client(const Client *client);

    bool is_member(const Client *client) const;

    bool is_operator(const Client *client) const;

    void set_operator(const Client *client, bool is_operator);

    bool is_invited(const Client *client) const;

    void set_invited(const Client *client, bool is_invited);

    bool is_full() const;

    size_t size() const;

    void send_private_message(Client *client, const string &message);

    void send_message(const string &message);

    map<int, ChannelUser *> getUsers();

    void leave_channel(Client *client);
};
