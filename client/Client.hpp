#pragma once

#include "../server/Irc.hpp"

class Client
{
private:
	int _connectionSock;
	int _isAuthenticated;

	string _nick;
	string _user;
	string _passWord;

public:
	string _buffer;
	Client(int connectionSock);
	~Client(void);

public:
	int getSock(void) const;
	int isAuthenticated(void) const;

	string getNick(void) const;
	string getUser(void) const;
	string getPassWord(void) const;
	string getHostName(void) const;

	void authenticated(void);
	void setNick(string name);
	void setUser(string name);
	void setPassWord(string pass);
};
