#include "assets/AssetRegistry.h"

#include <fstream>

#include "core/Logger.h"
#include "util/StringUtil.h"

namespace fw {
namespace {

AssetType ParseAssetType(const std::string& value) {
    if (value == "texture") return AssetType::Texture;
    if (value == "model") return AssetType::Model;
    if (value == "shader") return AssetType::Shader;
    return AssetType::Unknown;
}

} // namespace

bool AssetRegistry::LoadFromFile(const std::string& filePath) {
    Clear();

    std::ifstream file(filePath);
    if (!file.is_open()) {
        Logger::Warn("Failed to open asset registry: " + filePath);
        return false;
    }

    std::string line;
    int lineNumber = 0;
    while (std::getline(file, line)) {
        ++lineNumber;
        line = StringUtil::Trim(line);
        if (line.empty() || line[0] == '#') {
            continue;
        }

        const std::size_t equals = line.find('=');
        if (equals == std::string::npos) {
            Logger::Warn("Invalid asset registry line " + std::to_string(lineNumber) + ": " + line);
            continue;
        }

        const std::string key = StringUtil::Trim(line.substr(0, equals));
        if (key != "asset") {
            Logger::Warn("Unknown asset registry key on line " + std::to_string(lineNumber) + ": " + key);
            continue;
        }

        AssetRecord record;
        const auto tokens = StringUtil::Split(line.substr(equals + 1), ';');
        for (const std::string& rawToken : tokens) {
            const std::string token = StringUtil::Trim(rawToken);
            if (token.empty()) continue;

            const std::size_t tokenEquals = token.find('=');
            if (tokenEquals == std::string::npos) {
                if (record.id.empty()) {
                    record.id = token;
                }
                continue;
            }

            const std::string tokenKey = StringUtil::Trim(token.substr(0, tokenEquals));
            const std::string tokenValue = StringUtil::Trim(token.substr(tokenEquals + 1));
            if (tokenKey == "id") {
                record.id = tokenValue;
            } else if (tokenKey == "type") {
                record.type = ParseAssetType(tokenValue);
            } else if (tokenKey == "path") {
                record.path = tokenValue;
            }
        }

        if (record.id.empty() || record.path.empty()) {
            Logger::Warn("Incomplete asset registry entry on line " + std::to_string(lineNumber) + ": " + line);
            continue;
        }

        if (record.type == AssetType::Unknown) {
            Logger::Warn("Asset registry entry has unknown type on line " + std::to_string(lineNumber) + ": " + line);
            continue;
        }

        if (m_records.contains(record.id)) {
            Logger::Warn("Duplicate asset id in registry, keeping latest: " + record.id);
        }
        m_records[record.id] = std::move(record);
    }

    Logger::Info("Loaded asset registry entry count: " + std::to_string(m_records.size()));
    return !m_records.empty();
}

void AssetRegistry::Clear() {
    m_records.clear();
}

const AssetRecord* AssetRegistry::Find(const std::string& id) const {
    const auto it = m_records.find(id);
    return it != m_records.end() ? &it->second : nullptr;
}

bool AssetRegistry::Has(const std::string& id) const {
    return m_records.contains(id);
}

} // namespace fw
