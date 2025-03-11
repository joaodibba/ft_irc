#pragma once

#include "../server/Irc.hpp"

class Client
{
private:
	int _connectionSock; // socket file descriptor that represents the client connection and the client's unique identifier
	bool _authenticated;

	string _nick;
	string _user;
	string _passWord;
	string _buffer;

public:
	explicit Client(int connectionSock);
	~Client();

	int getSock() const;
	bool getAuthenticated() const;

	string getBuffer() const;
	string getNick() const;
	string getUser() const;
	string getPassWord() const;
	string getHostName() const;

	void setAuthenticated(bool auth);
	void setNick(const string &name);
	void setUser(const string &name);
	void setPassWord(const string &pass);
	void setBuffer(const string &buffer);
};
