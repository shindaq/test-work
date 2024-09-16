#pragma once

#include <fmt/format.h>
#include <pqxx/pqxx>
#include <sql/query.hpp>
#include <string_view>
#include "dto/requisition.hpp"
#include "dto/user_balance.hpp"
#include "models/deal.hpp"
#include "models/requisition.hpp"
#include "utils/logging.hpp"
#include "configs/config.hpp"

namespace components {
class StockClient final {
 public:
  StockClient();
  std::string AuthUser(std::string_view name);
  std::string AddRequisition(const models::Requisition& requisition);
  dto::UserBalance TakeUserInfo();
  std::vector<models::Deal> TakeDeals();
  std::string_view TakeId() const;
  ~StockClient();

 private:
  void ProceedDeal(pqxx::work& trx, int& volume,
                   const models::Requisition& requisition,
                   const dto::Requisition& matched_requisition,
                   std::string_view requisition_id) const;

 private:
  pqxx::connection pg_connection_;
  std::string user_id_string_;
};
};  // namespace components