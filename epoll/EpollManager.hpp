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

	public:
		void addFd(int fd, uint32_t newEvent);
		void deleteFd(int fd);
		void modFd(int fd, uint32_t newEvent);

	public:
		int getEpSock(void) const;

};
