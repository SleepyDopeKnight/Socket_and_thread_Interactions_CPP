#ifndef SRC_CLIENT_H_
#define SRC_CLIENT_H_

#include <arpa/inet.h>
#include <unistd.h>

#include <algorithm>
#include <atomic>
#include <condition_variable>
#include <cstring>
#include <iostream>
#include <mutex>
#include <queue>
#include <sstream>
#include <string>
#include <thread>

constexpr int PORT = 12345;
constexpr int MAX_STRING_LENGHT = 64;

class Client {
 public:
  Client();
  ~Client();

  void StartProcessing();
  void StopProcessing();

  std::string GetBufferValue();
  int GetSummaryOfNumbers();
  void SetBufferValue(std::string buffer);

  void Input(std::istream &istream);
  void Handling();
  void Output();

 private:
  bool ValidationString(std::string input_line);
  void SortAndReplaceElements(std::string &input_line);
  void SendData(int &summary_of_numbers, sockaddr_in &server_address);
  int CreateSocket();
  void FillServer(sockaddr_in &server_address);

  std::string buffer_value_;
  std::mutex buffer_mutex_;
  std::condition_variable buffer_condition_value_;

  int summary_of_numbers_;
  std::mutex output_mutex_;
  std::queue<std::string> output_;

  bool buffer_ready_;
  std::atomic<bool> stop_processing_;
};

#endif  // SRC_CLIENT_H_