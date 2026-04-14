#pragma once

#include <string>
#include <unordered_map>
#include <unordered_set>

#include "gameplay/prefabs/PrefabDefinition.h"
#include "gameplay/prefabs/PrefabVariantDefinition.h"

namespace fw {

class PrefabLibrary {
public:
    void Clear();

    bool LoadFromDirectory(const std::string& directoryPath);
    bool LoadVariantsFromDirectory(const std::string& directoryPath);
    bool AppendFromDirectory(const std::string& directoryPath);
    bool AppendVariantsFromDirectory(const std::string& directoryPath);

    void Register(PrefabDefinition definition);
    void RegisterVariant(PrefabVariantDefinition definition);

    const PrefabDefinition* Find(const std::string& name) const;
    const PrefabVariantDefinition* FindVariant(const std::string& name) const;

    bool BuildPrefabFromVariant(const std::string& variantName, PrefabDefinition& outPrefab) const;

    const std::unordered_map<std::string, PrefabDefinition>& Prefabs() const { return m_prefabs; }
    const std::unordered_map<std::string, PrefabVariantDefinition>& Variants() const { return m_variants; }
    std::size_t PrefabCount() const { return m_prefabs.size(); }
    std::size_t VariantCount() const { return m_variants.size(); }

private:
    bool LoadPrefabFile(const std::string& filePath);
    bool LoadVariantFile(const std::string& filePath);
    bool BuildPrefabFromVariantRecursive(const std::string& variantName, PrefabDefinition& outPrefab, std::unordered_set<std::string>& stack) const;

    std::unordered_map<std::string, PrefabDefinition> m_prefabs;
    std::unordered_map<std::string, PrefabVariantDefinition> m_variants;
};

} // namespace fw
