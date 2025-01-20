#ifndef ERROR_HPP
#define ERROR_HPP

#define RPL_WELCOME(nick, host) ":" + host + " 001 " + nick + " :Welcome " + nick + " to the ft_irc network\r\n"
#define RPL_YOURHOST(nick, host) ":" + host + " 002 " + nick + " :Your host is " + host + " running version 1.0\r\n"
#define RPL_CREATED(nick, host) ":" + host + " 003 " + nick + " :This server was created 2024-08-1\r\n"
#define RPL_MYINFO(nick, host) ":" + host + " 004 " + nick + " :Host: " + host + ", Version: 1.0, User mode: none, Channel modes: o, t, k, i\r\n"

#define ERR_NOTREGISTERED(nick, host) ":" + host + " 451\r\n" + nick + ": You have not registered\r\n"
#define ERR_NEEDMOREPARAMS(nick, host) ":" + host + " 461\r\n" + nick + ": Not enough parameters\r\n"
#define ERR_ALREADYREGISTERED(nick, host) ":" + host + " 462\r\n" + nick + ": You may not reregister\r\n"
#define ERR_PASSWDMISMATCH(nick, host) ":" + host + " 464\r\n" + nick + ": Password incorrect\r\n"

#define ERR_NONICKNAMEGIVEN(nick, host) ":" + host + " 431\r\n" + nick + ": No nickname given\r\n"
#define ERR_ERRONEUSNICKNAME(nick, host) ":" + host + " 432\r\n" + nick + ": Erroneus nickname\r\n"
#define ERR_NICKNAMEINUSE(nick, host) ":" + host + " 433\r\n" + nick + ": Nickname is already in use\r\n"


// #define ERR_CHANNELISFULL(nick, channelName) ":" + nick + " 471 " + channelName + " :Cannot join channel (+l)\r\n"
// #define ERR_INVITEONLY(nick, channelName) ":" + nick + " 473 " + channelName + " :Cannot join channel (+i)\r\n"
#define ERR_BADCHANNELKEY(nick, host, channelName) ":" + host + " 475\r\n" + nick + " " + channelName + ": Cannot join channel (+k)\r\n"
#define ERR_BADCHANMASK(host, channelName) ":" + host + " 476\r\n" + channelName + ": Bad Channel Mask\r\n"

// // //topic
// // S <-   :dan!d@Clk-830D7DDC TOPIC #v3 :This is a cool channel!!
// // S <-   :irc.example.com 332 dan #v4 :Coolest topic
// // S <-   :irc.example.com 333 dan #v4 dan 1547858123

// #define RPL_NOTOPIC(nick, host, channel) ":" + host + " 331 " + nick + " " + channel + " :No topic is set\r\n"
// #define RPL_TOPIC(nick, host, channel, topic) ":" + host + " 332 " + nick + " " + channel + " :" + topic + "\r\n"
// #define RPL_TOPICTIME(nick, host, channel, nickSeter, time) ":" + host + " 333 " + nick + " " + channel + " " + nickSeter + " " + time + "\r\n"


// #define RPL_JOIN(nick, username, channelname, ipaddress) ":" + nick + "!~" + username + "@" + ipaddress + " JOIN " + channelname + "\r\n"
// #define RPL_PART(host, nick, username, channel, reason) ":"+nick+"!"+username+ "@" + host +" PART "  + channel+" "+reason+ "\r\n"
// #define RPL_QUIT(host, nick, username, reason) ":"+nick+"!"+username+ "@" + host +" QUIT :"+reason+ "\r\n"

// #define RPL_NAMREPLY(host, clients, channelname, nick) ":" + host + " 353 " + nick + " = " + channelname + " :" + clients + "\r\n"
// #define RPL_ENDOFNAMES(host, nick, channelname) ":" + host + " 366 " + nick + " " + channelname + " :END of /NAMES list\r\n"

// #define RPL_MODEIS(channel, host, mode) ":" + host + " MODE " + channel + " " + mode + "\r\n"
// #define RPL_CHANNELMODEIS(channel, host, mode, modeArgs) ":" + host + " MODE " + channel + " " + mode + " " + modeArgs "\r\n"
// #define RPL_MODEISLIMIT(channel, host, mode, newlimit) ":" + host + " MODE " + channel + " " + mode + " " + newlimit + "\r\n"
// #define RPL_MODEISOP(channel, host, mode, target) ":" + host + " MODE " + channel + " " + mode + " " + target + "\r\n"
// #define ERR_INVALIDMODEPARAM(channel, host, flag) ":" + host + " 696 " + channel + " " + flag + " * you must specifiy a parameter for the op mode\r\n"
// #define ERR_USERNOTINCHANNEL(host, channel) ":" + host + " 441 " + channel + " " + ":they aren't on that channel\r\n"
// #define ERR_NOTOP(host, channel) ":" + host + " 482 " + channel + " " + ":You're not a channel operator\r\n"

// #define RPL_CREATIONTIME(host, channelName, nick, time) ":" + host + " 329 " + nick + " " + channelName + " " + time + "\r\n"
// #define RPL_CHANNELMODES(host, channelName, nick, channelmode) ":" + host + " 324 " + nick + " " + channelName + " " + channelmode + "\r\n"

// #define RPL_INVITING(host, inviting, invited, channel) ":" + host + " 341 " + inviting + " " + invited + " " + channel + " :Inviting " + invited + " to " + channel + "\r\n"
// #define RPL_INVITE(nick, username, clienthost, invited, channel) ":" + nick + "!" + username + "@" + clienthost + " INVITE " + invited + " :" + channel + "\r\n"
#define ERR_NOSUCHNICK(host, nick) ":" + host + " 401\r\n" + nick + ": No such nick/channel\r\n"
#define ERR_NOSUCHCHANNEL(host, channel, nick) ":" + host + " 403\r\n" + nick + " " + channel + ": No such channel\r\n"
#define ERR_NOTONCHANNEL(host, channel, nick) ":" + host + " 442\r\n" + nick + " " + channel + ": You're not on that channel\r\n"
// #define ERR_USERONCHANNEL(host, channel, nick) ":" + host + " 443 " + nick + " " + channel + "  :is already on channel\r\n"

// #define ERR_INPUTTOOLONG(nick, host) ":" + host + " 417 " + nick + " :Input line was too long\r\n"

// #define RPL_VIEWTOPIC(host, nick, channel, topic) ":" + host + " 332 " + nick + " " + channel + " :" + topic + "\r\n"
// #define RPL_VIEWTIME(host, nick, channel, time) ":" + host + " 333 " + nick + " " + channel + " " + nick + " " + time + "\r\n"
// #define RPL_SETTOPIC(nick, host, channel, topic) ":" + nick + "!" + nick + "@" + host + " TOPIC " + channel + " :" + topic + "\r\n"

// #define RPL_BOT(host, nick, message) ":" + host + " 001 " + nick + " Dad joke: " + message + "\r\n"
// #define ERR_BOT(host, nick) ":" + host + " 450 " + nick + "enter [BOT generate] to generate a joke" + "\r\n"
// #define RPL_UMODEIS(host, channelname) ":" + host + " MODE " + channelname + " +nt\r\n"

// #define ERR_UNKNOWNMODE(nick, host, channel, character) ":" + host + " 472 " + nick + " " + channel + " " + character + " :is unknown mode char to me\r\n"
// #define RPL_YOUREOPER(host, nick) ":" + host + " 381 " + nick + ":You are now an IRC operator\r\n"
// #define RPL_KICK(kicker, username, host, channel, targetuser, reason) "\r\n:" + kicker + "!" + username + "@" + host + " KICK " + channel + " " + targetuser + " :" + reason + "\r\n"
// #define PRIVMSG_FORMATCH(senderNick, senderUsername, senderhost, receiver, message) ":" + senderNick + "!~" + senderUsername + "@" + senderhost + " PRIVMSG " + receiver + " :" + message + "\r\n"
// #define PRIVMSG_FORMATUSER(senderNick, receiver, message) ":" + senderNick + " PRIVMSG " + receiver + " :" + message + "\r\n"
#define ERR_UNKNOWNCOMMAND(nick, host, command) ":" + host + " 421 " + nick + " " + command + " :Unknown command\r\n"

// #define ERR_CHANOPRIVSNEEDED(nick, host, channel) ":" + host + " 482 " + nick + " " + channel + " :You're not channel operator\r\n"

// #define RPL_INVITELIST(nick, host, invitedChannel) ":" + host + " 346 " + nick + " " + invitedChannel + " :Invite list\r\n"
// #define RPL_ENDOFINVITELIST(nick, host) ":" + host + " 347 " + nick + " :End of invite list\r\n"
// #define RPL_NOTOPIC(nick, host, channel) ":" + host + " 331 " + nick + " " + channel + " :No topic is set\r\n"

// #define ERR_CANNOTSENDTOCHAN(nick, channel, host) ":" + host + " 404 " + nick + " " + channel + " :Cannot send to channel\r\n"

// // PING PONG
// #define RPL_PONG(host, nick) ":" + nick + " PING " + host + "\r\n"
// #define RPL_PING(host, nick) ":" + host + " PONG " + nick + "\r\n"

#endif