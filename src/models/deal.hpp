#pragma once

#include <fmt/core.h>
#include <string>

namespace models {
struct Deal {
  std::string id;
  std::string created_at;
  std::string buyer_id;
  std::string seller_id;
  int volume;
  int exchange_rate;
};

namespace {
constexpr std::string_view kBuyDealFmtPattern =
    "Deal id: [{}], Date: {}, Deal type: Buy, Seller: [{}], Volume: {}, "
    "Exchange rate: {}\n";
constexpr std::string_view kSellDealFmtPattern =
    "Deal id: [{}], Date: {}, Deal type: Sell, Buyer: [{}], Volume: {}, "
    "Exchange rate: {}\n";
}  // namespace

inline std::string ToString(const Deal& deal, std::string_view user_id) {
  if (deal.buyer_id == user_id) {
    return fmt::format(kBuyDealFmtPattern, deal.id, deal.created_at,
                       deal.seller_id, deal.volume, deal.exchange_rate);
  }
  return fmt::format(kSellDealFmtPattern, deal.id, deal.created_at,
                     deal.buyer_id, deal.volume, deal.exchange_rate);
}
}  // namespace models