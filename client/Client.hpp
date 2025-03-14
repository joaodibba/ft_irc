#pragma once

#include "../server/Irc.hpp"

class Client
{
private:
	int _connectionSock; // socket file descriptor that represents the client connection and the client's unique identifier
	authState _authState;
	bool _authenticated;

	string _nick;
	string _user;
	string _passWord;
	string _buffer;
	string _realname;

public:
	explicit Client(int connectionSock);
	~Client();

	int getSock() const;
	
	string getBuffer() const;
	string getNick() const;
	string getUser() const;
	string getPassWord() const;
	string getHostName() const;
	string getRealName() const;
	authState getAuthState() const;
	bool isAuthenticated() const;

	void setNick(const string &name);
	void setUser(const string &name);
	void setPassWord(const string &pass);
	void setBuffer(const string &buffer);
	void setRealName(const string &realname);
	void setAuthState(authState state);
	void setAuthenticated(bool auth);
};
