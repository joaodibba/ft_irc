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
	if (pass.empty() == true)
		throw runtime_error("Password cannot be empty");
	if (pass.find(',') != string::npos)
		throw runtime_error("Password cannot contain ','");
	_passWord = pass;
}
