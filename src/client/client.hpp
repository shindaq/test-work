#pragma once
#include <boost/asio.hpp>
#include <boost/asio/io_service.hpp>
#include <iostream>
#include "dto/request.hpp"
#include "exceptions/client_exceptions.hpp"
#include "utils/json.hpp"

namespace asio = boost::asio;

class Client final {
 public:
  Client(asio::io_service& io_service, asio::ip::tcp::socket& socket);
  void Start();

 private:
  void SendMessage(const std::string& message);
  std::string ReadMessage();
  void ProcessRegistration();
  void ProcessAddRequisition();
  void ProcessTakeBalance();
  void ProcessTakeDealHistory();
  void ProcessRequest(const dto::Request& request);

 private:
  asio::io_service& io_service_;
  asio::ip::tcp::socket& socket_;
};
