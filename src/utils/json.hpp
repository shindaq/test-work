#pragma once

#include <nlohmann/json.hpp>
#include "dto/request.hpp"
#include "models/requisition.hpp"
#include "exceptions/client_exceptions.hpp"

namespace models {
void from_json(const nlohmann::json& j, Requisition& r);
void to_json(nlohmann::json& j, const Requisition& r);
}  // namespace models

namespace dto {
void from_json(const nlohmann::json& j, Request& r);
void to_json(nlohmann::json& j, const Request& r);
void to_json(nlohmann::json& j, const AuthContent& a);
}  // namespace dto
