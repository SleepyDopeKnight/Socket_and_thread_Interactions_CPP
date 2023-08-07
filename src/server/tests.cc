#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "server.h"

TEST(ServerTest, CheckNumberCorrectInput) {
  Server server;
	server.SetSummaryNumber(128);
	bool result = server.CheckNumber();
	EXPECT_TRUE(result);
	server.SetSummaryNumber(256);
	result = server.CheckNumber();
	EXPECT_TRUE(result);
	server.SetSummaryNumber(512);
	result = server.CheckNumber();
	EXPECT_TRUE(result);
		server.SetSummaryNumber(768);
	result = server.CheckNumber();
	EXPECT_TRUE(result);
}

TEST(ServerTest, CheckNumberIncorrectInput) {
  Server server;
	server.SetSummaryNumber(32);
	bool result = server.CheckNumber();
	EXPECT_FALSE(result);
	server.SetSummaryNumber(64);
	result = server.CheckNumber();
	EXPECT_FALSE(result);
	server.SetSummaryNumber(0);
	result = server.CheckNumber();
	EXPECT_FALSE(result);
		server.SetSummaryNumber(17);
	result = server.CheckNumber();
	EXPECT_FALSE(result);
}

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
