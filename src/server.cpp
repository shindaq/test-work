#include "server/server.hpp"

int main() {
  try {
    boost::asio::io_service io_service;
    Server server(io_service);
    io_service.run();
  } catch (std::exception& ex) {
    LOG_ERROR(ex.what());
  }
  return 0;
}