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
#include <fcntl.h>

//Cross platform includes (Helder)
#ifdef __linux__
	#include <sys/epoll.h>
#elif defined(__APPLE__)
	#include <sys/event.h>
	#include <sys/time.h>
#endif

#define END			"\033[0m]"
#define RED			"\033[1;31m"
#define GREEN		"\033[1;32m"
#define YELLOW		"\033[1;33m"
#define BLUE		"\033[1;34m"
#define MAGENTA		"\033[1;35m"
#define CYAN		"\033[1;36m"
#define WHITE		"\033[1;37m"

#define BACKLOG 100
#define MAX_EVENTS 1024

//#include "serverNumeric.hpp"
//#include "src/client/Client.hpp"
//#include "src/channel/Channel.hpp"
//#include "src/epoll/EpollManager.hpp"

//int ssLength(istringstream &ss);
//void sendMsg(int fd, string msg);
//void logger(int type, int data);

class Irc
{
	private:
		int _port;
		std::string _serverPassWord;

		int _serverSock;
//		EpollManager *epfds;
//		map<int , Client*> _clients;
//		map<int, string> requests;
//		vector<Channel*> _serverChannels;
//
//		// ServerNetwork
//		void initNetwork(void);
//		void setNonBloking(int *ptr);
//		bool isNewClient(int targetFd);
//		void acceptClient(int serverFd);
//
//		// Server
//		void sendResponse(int targetFd);
//		void receiveRequest(int targetFd);
//
//		// ServerChannelModes
//		void apllyInviteOnlyFlag(bool optr, Channel *targetChannel);
//		void apllyTopicRestrictionFlag(bool optr, Channel *targetChannel);
//		void applyMode(istringstream &ss, Channel *targetChannel, Client *client, string modeFlag);
//		bool apllyPasswordFlag(istringstream &ss, string &modeFlag, Client *client, Channel *targetChannel);
//		bool apllyLimitRestrictionFlag(istringstream &ss, string &modeFlag, Client *client, Channel *targetChannel);
//		bool applyOperatorPrivilegeFlag(istringstream &ss, string &modeFlag, Client *client, Channel *targetChannel);
//
//		// ServerUtils
//		Client *findClient(int target);
//		Client *findClient(string name);
//		Channel *findChannel(string name);
//		Channel *createChannel(string name);
//		void leaveAllChannels(Client *ptr);
//		void deleteClient(map<int, Client*>::iterator &it);

		// ServerCommands
//        typedef void (Irc::*CommandPtr)(istringstream &line, Client *client);
		std::map<std::string, void(*)()> commandMap;
//
//
//		void privmsgCmd(istringstream &ss, Client *client);
//		void joinCmd(istringstream &ss, Client *client);
//		void partCmd(istringstream &ss, Client *client);
//		void topicCmd(istringstream &ss, Client *client);
//		void modeCmd(istringstream &ss, Client *client);
//		void passCmd(istringstream &ss, Client *client);
//		void nickCmd(istringstream &ss, Client *client);
//		void userCmd(istringstream &ss, Client *client);
//		void inviteCmd(istringstream &ss, Client *client);
//		void quitCmd(istringstream &ss, Client *client);
//		void kickCmd(istringstream &ss, Client *client);

	public:
		Irc(void);
//		~Irc(void);
		int run_server(char **av);
//		void setPortAndPassword(char **av);
        void saveData(void) const;
};
