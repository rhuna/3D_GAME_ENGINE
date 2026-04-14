#include "Logger.h"
#include <iostream>

namespace fw
{

    static const char *ToString(const std::string &level)
    {
        return level.c_str();
    }

    static void Write(const std::string &level, const std::string &category, const std::string &message)
    {
        std::cout << "[" << level << "][" << category << "] " << message << std::endl;
    }

    // --- Debug ---
    void Logger::Debug(const std::string &message)
    {
        Write("DEBUG", "GENERAL", message);
    }

    void Logger::Debug(const std::string &category, const std::string &message)
    {
        Write("DEBUG", category, message);
    }

    // --- Info ---
    void Logger::Info(const std::string &message)
    {
        Write("INFO", "GENERAL", message);
    }

    void Logger::Info(const std::string &category, const std::string &message)
    {
        Write("INFO", category, message);
    }

    // --- Warn ---
    void Logger::Warn(const std::string &message)
    {
        Write("WARN", "GENERAL", message);
    }

    void Logger::Warn(const std::string &category, const std::string &message)
    {
        Write("WARN", category, message);
    }

    // --- Error ---
    void Logger::Error(const std::string &message)
    {
        Write("ERROR", "GENERAL", message);
    }

    void Logger::Error(const std::string &category, const std::string &message)
    {
        Write("ERROR", category, message);
    }

}