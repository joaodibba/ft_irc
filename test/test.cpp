#include "../server/Irc.hpp"
#include <gtest/gtest.h>

TEST(ModeCommandTest, NoChannelSpecified) {
    Irc irc;
    Client client("User1", "127.0.0.1");
    istringstream ss("");  // No channel specified

    testing::internal::CaptureStdout();
    irc.modeCmd(ss, &client);
    string output = testing::internal::GetCapturedStdout();

    EXPECT_TRUE(output.find("461") != string::npos); // ERR_NEEDMOREPARAMS
}

TEST(ModeCommandTest, NonExistentChannel) {
    Irc irc;
    Client client("User1", "127.0.0.1");
    istringstream ss("#nonexistent +i");

    testing::internal::CaptureStdout();
    irc.modeCmd(ss, &client);
    string output = testing::internal::GetCapturedStdout();

    EXPECT_TRUE(output.find("403") != string::npos); // ERR_NOSUCHCHANNEL
}

TEST(ModeCommandTest, NotOperator) {
    Irc irc;
    Client client("User1", "127.0.0.1");
    Channel channel("#test");
    irc.addChannel(&channel);  // Add channel to IRC
    istringstream ss("#test +i");

    testing::internal::CaptureStdout();
    irc.modeCmd(ss, &client);
    string output = testing::internal::GetCapturedStdout();

    EXPECT_TRUE(output.find("482") != string::npos); // ERR_CHANOPRIVSNEEDED
}

TEST(ModeCommandTest, SetInviteOnlyAsOperator) {
    Irc irc;
    Client client("Admin", "127.0.0.1");
    Channel channel("#test");
    irc.addChannel(&channel);
    channel.addOperator("Admin"); // Make the user an operator

    istringstream ss("#test +i");

    testing::internal::CaptureStdout();
    irc.modeCmd(ss, &client);
    string output = testing::internal::GetCapturedStdout();

    EXPECT_TRUE(output.find("324") != string::npos); // RPL_CHANNELMODEIS
}