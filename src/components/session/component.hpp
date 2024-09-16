#pragma once

#include <boost/asio.hpp>
#include <boost/asio/io_service.hpp>
#include <boost/bind/bind.hpp>
#include <boost/smart_ptr/enable_shared_from_this.hpp>
#include <boost/system/error_code.hpp>
#include <cstddef>
#include <nlohmann/json.hpp>
#include "components/stock_exchange/component.hpp"
#include "dto/request.hpp"
#include "utils/json.hpp"

namespace components {
namespace asio = boost::asio;
class Session : public boost::enable_shared_from_this<Session> {
 public:
  Session(asio::io_service& io_service);

  asio::ip::tcp::socket& Socket();

  void Start();

  void HandleRead(const boost::system::error_code& error,
                  std::size_t bytes_transferred);

  void HandleWrite(const boost::system::error_code& error);

 private:
  std::string HandleResponse(const dto::Request& request);
  asio::ip::tcp::socket socket_;
  static const int max_length = 1024*1024;
  char data_[max_length];
  components::StockClient stock_client_;
};
}  // namespace components