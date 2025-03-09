#pragma once

#include "../server/Irc.hpp"

class Client
{
private:
	int _connectionSock;
	bool _authenticated;

	string _nick;
	string _user;
	string _passWord;
	string _buffer;

public:
	Client(int connectionSock);
	~Client(void);

public:
	int getSock(void) const;
	bool getAuthenticated(void) const;

	string getBuffer(void) const;
	string getNick(void) const;
	string getUser(void) const;
	string getPassWord(void) const;
	string getHostName(void) const;
	
	void setAuthenticated(bool auth);
	void setNick(string name);
	void setUser(string name);
	void setPassWord(string pass);
	void setBuffer(string buffer);
};
