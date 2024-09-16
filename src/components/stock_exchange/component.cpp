#include "component.hpp"

namespace components {

namespace {
constexpr std::string_view kUserCreationLogPattern =
    "User connected: username = {}, user_id = [{}]";

constexpr std::string_view kRequisitionInsertLogPattern =
    "User [{}] has created a requisition [{}]: volume = {}, exchange_rate = "
    "{}, operation_type = {}";

constexpr std::string_view kDealLogPattern =
    "Deal made: buyer = {}, seller = {}, volume = {}, exchange_rate = "
    "{}";

dto::Requisition FormRequisition(const pqxx::result& result) {
  return {.id = result[0]["id"].as<std::string>(),
          .user_id = result[0]["user_id"].as<std::string>(),
          .volume = result[0]["volume"].as<int>(),
          .exchange_rate = result[0]["exchange_rate"].as<int>()};
}

int GetOperationTypeMultiplier(models::OperationTypes type) {
  if (type == models::OperationTypes::kSell) {
    return 1;
  }
  return -1;
}
}  // namespace

StockClient::StockClient(const std::string& pg_connection)
    : pg_connection_{pg_connection} {}

StockClient::~StockClient() {
  LOG_INFO(fmt::format("User [{}] disconnected", user_id_string_));
}

std::string StockClient::AuthUser(std::string_view name) {
  pqxx::work trx(pg_connection_);
  auto result = trx.exec_params(sql::kSelectUser, name);

  if (result.affected_rows() == 0) {
    result = trx.exec_params(sql::kCreateUser, name);
  }
  trx.commit();
  user_id_string_ = result[0]["id"].as<std::string>();

  LOG_INFO(fmt::format(kUserCreationLogPattern, name, user_id_string_));
  return user_id_string_;
}

std::string StockClient::AddRequisition(
    const models::Requisition& requisition) {
  pqxx::work trx(pg_connection_);
  const auto& operation_type_str = models::ToString(requisition.operation_type);

  auto result = trx.exec_params(std::string(sql::kInsertRequisition),
                                user_id_string_, requisition.volume,
                                requisition.exchange_rate, operation_type_str);
  const auto& requisition_id = result[0]["id"].as<std::string>();

  LOG_DEBUG(fmt::format(kRequisitionInsertLogPattern, user_id_string_,
                        requisition_id, requisition.volume,
                        requisition.exchange_rate, operation_type_str));

  auto match_requisition = [&trx = trx](
                               models::OperationTypes type,
                               std::string_view user_id) -> pqxx::result {
    if (type == models::OperationTypes::kBuy) {
      return trx.exec_params(sql::kSelectSellRequisitions, user_id);
    }
    return trx.exec_params(sql::kSelectBuyRequisitions, user_id);
  };

  result = match_requisition(requisition.operation_type, user_id_string_);
  auto volume = requisition.volume;
  while (result.affected_rows() && volume > 0) {
    const auto& matched_requisition = FormRequisition(result);
    ProceedDeal(trx, volume, requisition, matched_requisition, requisition_id);
    result = match_requisition(requisition.operation_type, user_id_string_);
  }
  trx.commit();
  return requisition_id;
}

void StockClient::ProceedDeal(pqxx::work& trx, int& volume,
                              const models::Requisition& requisition,
                              const dto::Requisition& matched_requisition,
                              std::string_view requisition_id) const {
  auto multiplier = GetOperationTypeMultiplier(requisition.operation_type);
  auto rub_income = 0;
  auto deal_volume = 0;
  if (requisition.volume >= matched_requisition.volume) {
    rub_income = matched_requisition.volume * matched_requisition.exchange_rate;
    trx.exec_params(sql::kUpdateRequisition,
                    pqxx::params{requisition_id, -matched_requisition.volume});
    trx.exec_params(sql::kUpdateRequisitionStatus,
                    pqxx::params{matched_requisition.id});
    deal_volume = matched_requisition.volume;
  } else {
    rub_income = requisition.volume * matched_requisition.exchange_rate;
    trx.exec_params(sql::kUpdateRequisition,
                    pqxx::params{matched_requisition.id, -requisition.volume});
    trx.exec_params(sql::kUpdateRequisitionStatus,
                    pqxx::params{requisition_id});
    deal_volume = requisition.volume;
  }
  trx.exec_params(
      sql::kUpdateUserBalance,
      pqxx::params{user_id_string_, -matched_requisition.volume * multiplier,
                   rub_income * multiplier});
  trx.exec_params(sql::kUpdateUserBalance,
                  pqxx::params{matched_requisition.user_id,
                               matched_requisition.volume * multiplier,
                               -rub_income * multiplier});
  volume -= matched_requisition.volume;

  std::string buyer_id = "";
  std::string seller_id = "";

  if (requisition.operation_type == models::OperationTypes::kBuy) {
    buyer_id = user_id_string_;
    seller_id = matched_requisition.user_id;
  } else {
    buyer_id = matched_requisition.user_id;
    seller_id = user_id_string_;
  }
  trx.exec_params(sql::kInsertDeal, buyer_id, seller_id, deal_volume,
                  matched_requisition.exchange_rate);
  LOG_DEBUG(fmt::format(kDealLogPattern, buyer_id, seller_id, deal_volume,
                        matched_requisition.exchange_rate));
}

dto::UserBalance StockClient::TakeUserInfo() {
  pqxx::work trx(pg_connection_);
  auto result = trx.exec_params1(sql::kTakeUser, pqxx::params{user_id_string_});
  trx.commit();
  return {.user_id = result["id"].as<std::string>(),
          .usd = result["usd_balance"].as<int>(),
          .rub = result["rub_balance"].as<int>()};
}

std::vector<models::Deal> StockClient::TakeDeals() {
  std::vector<models::Deal> result;
  pqxx::work trx(pg_connection_);
  auto deals = trx.exec_params(sql::kTakeDeals, user_id_string_);
  result.reserve(deals.size());
  std::ranges::transform(deals, std::back_inserter(result), [](auto&& row) {
    return models::Deal{
        .id = row["id"].template as<std::string>(),
        .created_at = row["created_at"].template as<std::string>(),
        .buyer_id = row["buyer_id"].template as<std::string>(),
        .seller_id = row["seller_id"].template as<std::string>(),
        .volume = row["volume"].template as<int>(),
        .exchange_rate = row["exchange_rate"].template as<int>()};
  });
  return result;
}

std::string_view StockClient::TakeId() const { return user_id_string_; }
}  // namespace components