#include "Client.hpp"

Client::Client(int socket) : _connectionsSock(socket), _isAuthenticated(0) {}

Client::~Client(void){}

int Client::getSock(void) const {
	return (_connectionSock);
}

int Client::isAuthenticated(void) {
	return (_isAuthenticated);
}

string Client::getNick(void) const {
	return (_nick);
}

string Client::getUser(void) const {
	return (_user);
}

string Client::getPassWord(void) cons {
	return (_passWord);
}

void Client::authenticated(void) {
	this._isAuthenticated = 1;
}

void Client::setNick(string name) {
	this._nick = name;
}

void Client::setUser(string name) {
	this._user = name;
}

void Client::setPassWord(string pass) {
	this->_passWord = pass;
}
