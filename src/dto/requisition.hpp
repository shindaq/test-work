#pragma once

#include <string>

namespace dto {
struct Requisition {
  std::string id;
  std::string user_id;
  int volume;
  int exchange_rate;
};
}  // namespace dto