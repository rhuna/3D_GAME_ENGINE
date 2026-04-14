#pragma once
#include <string>

namespace fw
{

    class Logger
    {
    public:
        static void Debug(const std::string &message);
        static void Debug(const std::string &category, const std::string &message);

        static void Info(const std::string &message);
        static void Info(const std::string &category, const std::string &message);

        static void Warn(const std::string &message);
        static void Warn(const std::string &category, const std::string &message);

        static void Error(const std::string &message);
        static void Error(const std::string &category, const std::string &message);
    };

}