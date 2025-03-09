#pragma once

#include <iostream>
#include <string>
#include <fstream>
#include <algorithm>
#include <vector>
#include <map>
#include <sstream>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstring>
#include <csignal>
#include <cstdlib>
#include <ctime>
#include <cstdio>
#include <sys/epoll.h>
#include <fcntl.h>

#define END "\033[0m"
#define RED "\033[1;31m"
#define GREEN "\033[1;32m"
#define YELLOW "\033[1;33m"
#define BLUE "\033[1;34m"
#define MAGENTA "\033[1;35m"
#define CYAN "\033[1;36m"
#define WHITE "\033[1;37m"

#define BACKLOG 100
#define MAX_EVENTS 1024
#define BUFFER_SIZE 4096 
#define MAX_TCP_PORT 65535

using std::cerr;
using std::cout;
using std::endl;
using std::exception;
using std::find;
using std::getline;
using std::ios;
using std::istringstream;
using std::make_pair;
using std::map;
using std::ofstream;
using std::pair;
using std::runtime_error;
using std::string;
using std::stringstream;
using std::vector;

class Client;
class Channel;
class EpollManager;

#include "serverNumeric.hpp"
#include "../client/Client.hpp"
#include "../channel/Channel.hpp"
#include "../epoll/EpollManager.hpp"

extern bool running;

int ssLength(istringstream &ss);
void sendMsg(int fd, string msg);
void logger(int type, int data);

class Irc
{
public:
	Irc(void);
	~Irc(void);
	int run_server(char **av);
	void saveData(void) const;
	// ServerUtils
	Client *findClient(int target);
	Client *findClient(string name);
	Channel *findChannel(string name);
	Channel *createChannel(string name);
	void deleteChannel(string name);
	void leaveAllChannels(Client *ptr);
	void deleteClient(map<int, Client *>::iterator &it);

private:
	int _port;
	string _serverPassWord;

	int _serverSock;
	EpollManager *epfds;
	map<int, Client *> _clients;
	map<int, string> requests;
	vector<Channel *> _serverChannels;

	void setPassword(string password);
	void setPort(int port);

	// ServerNetwork
	void initNetwork(void);
	void setNonBlocking(int fd);
	bool isNewClient(int targetFd);
	void acceptClient(int serverFd);

	// Server
	void sendResponse(int targetFd);
	void receiveRequest(int targetFd);

	// ServerChannelModes
	void applyInviteOnlyFlag(bool optr, Channel *targetChannel);
	void applyTopicRestrictionFlag(bool optr, Channel *targetChannel);
	void applyMode(istringstream &ss, Channel *targetChannel, Client *client, string modeFlag); // ! FIXME maybe use this
	bool applyPasswordFlag(istringstream &ss, string &modeFlag, Client *client, Channel *targetChannel);
	bool applyLimitRestrictionFlag(istringstream &ss, string &modeFlag, Client *client, Channel *targetChannel);
	bool applyOperatorPrivilegeFlag(istringstream &ss, string &modeFlag, Client *client, Channel *targetChannel);

	//Commands
	typedef void (Irc::*CommandPtr)(istringstream &line, Client *client);
	map<string, CommandPtr> cmds;

	void sendToChannel(Client *sender, const string &channelName, const string &message);
	void sendToUser(Client *sender, const string &recipient, const string &message);

	void privmsgCmd(istringstream &ss, Client *client);
	void joinCmd(istringstream &ss, Client *client);
	void partCmd(istringstream &ss, Client *client);
	void topicCmd(istringstream &ss, Client *client);
	void modeCmd(istringstream &ss, Client *client);
	void passCmd(istringstream &ss, Client *client);
	void nickCmd(istringstream &ss, Client *client);
	void userCmd(istringstream &ss, Client *client);
	void inviteCmd(istringstream &ss, Client *client);
	void quitCmd(istringstream &ss, Client *client);
	void kickCmd(istringstream &ss, Client *client);
};
