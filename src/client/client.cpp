#include "client.hpp"

namespace {

enum class MenuOptions { kAddRequisition, kCheckBalance, kCheckHistory, kExit };

constexpr std::string_view kMenu =
    "Menu:\n"
    "1) Add requisition\n"
    "2) Check balance\n"
    "3) Check deals history\n"
    "4) Exit\n\n";
}  // namespace

Client::Client(asio::io_service& io_service,
               boost::asio::ip::tcp::socket& socket)
    : io_service_(io_service), socket_(socket) {}

void Client::SendMessage(const std::string& message) {
  asio::write(socket_, asio::buffer(message, message.size()));
}

std::string Client::ReadMessage() {
  asio::streambuf sbuf;
  asio::read_until(socket_, sbuf, "\0");
  std::istream is(&sbuf);
   std::string line(std::istreambuf_iterator<char>(is), {});
  return line;
}

void Client::ProcessRequest(const dto::Request& request) {
  nlohmann::json request_json = request;
  std::string json_string = request_json.dump();
  SendMessage(json_string);
  std::cout << ReadMessage();
}

void Client::ProcessRegistration() {
  std::string user_name;
  std::cout << "Hello! Enter your name: ";
  std::cin >> user_name;
  nlohmann::json content = user_name;
  dto::Request request{.type = dto::RequestType::kAuthenticate,
                       .content = content};
  ProcessRequest(request);
}

void Client::ProcessAddRequisition() {
  int volume, exchange_rate, op_type_num;
  std::cout << "Enter volume ";
  std::cin >> volume;
  std::cout << "Enter exchange rate ";
  std::cin >> exchange_rate;
  std::cout << "Enter operation type (buy - 0, sell - 1) ";
  std::cin >> op_type_num;
  if (op_type_num < 0 && op_type_num > 1) {
    throw exceptions::UserInputException("Operation type must be 0 or 1");
  }
  models::Requisition requisition = {
      .volume = volume,
      .exchange_rate = exchange_rate,
      .operation_type = models::OperationTypes{op_type_num}};
  nlohmann::json content = requisition;
  dto::Request request{.type = dto::RequestType::kAddRequisition,
                       .content = content};
  ProcessRequest(request);
}

void Client::ProcessTakeBalance() {
  dto::Request request{.type = dto::RequestType::kTakeBalance,
                       .content = nlohmann::json{}};
  nlohmann::json request_json = request;
  ProcessRequest(request);
}

void Client::ProcessTakeDealHistory() {
  dto::Request request{.type = dto::RequestType::kTakeHistory,
                       .content = nlohmann::json{}};
  nlohmann::json request_json = request;
  ProcessRequest(request);
}

void Client::Start() {
  ProcessRegistration();
  while (true) {
    try {
      std::cout << kMenu;
      int menu_option_num;
      std::cin >> menu_option_num;
      if (menu_option_num < 1 || menu_option_num >> 4) {
        std::cerr << "Invalid operation\n";
      }
      MenuOptions option = static_cast<MenuOptions>(menu_option_num-1);
      switch (option) {
        case MenuOptions::kAddRequisition: {
          ProcessAddRequisition();
          break;
        }
        case MenuOptions::kCheckBalance: {
          ProcessTakeBalance();
          break;
        }
        case MenuOptions::kCheckHistory: {
          ProcessTakeDealHistory();
          break;
        }
        case MenuOptions::kExit: {
          exit(0);
        }
      }
    } catch (const std::exception& ex) {
      std::cerr << ex.what();
    }
  }
}