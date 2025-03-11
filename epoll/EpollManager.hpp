#pragma once

#include "../server/Irc.hpp"

class EpollManager
{
private:
	int epSock;
	vector<int> listFds;

public:
	EpollManager(void);
	~EpollManager(void);

	int getEpSock(void) const;

	void addFd(int fd, uint32_t newEvent);
	void deleteFd(int fd);
	void modifyEpollFd(int op, int targetFd, uint32_t newEvent);
	void modFd(int fd, uint32_t newEvent);
};
