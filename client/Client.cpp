#include "Client.hpp"

Client::Client(const int connectionSock) : _connectionSock(connectionSock), _authenticated(false) {}

Client::~Client() {}

int Client::getSock() const
{
	return (_connectionSock);
}

bool Client::getAuthenticated() const
{
	return (_authenticated);
}

string Client::getNick() const
{
	return (_nick);
}

string Client::getUser() const
{
	return (_user);
}

string Client::getPassWord() const
{
	return (_passWord);
}

string Client::getBuffer() const
{
	return (_buffer);
}

string Client::getHostName() const
{
	return ("localhost");
}

string Client::getRealName() const
{
	return (_realname);
}

void Client::setBuffer(const string &buffer)
{
	_buffer = buffer;
}

void Client::setAuthenticated(const bool auth)
{
	_authenticated = auth;
}

void Client::setNick(const string &name)
{
	_nick = name;
}

void Client::setUser(const string &name)
{
	_user = name;
}

void Client::setPassWord(const string &pass)
{
	_passWord = pass;
}

void Client::setRealName(const string &realname)
{
	_realname = realname;
}
