#pragma once

#include <string>
#include <vector>

namespace fw {

class FileSystem {
public:
    static bool Exists(const std::string& path);
    static std::string ReadTextFile(const std::string& path);
    static bool EnsureDirectory(const std::string& path);
    static bool WriteTextFile(const std::string& path, const std::string& content);
    static std::vector<std::string> ListFilesRecursive(const std::string& root, const std::vector<std::string>& extensions = {});
};

} // namespace fw
