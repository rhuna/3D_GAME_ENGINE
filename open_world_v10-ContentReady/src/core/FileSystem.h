#pragma once

#include <string>

namespace fw {

class FileSystem {
public:
    static bool Exists(const std::string& path);
    static std::string ReadTextFile(const std::string& path);
};

} // namespace fw
