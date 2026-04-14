#include "core/FileSystem.h"

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <sstream>

namespace fs = std::filesystem;

namespace fw {

namespace {
bool MatchesExtension(const fs::path& path, const std::vector<std::string>& extensions) {
    if (extensions.empty()) return true;
    std::string ext = path.extension().string();
    std::transform(ext.begin(), ext.end(), ext.begin(), [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
    for (std::string candidate : extensions) {
        std::transform(candidate.begin(), candidate.end(), candidate.begin(), [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
        if (!candidate.empty() && candidate.front() != '.') candidate = "." + candidate;
        if (ext == candidate) return true;
    }
    return false;
}
}

bool FileSystem::Exists(const std::string& path) {
    return fs::exists(fs::path(path));
}

std::string FileSystem::ReadTextFile(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) return {};
    std::ostringstream stream;
    stream << file.rdbuf();
    return stream.str();
}

bool FileSystem::EnsureDirectory(const std::string& path) {
    std::error_code ec;
    if (path.empty()) return false;
    if (fs::exists(path)) return fs::is_directory(path);
    return fs::create_directories(path, ec) || fs::exists(path);
}

bool FileSystem::EnsureParentDirectory(const std::string& filePath) {
    const fs::path path(filePath);
    if (!path.has_parent_path()) return true;
    return EnsureDirectory(path.parent_path().string());
}

bool FileSystem::WriteTextFile(const std::string& path, const std::string& content) {
    if (!EnsureParentDirectory(path)) return false;
    std::ofstream file(path, std::ios::binary);
    if (!file.is_open()) return false;
    file << content;
    return file.good();
}

std::vector<std::string> FileSystem::ListFilesRecursive(const std::string& root) {
    return ListFilesRecursive(root, {});
}

std::vector<std::string> FileSystem::ListFilesRecursive(const std::string& root, const std::vector<std::string>& extensions) {
    std::vector<std::string> files;
    std::error_code ec;
    if (!fs::exists(root)) return files;
    for (fs::recursive_directory_iterator it(root, ec), end; it != end; it.increment(ec)) {
        if (ec) break;
        if (!it->is_regular_file()) continue;
        if (!MatchesExtension(it->path(), extensions)) continue;
        files.push_back(it->path().string());
    }
    std::sort(files.begin(), files.end());
    return files;
}

} // namespace fw
