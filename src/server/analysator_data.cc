#include "analysator_data.h"

int Server::GetData() {
  while (true) {
    int data_socket = CreatingSocket();
    if (data_socket < 0) {
      continue;
    }
    struct sockaddr_in server_address, client_address;
    std::memset(&client_address, 0, sizeof(client_address));
    FillingServer(server_address);
    BindingAndListeningSocket(server_address, data_socket);
    RecivingData(client_address, data_socket);
    CheckingNumber();
    close(data_socket);
  }
}

int Server::CreatingSocket() {
  int data_socket = socket(AF_INET, SOCK_STREAM, 0);
  if (data_socket < 0) {
    std::cout << "Failed to create socket." << std::endl;
    return -1;
  }
  return data_socket;
}

void Server::FillingServer(sockaddr_in &server_address) {
  std::memset(&server_address, 0, sizeof(server_address));
  server_address.sin_family = AF_INET;
  server_address.sin_port = htons(PORT);
  server_address.sin_addr.s_addr = INADDR_ANY;
}

void Server::BindingAndListeningSocket(sockaddr_in &server_address,
                                       int &data_socket) {
  if (bind(data_socket, (struct sockaddr *)&server_address,
           sizeof(server_address)) < 0) {
    std::cout << "Failed to bind socket." << std::endl;
    close(data_socket);
    return;
  }

  if (listen(data_socket, 1) < 0) {
    std::cout << "Failed to listen socket." << std::endl;
    close(data_socket);
    return;
  }
}

void Server::CheckingNumber() {
  if (summary_of_numbers_ > TWO_SYMBOLS &&
      summary_of_numbers_ % MULTIPLE_OF_32 == 0) {
    std::cout << "Summary of Numbers: " << summary_of_numbers_ << std::endl;
  } else {
    std::cout << "Invalid data (number is not a multiple of 32 or less than 3 "
                 "characters)"
              << std::endl;
  }
}

void Server::RecivingData(sockaddr_in &client_address, int &data_socket) {
  socklen_t client_lenght = sizeof(client_address);
  int client_data_socket =
      accept(data_socket, (struct sockaddr *)&client_address, &client_lenght);
  if (client_data_socket < 0) {
    std::cout << "Failed to accept connection." << std::endl;
    return;
  }
  int data = recv(client_data_socket, &summary_of_numbers_,
                  sizeof(summary_of_numbers_), 0);
  if (data != sizeof(summary_of_numbers_)) {
    std::cout << "Data received is corrupted." << std::endl;
    close(client_data_socket);
    return;
  }
  close(client_data_socket);
}
