#ifndef SRC_ANALYSATOR_DATA_H_
#define SRC_ANALYSATOR_DATA_H_

#include <arpa/inet.h>
#include <unistd.h>

#include <iostream>

#define PORT 12345
#define TWO_SYMBOLS 99
#define MULTIPLE_OF_32 32

class Server {
 public:
  int GetData();

 private:
  int CreatingSocket();
  void FillingServer(sockaddr_in &server_address);
  void BindingAndListeningSocket(sockaddr_in &server_address, int &data_socket);
  void CheckingNumber();
  void RecivingData(sockaddr_in &client_address, int &data_socket);

  int summary_of_numbers_ = 0;
};

#endif  // SRC_ANALYSATOR_DATA_H_
