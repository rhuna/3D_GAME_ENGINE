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

bool FileSystem::EnsureDirectory(const std::string& path) {
    std::error_code ec;
    if (path.empty()) return false;
    return std::filesystem::create_directories(path, ec) || std::filesystem::exists(path);
}

bool FileSystem::WriteTextFile(const std::string& path, const std::string& content) {
    const std::filesystem::path fsPath(path);
    if (fsPath.has_parent_path()) {
        EnsureDirectory(fsPath.parent_path().string());
    }

    std::ofstream file(path, std::ios::binary | std::ios::trunc);
    if (!file.is_open()) {
        return false;
    }
    file << content;
    return file.good();
}

std::vector<std::string> FileSystem::ListFilesRecursive(const std::string& root, const std::vector<std::string>& extensions) {
    std::vector<std::string> results;
    namespace fs = std::filesystem;
    if (!fs::exists(root)) return results;

    for (const auto& entry : fs::recursive_directory_iterator(root)) {
        if (!entry.is_regular_file()) continue;
        if (!extensions.empty()) {
            const std::string ext = entry.path().extension().string();
            bool matches = false;
            for (const std::string& allowed : extensions) {
                if (ext == allowed) {
                    matches = true;
                    break;
                }
            }
            if (!matches) continue;
        }
        results.push_back(entry.path().string());
    }
    return results;
}

} // namespace fw
