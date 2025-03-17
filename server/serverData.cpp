#include "Irc.hpp"

static string	intToStr(int value)
{
	stringstream	ss;

	ss << value;
	return (ss.str());
}

static string	boolToStr(bool b)
{
	return (b ? "true" : "false");
}

static string	timeToISO8601(time_t rawTime)
{
	struct tm	*ptm;
	char		buffer[32];

	ptm = gmtime(&rawTime);
	if (!ptm)
		return ("1970-01-01T00:00:00+00:00");
	strftime(buffer, sizeof(buffer), "%Y-%m-%dT%H:%M:%S", ptm);
	return (buffer);
}

void Irc::saveClients() const
{
	string filename_client = "data/clients.csv";
	string header = "fd_cl,nick,auth_state,authenticated,buffer";
	ofstream outfile_client(filename_client.c_str(), ios::trunc);
	if (!outfile_client)
	{
		cerr << "Error: Unable to open file for writing: " << filename_client << endl;
		return ;
	}

	outfile_client << header << endl;

	for (map<int, Client *>::const_iterator it = _clients.begin();
			it != _clients.end();
			++it)
	{
		if (!it->second)
			continue ;
		outfile_client
			<< it->first << ","
			<< it->second->getNick() << ","
			<< intToStr(it->second->getAuthState()) << ","
			<< boolToStr(it->second->isAuthenticated()) << ","
			<< it->second->getBuffer()
			<< std::endl;
	}
	outfile_client.close();
}

void Irc::saveRequests() const
{
	string filename_requests = "data/requests.csv";
	ofstream outFile_requests(filename_requests.c_str(), std::ios::app);
	static bool printedHeader = false;
	if (!outFile_requests)
	{
		cerr << "Error: Unable to open file for writing: "
				<< filename_requests << endl;
		return ;
	}

	if (!printedHeader)
	{
		outFile_requests << "fd_client,string" << std::endl;
		printedHeader = true;
	}
	for (map<int,
			std::string>::const_iterator it_r = requests.begin(); it_r != requests.end(); ++it_r)
		outFile_requests << it_r->first << "," << it_r->second << std::endl;
	outFile_requests.close();
}

void Irc::saveChannels() const
{
	string filename_serverChannel = "data/channels.csv";
	string header = "channel_name,topic,invite_only,topic_restricted,password_protected,user_limited,user_limit,password";
	ofstream outFile_serverChannel(filename_serverChannel.c_str(),
			std::ios::trunc);
	if (!outFile_serverChannel)
	{
		cerr << "Error: Unable to open file for writing: " << filename_serverChannel << endl;
		return ;
	}

	outFile_serverChannel << header << std::endl;

	for (vector<Channel *>::const_iterator it_sc = _serverChannels.begin(); it_sc != _serverChannels.end(); ++it_sc)
	{
		Channel *channel = *it_sc;
		if (!channel)
			continue ;

		ChannelMode &modes = channel->modes();

		outFile_serverChannel
			<< channel->get_channel_name() << ","
			<< channel->get_channel_topic() << ","
			<< boolToStr(modes.is_invite_only()) << ","
			<< boolToStr(modes.is_topic_restricted()) << ","
			<< boolToStr(modes.is_password_protected()) << ","
			<< boolToStr(modes.is_user_limited()) << ","
			<< intToStr(modes.get_user_limit()) << ","
			<< modes.get_password()
			<< endl;
	}
	outFile_serverChannel.close();
}

void Irc::saveChannelUsers() const
{
	string filename_channelUser = "data/channel_users.csv";
	string header = "channel_name,user_nick,user_fd,is_operator";
	ofstream outFile_channelUser(filename_channelUser.c_str(), std::ios::trunc);
	if (!outFile_channelUser)
	{
		std::cerr << "Error: Unable to open file for writing: " << filename_channelUser << std::endl;
		return ;
	}
	outFile_channelUser << header << std::endl;

	for (vector<Channel *>::const_iterator it_sc = _serverChannels.begin();
			it_sc != _serverChannels.end();
			++it_sc)
	{
		Channel *channel = *it_sc;
		if (!channel)
			continue ;
		map<int, ChannelUser *> users = channel->users();

		for (map<int,
				ChannelUser *>::const_iterator it_cu = users.begin(); it_cu != users.end(); ++it_cu)
		{
			ChannelUser *channelUser = it_cu->second;
			if (!channelUser || !channelUser->get_client())
				continue ;
			outFile_channelUser
				<< channel->get_channel_name() << ","
				<< channelUser->get_client()->getNick() << ","
				<< channelUser->get_client()->getSock() << ","
				<< boolToStr(channelUser->is_operator())
				<< endl;
		}
	}
	outFile_channelUser.close();
}

void Irc::saveChannelInvites() const
{
	string filename = "data/channel_invites.csv";
	string header = "channel_name,sender_nick,sender_sock,receiver_nick,receiver_sock,created_at,expires_at,is_valid";
	ofstream outfile(filename.c_str(), std::ios::trunc);
	if (!outfile)
	{
		cerr << "Error: Unable to open file for writing: " << filename << endl;
		return ;
	}

	outfile << header << std::endl;

	for (vector<Channel *>::const_iterator it_sc = _serverChannels.begin();
			it_sc != _serverChannels.end();
			++it_sc)
	{
		Channel *channel = *it_sc;
		if (!channel)
			continue ;

		const vector<ChannelInvite> &invites = channel->get_invites();
		for (vector<ChannelInvite>::const_iterator it_ci = invites.begin();
				it_ci != invites.end();
				++it_ci)
		{
			const ChannelInvite &channelInvite = *it_ci;
			const Client *sender = channelInvite.get_sender();
			const Client *receiver = channelInvite.get_receiver();
			if (!sender || !receiver)
				continue ;

			outfile
				<< channel->get_channel_name() << ","
				<< sender->getNick() << ","
				<< sender->getSock() << ","
				<< receiver->getNick() << ","
				<< receiver->getSock() << ","
				<< timeToISO8601(channelInvite.get_created_at()) << ","
				<< timeToISO8601(channelInvite.get_expires_at()) << ","
				<< boolToStr(channelInvite.is_valid())
				<< endl;
		}
	}
	outfile.close();
}
