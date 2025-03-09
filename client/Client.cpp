#include "Client.hpp"

Client::Client(int socket) : _connectionSock(socket), _authenticated(false) {}

Client::~Client(void) {}

int Client::getSock(void) const
{
	return (_connectionSock);
}

bool Client::getAuthenticated(void) const
{
	return (_authenticated);
}

string Client::getNick(void) const
{
	return (_nick);
}

string Client::getUser(void) const
{
	return (_user);
}

string Client::getPassWord(void) const
{
	return (_passWord);
}

string Client::getBuffer(void) const
{
	return (_buffer);
}

void Client::setBuffer(string buffer)
{
	_buffer = buffer;
}

void Client::setAuthenticated(bool auth)
{
	_authenticated = auth;
}

void Client::setNick(string name)
{
	_nick = name;
}

void Client::setUser(string name)
{
	_user = name;
}

void Client::setPassWord(string pass)
{
	if (pass.empty() == true)
		throw runtime_error("Password cannot be empty");
	if (pass.find(",") != string::npos)
		throw runtime_error("Password cannot contain ','");
	_passWord = pass;
}
