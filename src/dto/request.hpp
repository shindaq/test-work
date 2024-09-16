#pragma once

#include <nlohmann/json.hpp>

namespace dto {

enum class RequestType {
  kAuthenticate,
  kAddRequisition,
  kTakeBalance,
  kTakeHistory
};

struct Request {
  RequestType type;
  nlohmann::json content;
};

struct AuthContent {
  std::string name;
};

using rtsv = std::pair<RequestType, std::string_view>;

namespace {
constexpr std::array RequestMapping = {
    rtsv{RequestType::kAuthenticate, "authenticate"},
    rtsv{RequestType::kAddRequisition, "addRequisition"},
    rtsv{RequestType::kTakeBalance, "takeBalance"},
    rtsv{RequestType::kTakeHistory, "takeHistory"}};
}
inline RequestType FromString(std::string_view sv) {
  return std::ranges::find_if(
             RequestMapping,
             [&](const auto& mapping) { return mapping.second == sv; })
      ->first;
}

constexpr std::string_view ToString(RequestType type) {
  return std::ranges::find_if(
             RequestMapping,
             [&](const auto& mapping) { return mapping.first == type; })
      ->second;
}
}  // namespace dto