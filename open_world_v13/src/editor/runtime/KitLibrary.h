#pragma once

#include <string>
#include <vector>

namespace fw {

struct KitRecord {
    std::string id;
    std::string name;
    std::string displayName;
    std::string category = "generated";
    std::vector<std::string> tags;
    std::string scenePath;
    bool favorite = false;
    int pieceCount = 0;
};

class KitLibrary {
public:
    bool Reload(const std::string& rootDirectory = "assets/scenes/generated");
    const KitRecord* FindByName(const std::string& name) const;
    std::vector<const KitRecord*> BuildView(const std::string& categoryFilter, bool favoritesOnly) const;
    std::vector<std::string> Categories() const;
    const std::vector<KitRecord>& Kits() const { return m_kits; }

private:
    bool LoadKitMetadata(const std::string& kitFilePath, KitRecord& outRecord) const;
    void InferFromScenePath(const std::string& scenePath, KitRecord& outRecord) const;
    int CountScenePieces(const std::string& scenePath) const;

    std::vector<KitRecord> m_kits;
};

} // namespace fw
