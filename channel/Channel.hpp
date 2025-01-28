#pragma once

#include "../Irc.hpp"

class Channel
{
	private:
		string 				_channelName;
		string 				_channelModes;
		string				_channelTopic; // t:set/remove restrictions of the TOPIC
		string				_channelPassword; // k: set/remove the channel key (password)
		map<Client*, bool>	_channelUsers;
		vector<string>		_inviteUsers; // i: set/remove invite-only channel
		size_t				_maxUsersNumber; // l: set/remove limit of user on channel

	public:
		string getChannelName(void) const;
		string getChannelTopic(void) const;
		string getChannelModes(void) const;
		string getChannelPassword(void) const;
		size_t getMaxUsersNumber(void) const;
		size_t getNumberOfUsersOnChannel(void) const;

		void setChannelModes(char flag);
		void setInviteUser(string nick);
		void setMaxUsersNumber(size_t nb);
		void setChannelTopic(string content);
		void setChannelPassword(string pass);
		void setChannelUser(bool oprt, Client* ptr);

		void removeClient(Client* ptr);
		void removeChannelModesFlag(char flag);

		void sendAll(string msg) const;
		void sendPrivMsg(int fd, string msg) const;

		bool isChannelFull(void) const;
		bool isFlagSet(char flag) const;
		bool isOperator(string nick) const;
		bool isUserInvited(string nick) const;
		bool isPartOfChannel(string nick) const;

		void giveOrTakeOperatorPrivilege(string targetNick, bool privilege = false);

	public:
		Channel(string name);
		~Channel(void);

};

