#pragma once

#include <string>
#include <unordered_map>

namespace fw {

enum class AssetType {
    Unknown,
    Texture,
    Model,
    Shader
};

struct AssetRecord {
    std::string id;
    AssetType type = AssetType::Unknown;
    std::string path;
};

class AssetRegistry {
public:
    bool LoadFromFile(const std::string& filePath);
    void Clear();

    const AssetRecord* Find(const std::string& id) const;
    bool Has(const std::string& id) const;
    const std::unordered_map<std::string, AssetRecord>& Records() const { return m_records; }

private:
    std::unordered_map<std::string, AssetRecord> m_records;
};

} // namespace fw
