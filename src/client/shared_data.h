#ifndef SRC_SHARED_DATA_H_
#define SRC_SHARED_DATA_H_

#include <arpa/inet.h>
#include <unistd.h>

#include <algorithm>
#include <atomic>
#include <condition_variable>
#include <iostream>
#include <mutex>
#include <queue>
#include <sstream>
#include <string>
#include <thread>

class Client {
 public:
  void StartProcessing();
  void StopInput();
  void StopHandling();
  void StopOutput();

  std::string GetBufferValue();
  int GetSummaryOfNumbers();
  void SetBufferValue(std::string buffer);

  void Input(std::istream &istream);
  void Handling();
  void Output();

 private:
  int ValidationString(std::string input_line);
  void SortingAndReplacingElements(std::string &input_line);
  void SendData(int &summary_of_numbers);
  int CreatingSocket();
  void FillingServer(sockaddr_in &server_address);

  std::string buffer_value_;
  std::mutex buffer_mutex_;

  std::mutex input_mutex_;
  std::mutex handling_mutex_;

  std::mutex output_mutex_;
  std::condition_variable buffer_condition_value_;
  int summary_of_numbers_ = 0;
  std::queue<std::string> output_;

  bool buffer_ready_ = false;
  std::atomic<bool> stop_input_ = false;
  std::atomic<bool> stop_handling_ = false;
  std::atomic<bool> stop_output_ = false;
};

#endif  // SRC_SHARED_DATA_H_