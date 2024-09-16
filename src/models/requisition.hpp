#pragma once

#include <array>
#include <nlohmann/json.hpp>
#include <string_view>
#include <utility>

namespace models {

enum class OperationTypes { kBuy, kSell };

struct Requisition {
  int volume;
  int exchange_rate;
  OperationTypes operation_type;
};

using ttsv = std::pair<OperationTypes, std::string_view>;

namespace {
constexpr std::array OperationMapping = {ttsv{OperationTypes::kBuy, "buy"},
                                         ttsv{OperationTypes::kSell, "sell"}};
}

inline OperationTypes FromString(std::string_view sv) {
  return std::ranges::find_if(
             OperationMapping,
             [&](const auto& mapping) { return mapping.second == sv; })
      ->first;
}

inline std::string_view ToString(OperationTypes type) {
  return std::ranges::find_if(
             OperationMapping,
             [&](const auto& mapping) { return mapping.first == type; })
      ->second;
}
};  // namespace models
