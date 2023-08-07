#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "client.h"

TEST(ClientTest, InputTestCorrectInput) {
  Client client;
  std::stringstream test_input("12323123");
  std::thread client_thread([&]() { client.Input(std::ref(test_input)); });

  std::this_thread::sleep_for(std::chrono::microseconds(500));
  std::string value = client.GetBufferValue();
  EXPECT_TRUE(value == "333KBKBKB11");
  client.StopProcessing();
  client_thread.join();
}

TEST(ClientTest, InputTestIncorrectInput) {
  Client client;
  std::stringstream test_input("safsad");
  std::thread client_thread([&]() { client.Input(std::ref(test_input)); });

  std::this_thread::sleep_for(std::chrono::microseconds(500));
  std::string value = client.GetBufferValue();
  EXPECT_TRUE(value == "");
  client.StopProcessing();
  client_thread.join();
}

TEST(ClientTest, InputTestIncorrectLenghtInput) {
  Client client;
  std::stringstream test_input(
      "0123456789012345678901234567890123456789012345678901234567890123456789");
  std::thread client_thread([&]() { client.Input(std::ref(test_input)); });

  std::this_thread::sleep_for(std::chrono::microseconds(500));
  std::string value = client.GetBufferValue();
  EXPECT_TRUE(value == "");
  client.StopProcessing();
  client_thread.join();
}

TEST(ClientTest, InputTestEmptyInput) {
  Client client;
  std::stringstream test_input("");
  std::thread client_thread([&]() { client.Input(std::ref(test_input)); });

  std::this_thread::sleep_for(std::chrono::microseconds(500));
  std::string value = client.GetBufferValue();
  EXPECT_TRUE(value == "");
  client.StopProcessing();
  client_thread.join();
}

TEST(ClientTest, HandlingTestCorrectInput) {
  Client client;
  client.SetBufferValue("3KBKB1");
  std::thread client_thread(&Client::Handling, &client);

  std::this_thread::sleep_for(std::chrono::microseconds(500));
  client.StopProcessing();
  client_thread.join();
  int summary = client.GetSummaryOfNumbers();
  EXPECT_EQ(summary, 4);
}

TEST(ClientTest, HandlingTestIncorrectInput) {
  Client client;
  client.SetBufferValue("asdsadasd");
  std::thread client_thread(&Client::Handling, &client);

  std::this_thread::sleep_for(std::chrono::microseconds(500));
  client.StopProcessing();
  client_thread.join();
  int summary = client.GetSummaryOfNumbers();
  EXPECT_EQ(summary, 0);
}

TEST(ClientTest, HandlingTestEmptyInput) {
  Client client;
  client.SetBufferValue("");
  std::thread client_thread(&Client::Handling, &client);

  std::this_thread::sleep_for(std::chrono::microseconds(500));
  client.StopProcessing();
  client_thread.join();
  int summary = client.GetSummaryOfNumbers();
  EXPECT_EQ(summary, 0);
}

TEST(ClientTest, StartStop) {
  Client client;
  client.SetBufferValue("");
  std::thread client_thread(&Client::StartProcessing, &client);
  client.StopProcessing();
  std::this_thread::sleep_for(std::chrono::microseconds(500));
  client_thread.join();
  int summary = client.GetSummaryOfNumbers();
  EXPECT_EQ(summary, 0);
}

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
