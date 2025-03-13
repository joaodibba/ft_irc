def notice_msg(message):
    return f":localhost NOTICE * :{message}\r\n"

def err_sample(code, err_name, nick):
    return f":localhost {code} {nick} :{err_name}\r\n"

def err_sample_2(code, err_name, nick, name):
    return f":localhost {code} {nick} {name} :{err_name}\r\n"

def err_sample_3(code, err_name, nick, name, channel_name):
    return f":localhost {code} {nick} {name} {channel_name} :{err_name}\r\n"

def rpl(nick, user, command, target, character, content):
    return f":{nick}!{user}@localhost {command} {target}{character}{content}\r\n"

def rpl_mode(nick, user, channel_name, flags):
    return rpl(nick, user, "MODE", channel_name, ' ', flags)

def rpl_msg(nick, user, target, msg):
    return rpl(nick, user, "MSG", target, " :", msg)

def rpl_privmsg(nick, user, target, msg):
    return rpl(nick, user, "PRIVMSG", target, " :", msg)

def rpl_part(nick, user, channel_name, reason_to_part):
    return rpl(nick, user, "PART", channel_name, " :", reason_to_part)

def rpl_join(nick, user, channel_name, msg):
    return rpl(nick, user, "JOIN", channel_name, " * :", msg)

def rpl_welcome(nick):
    return f":localhost 001 {nick if nick else '*'} :Welcome to the Internet Relay Network *!\r\n Type INFO for information.\r\n"

def rpl_info():
    return ("Put the server's password with command P\r\n"
            "INFO: Server Command Usage Guide\r\n"
            "=====================================\r\n"
            "1. PASS <password>      - Authenticate on the server using the password.\r\n"
            "2. NICK <nickname>      - Set a nickname for the user.\r\n"
            "3. USER <username> <hostname> <servername> <realname> - Register a user.\r\n"
            "4. JOIN <#channel>      - Join a specified channel.\r\n"
            "5. PART <#channel>      - Leave a specified channel.\r\n"
            "6. PRIVMSG <target> <message> - Send a private message.\r\n"
            "7. MODE <target> <modes> - Change user/channel modes.\r\n"
            "8. TOPIC <#channel> [new_topic] - View or set a channel topic.\r\n"
            "9. KICK <#channel> <user> [reason] - Remove a user from a channel.\r\n"
            "10. INVITE <user> <#channel> - Invite a user to a channel.\r\n"
            "11. QUIT [message]       - Disconnect from the server.\r\n"
            "=====================================\r\n"
            "NOTE: Follow the correct order for proper interaction.\r\n"
            "Example: PASS secretpass -> NICK MyNick -> USER myuser host serv RealName\r\n")

def err_nosuchnick(nick, target_nick):
    return err_sample_2("401", "No such nick", nick, target_nick)

def err_nosuchchannel(nick, channel_name):
    return err_sample_2("403", "No such channel", nick, channel_name)

def err_cannotsendtochan(nick, channel_name):
    return err_sample_2("404", "Cannot send to channel", nick, channel_name)

def err_notexttosend(nick):
    return err_sample("412", "No text to send", nick)

def err_unknowncommand(nick, target_command):
    return err_sample_2("421", "unknown command", nick, target_command)

def err_nonicknamegiven(nick):
    return err_sample("431", "No nickname given", nick)

def err_nicknameinuse(target_nick):
    return err_sample_2("433", "Nickname is already in use", "*", target_nick)

def err_notregistered(nick):
    return err_sample("451", "You have not registered", nick)

def err_needmoreparams(nick, command):
    return err_sample_2("461", "Not enough parameters", nick, command)

def err_passwdmismatch(nick):
    return err_sample("464", "Password incorrect", nick)

def err_channelisfull(nick, channel_name):
    return err_sample_2("471", "Cannot join channel (+l)", nick, channel_name)

def err_chanoprivsneeded(nick, channel_name):
    return err_sample_2("482", "You're not channel operator", nick, channel_name)
