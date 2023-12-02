#include "server.h"

Server::Server() : summary_of_numbers_(0) {}

void Server::StartServer() {
  data_socket_ = CreateSocket();
  if (data_socket_ < 0) {
    return;
  }

  std::memset(&server_address_, 0, sizeof(server_address_));
  FillServer(server_address_);
  BindAndListenSocket(server_address_, data_socket_);

  struct sockaddr_in client_address;
  while (true) {
    ReceiveData(client_address);
    if (CheckNumber()) {
      std::cout << "Summary of Numbers: " << summary_of_numbers_ << std::endl;
    } else {
      std::cerr << "Invalid data (number is not a multiple of 32 or less than "
                   "3 characters)"
                << std::endl;
    }
  }
  close(data_socket_);
}

void Server::SetSummaryNumber(int number) { summary_of_numbers_ = number; }

int Server::CreateSocket() {
  int data_socket = socket(AF_INET, SOCK_STREAM, 0);
  if (data_socket < 0) {
    std::cerr << "Failed to create socket." << std::endl;
    return -1;
  }
  return data_socket;
}

void Server::FillServer(sockaddr_in &server_address) {
  std::memset(&server_address, 0, sizeof(server_address));
  server_address.sin_family = AF_INET;
  server_address.sin_port = htons(kPort);
  server_address.sin_addr.s_addr = INADDR_ANY;
}

void Server::BindAndListenSocket(sockaddr_in &server_address,
                                 int &data_socket) {
  if (bind(data_socket, (struct sockaddr *)&server_address,
           sizeof(server_address)) < 0) {
    std::cerr << "Failed to bind socket." << std::endl;
    close(data_socket);
    return;
  }
  if (listen(data_socket, 1) < 0) {
    std::cerr << "Failed to listen socket." << std::endl;
    close(data_socket);
    return;
  }
}

bool Server::CheckNumber() {
  return summary_of_numbers_ > kTwoSymbols &&
         summary_of_numbers_ % kMultipleOf_32 == 0;
}

void Server::ReceiveData(sockaddr_in &client_address) {
  socklen_t client_lenght = sizeof(client_address);
  int client_data_socket =
      accept(data_socket_, (struct sockaddr *)&client_address, &client_lenght);
  if (client_data_socket < 0) {
    std::cerr << "Failed to accept connection." << std::endl;
    return;
  }
  int data = recv(client_data_socket, &summary_of_numbers_,
                  sizeof(summary_of_numbers_), 0);
  if (data != sizeof(summary_of_numbers_)) {
    std::cerr << "Data received is corrupted." << std::endl;
    return;
  }
  close(client_data_socket);
}
