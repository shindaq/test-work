#pragma once

#include <string>

namespace dto {
struct UserBalance {
  std::string user_id;
  int usd;
  int rub;
};
}  // namespace dto