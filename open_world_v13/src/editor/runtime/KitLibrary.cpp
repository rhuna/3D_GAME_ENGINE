#include "editor/runtime/KitLibrary.h"

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <set>

#include "core/FileSystem.h"
#include "util/StringUtil.h"

namespace fw {
namespace {
std::vector<std::string> ParseTags(const std::string& value) {
    std::vector<std::string> out;
    for (const std::string& part : StringUtil::Split(value, ',')) {
        const std::string trimmed = StringUtil::Trim(part);
        if (!trimmed.empty()) out.push_back(trimmed);
    }
    return out;
}

std::string Stem(const std::string& path) {
    return std::filesystem::path(path).stem().string();
}
}

bool KitLibrary::Reload(const std::string& rootDirectory) {
    m_kits.clear();
    namespace fs = std::filesystem;
    if (!fs::exists(rootDirectory)) {
        return false;
    }

    std::set<std::string> seenNames;
    for (const auto& entry : fs::recursive_directory_iterator(rootDirectory)) {
        if (!entry.is_regular_file()) continue;
        const std::string ext = entry.path().extension().string();
        if (ext != ".kit" && ext != ".scene") continue;

        KitRecord record;
        bool ok = false;
        if (ext == ".kit") {
            ok = LoadKitMetadata(entry.path().string(), record);
        } else {
            InferFromScenePath(entry.path().string(), record);
            ok = true;
        }

        if (!ok || record.name.empty()) continue;
        if (seenNames.insert(record.name).second) {
            if (record.scenePath.empty()) {
                record.scenePath = (entry.path().parent_path() / (record.name + ".scene")).string();
            }
            record.pieceCount = CountScenePieces(record.scenePath);
            m_kits.push_back(record);
        }
    }

    std::sort(m_kits.begin(), m_kits.end(), [](const KitRecord& a, const KitRecord& b) {
        if (a.favorite != b.favorite) return a.favorite > b.favorite;
        if (a.category != b.category) return a.category < b.category;
        return a.displayName < b.displayName;
    });
    return !m_kits.empty();
}

const KitRecord* KitLibrary::FindByName(const std::string& name) const {
    for (const KitRecord& kit : m_kits) {
        if (kit.name == name) return &kit;
    }
    return nullptr;
}

std::vector<const KitRecord*> KitLibrary::BuildView(const std::string& categoryFilter, bool favoritesOnly) const {
    std::vector<const KitRecord*> out;
    for (const KitRecord& kit : m_kits) {
        if (favoritesOnly && !kit.favorite) continue;
        if (!categoryFilter.empty() && categoryFilter != "all" && kit.category != categoryFilter) continue;
        out.push_back(&kit);
    }
    return out;
}

std::vector<std::string> KitLibrary::Categories() const {
    std::set<std::string> categories;
    categories.insert("all");
    for (const KitRecord& kit : m_kits) {
        categories.insert(kit.category.empty() ? "generated" : kit.category);
    }
    return {categories.begin(), categories.end()};
}

bool KitLibrary::LoadKitMetadata(const std::string& kitFilePath, KitRecord& outRecord) const {
    std::ifstream file(kitFilePath);
    if (!file.is_open()) return false;

    outRecord = {};
    outRecord.category = "generated";
    outRecord.id = Stem(kitFilePath);
    std::string line;
    while (std::getline(file, line)) {
        line = StringUtil::Trim(line);
        if (line.empty() || line[0] == '#') continue;
        const std::size_t equals = line.find('=');
        if (equals == std::string::npos) continue;
        const std::string key = StringUtil::Trim(line.substr(0, equals));
        const std::string value = StringUtil::Trim(line.substr(equals + 1));

        if (key == "id") outRecord.id = value;
        else if (key == "name") outRecord.name = value;
        else if (key == "displayName") outRecord.displayName = value;
        else if (key == "category") outRecord.category = value;
        else if (key == "tags") outRecord.tags = ParseTags(value);
        else if (key == "scene") outRecord.scenePath = value;
        else if (key == "favorite") outRecord.favorite = (value == "1" || value == "true" || value == "yes");
    }

    if (outRecord.name.empty()) outRecord.name = Stem(kitFilePath);
    if (outRecord.displayName.empty()) outRecord.displayName = outRecord.name;
    if (outRecord.scenePath.empty()) {
        outRecord.scenePath = std::filesystem::path(kitFilePath).replace_extension(".scene").string();
    }
    return true;
}

void KitLibrary::InferFromScenePath(const std::string& scenePath, KitRecord& outRecord) const {
    outRecord = {};
    outRecord.name = Stem(scenePath);
    outRecord.displayName = outRecord.name;
    outRecord.id = outRecord.name;
    outRecord.category = "generated";
    outRecord.scenePath = scenePath;
}

int KitLibrary::CountScenePieces(const std::string& scenePath) const {
    int count = 0;
    for (const std::string& line : StringUtil::Split(FileSystem::ReadTextFile(scenePath), '\n')) {
        const std::string trimmed = StringUtil::Trim(line);
        if (trimmed.rfind("spawn=", 0) == 0) ++count;
    }
    return count;
}

} // namespace fw
