#include "logging.hpp"

namespace {
static constexpr std::string_view kInfo = "\033[32mINFO\033[0m";
static constexpr std::string_view kDebug = "DEBUG";
static constexpr std::string_view kWarning = "\033[93mWARNING\033[0m";
static constexpr std::string_view kError = "\033[91mCRITICAL\033[0m";
}  // namespace

void LOG_INFO(std::string_view msg) {
  std::cout << fmt::format("{} {}", kInfo, msg) << '\n';
}
void LOG_DEBUG(std::string_view msg) {
  std::clog << fmt::format("{} {}", kDebug, msg) << '\n';
}
void LOG_WARNING(std::string_view msg) {
  std::cerr << fmt::format("{} {}", kWarning, msg) << '\n';
}
void LOG_ERROR(std::string_view msg) {
  std::cerr << fmt::format("{} {}", kError, msg) << '\n';
}