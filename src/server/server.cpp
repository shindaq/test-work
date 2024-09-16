#include "server.hpp"

namespace {
constexpr std::string_view kStartInfoPattern = "Server started! Listen {} port";
}  // namespace

Server::Server(asio::io_service& io_service)
    : io_service_(io_service),
      acceptor_(io_service, tcp::endpoint{tcp::v4(), configs::Port}) {
  LOG_INFO(fmt::format(kStartInfoPattern, 5555));
  boost::shared_ptr<components::Session> new_session(
      new components::Session(io_service_));
  acceptor_.async_accept(new_session->Socket(),
                         boost::bind(&Server::HandleAccept, this, new_session,
                                     asio::placeholders::error));
}

void Server::HandleAccept(boost::shared_ptr<components::Session> new_session,
                          const boost::system::error_code& error) {
  if (!error) {
    new_session->Start();
    boost::shared_ptr<components::Session> another_session(
        new components::Session(io_service_));
    acceptor_.async_accept(
        another_session->Socket(),
        boost::bind(&Server::HandleAccept, this, another_session,
                    boost::asio::placeholders::error));
  } else {
    new_session->Socket().close();
    new_session.reset();
  }
}
