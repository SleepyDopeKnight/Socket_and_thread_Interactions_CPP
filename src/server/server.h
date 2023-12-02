#ifndef SRC_SERVER_H_
#define SRC_SERVER_H_

#include <arpa/inet.h>
#include <unistd.h>

#include <cstring>
#include <iostream>

constexpr int kPort = 12345;
constexpr int kTwoSymbols = 99;
constexpr int kMultipleOf_32 = 32;

class Server {
 public:
  Server();

  void StartServer();
  bool CheckNumber();
  void SetSummaryNumber(int number);

 private:
  int CreateSocket();
  void FillServer(sockaddr_in &server_address);
  void BindAndListenSocket(sockaddr_in &server_address, int &data_socket);
  void ReceiveData(sockaddr_in &client_address);

  int summary_of_numbers_ = 0;
  int data_socket_;
  sockaddr_in server_address_;
};

#endif  // SRC_SERVER_H_
