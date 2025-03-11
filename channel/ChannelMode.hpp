#pragma once

#include <string>
#include <cstddef>

using namespace std;

class ChannelMode
{
private:
    bool _inviteOnly;
    bool _topicRestricted;
    bool _passwordProtected;
    size_t _userLimit;
    string _password;

public:
    ChannelMode();
    ~ChannelMode();

    bool is_invite_only() const;

    void set_invite_only(bool invite_only);

    bool is_topic_restricted() const;

    void set_topic_restricted(bool topic_restricted);

    bool is_password_protected() const;

    void set_password_protected(bool password_protected);

    bool is_user_limited() const;

    size_t get_user_limit() const;

    void set_user_limit(size_t user_limit);

    string get_password() const;

    void set_password(const string &password);
};