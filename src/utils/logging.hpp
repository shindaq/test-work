#pragma once

#include <string_view>
#include <iostream>
#include <fmt/format.h>

void LOG_INFO(std::string_view msg);
void LOG_DEBUG(std::string_view msg);
void LOG_WARNING(std::string_view msg);
void LOG_ERROR(std::string_view msg);
