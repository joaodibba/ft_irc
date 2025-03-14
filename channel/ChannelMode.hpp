#pragma once

#include <string>
#include <cstddef>
#include <sstream>
#include <string>
#include <cerrno>
#include <cstdlib>

using namespace std;

// Declarações antecipadas
class Client;
class Channel;

class ChannelMode
{
private:
    bool _inviteOnly;
    bool _topicRestricted;
    bool _passwordProtected;
	bool _limited;
    size_t _userLimit;
    string _password;
	string _modes;

public:
    ChannelMode();
    ~ChannelMode();

    bool is_invite_only() const;

    void set_invite_only(bool invite_only);

    bool is_topic_restricted() const;

    void set_topic_restricted(bool topic_restricted);

    // param k
	bool is_password_protected() const;

    void set_password_protected(bool password_protected);

    string get_password() const;

    void set_password(const string &password);

	// param l
    bool is_user_limited() const;

    size_t get_user_limit() const;
	
	void set_limit(const bool flag);

    void set_user_limit(size_t user_limit);


	bool apllyLimitRestriction(istringstream& ss, bool Flag, Client* client);
};