#include "component.hpp"

namespace components {

namespace {
constexpr std::string_view kUserAuthResponsePattern =
    "You've succesfully authenticated, your id: [{}]\n";

constexpr std::string_view kAddRequisitionResponsePattern =
    "Your requisition was successfully uploaded, it's id: [{}]\n";
constexpr std::string_view kTakeBalanceResponsePattern =
    "Your balance:\nRUB: {}\nUSD: {}\n";
}  // namespace

Session::Session(asio::io_service& io_service)
    : socket_(io_service),
      stock_client_("postgres://postgres:postgres@localhost:5432") {}

asio::ip::tcp::socket& Session::Socket() { return socket_; }

std::string Session::HandleResponse(const dto::Request& request) {
  switch (request.type) {
    case dto::RequestType::kAuthenticate: {
      const auto& name = request.content.template get<std::string>();
      const auto& user_id = stock_client_.AuthUser(name);
      return fmt::format(kUserAuthResponsePattern, user_id);
    }
    case dto::RequestType::kAddRequisition: {
      const auto& requisition =
          request.content.template get<models::Requisition>();
      const auto& requisition_id = stock_client_.AddRequisition(requisition);
      return fmt::format(kAddRequisitionResponsePattern, requisition_id);
    }
    case dto::RequestType::kTakeBalance: {
      const auto& user = stock_client_.TakeUserInfo();
      return fmt::format(kTakeBalanceResponsePattern, user.rub, user.usd);
    }
    case dto::RequestType::kTakeHistory: {
      const auto& deals = stock_client_.TakeDeals();
      std::string result = "";
      auto user_id = stock_client_.TakeId();
      for (const auto& deal : deals) {
        result += models::ToString(deal, user_id);
      }
      return result;
    }
  }
}

void Session::Start() {
  socket_.async_read_some(asio::buffer(data_, max_length),
                          boost::bind(&Session::HandleRead, shared_from_this(),
                                      asio::placeholders::error,
                                      asio::placeholders::bytes_transferred));
}

void Session::HandleRead(const boost::system::error_code& error,
                         std::size_t bytes_transferred) {
  if (!error) {
    data_[bytes_transferred] = '\0';

    const auto& json = nlohmann::json::parse(data_);
    const auto& request = json.template get<dto::Request>();
    const auto& response = HandleResponse(request);
    boost::asio::async_write(
        socket_, asio::buffer(response, response.size()),
        boost::bind(&Session::HandleWrite, shared_from_this(),
                    asio::placeholders::error()));
  } else {
    socket_.close();
    shared_from_this().reset();
  }
}

void Session::HandleWrite(const boost::system::error_code& error) {
  if (!error) {
    socket_.async_read_some(
        asio::buffer(data_, max_length),
        boost::bind(&Session::HandleRead, shared_from_this(),
                    asio::placeholders::error,
                    asio::placeholders::bytes_transferred));
  } else {
    socket_.close();
    shared_from_this().reset();
  }
}
}  // namespace components