#include "client.h"

Client::Client()
    : summary_of_numbers_(0), buffer_ready_(false), stop_processing_(false) {}

Client::~Client() { StopProcessing(); }

void Client::Input(std::istream &istream) {
  std::string input_line;
  while (!stop_processing_) {
    std::getline(istream, input_line);
    if (!ValidationString(input_line)) {
      {
        std::lock_guard<std::mutex> output_lock(output_mutex_);
        output_.push(
            "Erorr, please enter a string of only numbers and no more than 64 "
            "characters.");
      }
      continue;
    }
    SortAndReplaceElements(input_line);
    std::unique_lock<std::mutex> buffer_lock(buffer_mutex_);
    buffer_value_ = input_line;
    buffer_ready_ = true;
    buffer_lock.unlock();
    buffer_condition_value_.notify_one();
    {
      std::lock_guard<std::mutex> output_lock(output_mutex_);
      output_.push("Buffer value: " + buffer_value_);
    }
  }
}

void Client::Handling() {
  struct sockaddr_in server_address;
  while (!stop_processing_) {
    summary_of_numbers_ = 0;
    std::unique_lock<std::mutex> buffer_lock(buffer_mutex_);
    buffer_condition_value_.wait(
        buffer_lock, [this] { return buffer_ready_ || stop_processing_; });

    buffer_ready_ = false;
    std::string data_thread = buffer_value_;
    buffer_lock.unlock();

    for (char number : data_thread) {
      if (number >= '0' && number <= '9') {
        summary_of_numbers_ += number - '0';
      }
    }
    {
      std::lock_guard<std::mutex> output_lock(output_mutex_);
      output_.push("The total sum of all elements that are numerical values: " +
                   std::to_string(summary_of_numbers_));
    }
    SendData(summary_of_numbers_, server_address);
    buffer_value_.clear();
  }
}

void Client::Output() {
  while (!stop_processing_) {
    std::lock_guard<std::mutex> buffer_lock(output_mutex_);
    while (!output_.empty()) {
      std::cout << output_.front() << std::endl;
      output_.pop();
    }
  }
}

bool Client::ValidationString(std::string input_line) {
  if (input_line.find_first_not_of("0123456789") != input_line.npos) {
    return false;
  }
  if (input_line.length() > kMaxStringLength) {
    return false;
  }
  return true;
}

void Client::SortAndReplaceElements(std::string &input_line) {
  std::sort(input_line.rbegin(), input_line.rend());
  for (size_t i = 0; i < input_line.size();) {
    if (input_line[i] % 2 == 0) {
      input_line[i] = 'B';
      input_line.insert(input_line.begin() + i, 'K');
      i += 2;
    } else {
      ++i;
    }
  }
}

void Client::SendData(int &summary_of_numbers, sockaddr_in &server_address) {
  int data_socket = CreateSocket();
  if (data_socket < 0) {
    return;
  }
  FillServer(server_address);
  if (connect(data_socket, (struct sockaddr *)&server_address,
              sizeof(server_address)) < 0) {
    {
      std::lock_guard<std::mutex> output_lock(output_mutex_);
      output_.push("Failed connect to server.");
    }
    close(data_socket);
    return;
  }
  send(data_socket, &summary_of_numbers, sizeof(summary_of_numbers), 0);
  close(data_socket);
}

int Client::CreateSocket() {
  int data_socket = socket(AF_INET, SOCK_STREAM, 0);
  if (data_socket < 0) {
    {
      std::lock_guard<std::mutex> output_lock(output_mutex_);
      output_.push("Failed to create socket.");
    }
    return -1;
  }
  return data_socket;
}

void Client::FillServer(sockaddr_in &server_address) {
  std::memset(&server_address, 0, sizeof(server_address));
  server_address.sin_family = AF_INET;
  server_address.sin_port = htons(kPort);
  inet_pton(AF_INET, "127.0.0.1", &server_address.sin_addr);
}

std::string Client::GetBufferValue() { return buffer_value_; }

int Client::GetSummaryOfNumbers() { return summary_of_numbers_; }

void Client::SetBufferValue(std::string buffer) { buffer_value_ = buffer; }

void Client::StartProcessing() {
  std::thread first_thread([this]() { Client::Input(std::ref(std::cin)); });
  std::thread second_thread(&Client::Handling, this);
  std::thread third_thread(&Client::Output, this);
  first_thread.join();
  second_thread.join();
  third_thread.join();
}

void Client::StopProcessing() {
  stop_processing_ = true;
  buffer_condition_value_.notify_all();
}
