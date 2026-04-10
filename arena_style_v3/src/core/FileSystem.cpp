#include "core/FileSystem.h"

#include <filesystem>
#include <fstream>
#include <sstream>

namespace fw {

bool FileSystem::Exists(const std::string& path) {
    return std::filesystem::exists(path);
}

std::string FileSystem::ReadTextFile(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        return {};
    }

    std::ostringstream stream;
    stream << file.rdbuf();
    return stream.str();
}

} // namespace fw
