#include "client/client.hpp"
#include "configs/config.hpp"

int main() {
  try {
    boost::asio::io_service io_service;
    boost::asio::ip::tcp::resolver resolver(io_service);
    boost::asio::ip::tcp::resolver::query query(
        boost::asio::ip::tcp::v4(), "127.0.0.1", std::to_string(configs::Port));
    boost::asio::ip::tcp::resolver::iterator iterator = resolver.resolve(query);
    boost::asio::ip::tcp::socket socket(io_service);
    socket.connect(*iterator);
    Client client(io_service, socket);
    client.Start();
  } catch (const std::exception& ex) {
    std::cerr << ex.what();
  }
  return 0;
}