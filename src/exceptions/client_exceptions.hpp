#pragma once

#include <fmt/core.h>
#include <stdexcept>

namespace exceptions {
class UserInputException final : public std::runtime_error {
 public:
  UserInputException(const std::string& what)
      : std::runtime_error(fmt::format("Invalid input exception: {}", what)) {}
};
}  // namespace exceptions