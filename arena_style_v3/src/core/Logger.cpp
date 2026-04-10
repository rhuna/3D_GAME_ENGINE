#include "core/Logger.h"

#include <iostream>

namespace fw {

void Logger::Info(const std::string& message) {
    Print("INFO", message);
}

void Logger::Warn(const std::string& message) {
    Print("WARN", message);
}

void Logger::Error(const std::string& message) {
    Print("ERROR", message);
}

void Logger::Print(const char* level, const std::string& message) {
    std::cout << "[" << level << "] " << message << '\n';
}

} // namespace fw
