#include "json.hpp"

namespace models {
void from_json(const nlohmann::json& j, Requisition& r) {
  j.at("volume").get_to(r.volume);
  j.at("exchangeRate").get_to(r.exchange_rate);
  std::string ot_str;
  j.at("operationType").get_to(ot_str);
  r.operation_type = FromString(ot_str);
}

void to_json(nlohmann::json& j, const Requisition& r) {
  if (r.volume <= 0) {
    throw exceptions::UserInputException("Volume must be > 0\n");
  }
  if (r.exchange_rate <= 0) {
    throw exceptions::UserInputException("Exchange rate must be > 0\n");
  }
  j = nlohmann::json{{"volume", r.volume},
                     {"exchangeRate", r.exchange_rate},
                     {"operationType", ToString(r.operation_type)}};
}
}  // namespace models

namespace dto {

void from_json(const nlohmann::json& j, Request& r) {
  std::string rt_str;
  j.at("type").get_to(rt_str);
  r.type = FromString(rt_str);
  r.content = j.at("content");
}

void to_json(nlohmann::json& j, const Request& r) {
  j = nlohmann::json{{"type", ToString(r.type)}, {"content", r.content}};
}

void to_json(nlohmann::json& j, const AuthContent& r) {
  j = nlohmann::json{{"name", r.name}};
}
}  // namespace dto