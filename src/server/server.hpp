#pragma once

#include <boost/algorithm/string.hpp>
#include <boost/asio.hpp>
#include <boost/asio/awaitable.hpp>
#include <boost/asio/co_spawn.hpp>
#include <boost/asio/detached.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/asio/io_service.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/bind/bind.hpp>
#include <boost/smart_ptr/shared_ptr.hpp>
#include "components/session/component.hpp"
#include "components/stock_exchange/component.hpp"
#include "models/requisition.hpp"
#include "utils/logging.hpp"

namespace asio = boost::asio;
using tcp = boost::asio::ip::tcp;
class Server final {
 public:
  Server(asio::io_service& io_service);
  void HandleAccept(boost::shared_ptr<components::Session> new_session,
                    const boost::system::error_code& error);
 private:
  asio::io_service& io_service_;
  tcp::acceptor acceptor_;
};
