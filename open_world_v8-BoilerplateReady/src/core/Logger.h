#pragma once

#include <string>

namespace fw {

class Logger {
public:
    static void Info(const std::string& message);
    static void Warn(const std::string& message);
    static void Error(const std::string& message);

private:
    static void Print(const char* level, const std::string& message);
};

} // namespace fw
