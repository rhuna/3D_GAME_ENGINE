#pragma once

#include <string>
#include <unordered_map>

#include "gameplay/prefabs/PrefabDefinition.h"

namespace fw {

class PrefabLibrary {
public:
    void Clear();
    bool LoadFromDirectory(const std::string& directoryPath);
    void Register(PrefabDefinition definition);
    const PrefabDefinition* Find(const std::string& name) const;

private:
    bool LoadFromFile(const std::string& filePath);
    std::unordered_map<std::string, PrefabDefinition> m_prefabs;
};

} // namespace fw
