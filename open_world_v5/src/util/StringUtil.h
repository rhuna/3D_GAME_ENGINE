#pragma once

#include <string>
#include <string_view>
#include <vector>

namespace fw::StringUtil {

std::string Trim(std::string_view value);
std::vector<std::string> Split(std::string_view value, char delimiter);

} // namespace fw::StringUtil
