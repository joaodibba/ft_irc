#include "EpollManager.hpp"

/**
 * @brief Construct a new Epoll Manager object
 *
 * Creates a new epoll instance and stores the file descriptor in epSock member.
 * 
 * @throws std::runtime_error if epoll_create fails
*/
EpollManager::EpollManager(void) : epSock(epoll_create(1))
{
	if (epSock < 0)
		throw runtime_error("creating epoll instance: " + std::string(strerror(errno)));
}

/**
 * @brief Destroy the Epoll Manager object
 *
 * Closes the epoll instance and deletes all file descriptors stored in listFds. 
*/

EpollManager::~EpollManager(void)
{
	for (size_t i = 0; i < listFds.size(); ++i)
	{
		deleteFd(listFds[i]);
	}
	close(epSock);
}

/**
 * @brief Get the epoll file descriptor
 * 
 * @return int epoll file descriptor
 */
int EpollManager::getEpSock(void) const
{
	return epSock;
}

/**
 * @brief Add a file descriptor to the epoll instance
 * 
 * Adds a file descriptor to the epoll instance and stores it in listFds.
 * 
 * @param targetFd file descriptor to add
 * @param newEvent event to listen
*/
void EpollManager::addFd(int targetFd, uint32_t newEvent)
{
	modifyEpollFd(EPOLL_CTL_ADD, targetFd, newEvent);
	listFds.push_back(targetFd);
}

/**
 * @brief Modify a file descriptor in the epoll instance
 * 
 * Modifies a file descriptor in the epoll instance.
 * If the file descriptor is not found in listFds, it does nothing.
 * 
 * @param targetFd file descriptor to modify
 * @param newEvent new event to listen
*/
void EpollManager::modFd(int targetFd, uint32_t newEvent)
{
	bool found = false;

	for (size_t i = 0; i < listFds.size(); ++i)
	{
		if (listFds[i] == targetFd)
		{
			found = true;
			break;
		}
	}

	if (!found)
		return;

	modifyEpollFd(EPOLL_CTL_MOD, targetFd, newEvent);
}

/**
 * @brief Delete a file descriptor from the epoll instance
 * 
 * Deletes a file descriptor from the epoll instance and closes the file descriptor.
 * If the file descriptor is not found in listFds, it does nothing.
 * 
 * @param targetFd file descriptor to delete
 */
void EpollManager::deleteFd(int targetFd)
{
	epoll_ctl(epSock, EPOLL_CTL_DEL, targetFd, NULL);
	close(targetFd);

	for (size_t i = 0; i < listFds.size(); ++i)
	{
		if (listFds[i] == targetFd)
		{
			listFds.erase(listFds.begin() + i);
			break;
		}
	}
}

/**
 * @brief Modify a file descriptor in the epoll instance
 * 
 * Modifies a file descriptor in the epoll instance.
 * 
 * @param op operation to perform (EPOLL_CTL_ADD, EPOLL_CTL_MOD, EPOLL_CTL_DEL)
 * @param targetFd file descriptor to modify
 * @param newEvent new event to listen
 * 
 * @throws std::runtime_error if epoll_ctl fails
 */
void EpollManager::modifyEpollFd(int op, int targetFd, uint32_t newEvent)
{
	struct epoll_event ev;
	memset(&ev, 0, sizeof(ev));
	ev.events = newEvent;
	ev.data.fd = targetFd;

	if (epoll_ctl(epSock, op, targetFd, &ev) == -1)
		throw runtime_error("Cannot modify fd in epoll instance: " + std::string(strerror(errno)));
}
