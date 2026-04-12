#pragma once

#include <string>
#include <unordered_map>

#include "scene/data/SceneDefinition.h"

namespace fw {

class SceneLibrary {
public:
    void Clear();
    bool LoadFromDirectory(const std::string& directoryPath);
    void Register(SceneDefinition definition);
    const SceneDefinition* Find(const std::string& name) const;
    bool LoadSceneFile(const std::string& filePath);
    const std::unordered_map<std::string, SceneDefinition>& Scenes() const { return m_scenes; }
    std::size_t SceneCount() const { return m_scenes.size(); }

private:

    std::unordered_map<std::string, SceneDefinition> m_scenes;
};

} // namespace fw
