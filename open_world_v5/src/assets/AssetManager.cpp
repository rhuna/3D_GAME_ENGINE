#include "assets/AssetManager.h"

#include "core/FileSystem.h"
#include "core/Logger.h"

namespace fw {

AssetManager::~AssetManager() {
    UnloadAll();
}

Texture2D* AssetManager::LoadTextureCached(const std::string& path) {
    if (auto it = m_textures.find(path); it != m_textures.end()) {
        return &it->second;
    }

    if (!FileSystem::Exists(path)) {
        Logger::Warn("Texture not found: " + path);
        return nullptr;
    }

    Texture2D texture = LoadTexture(path.c_str());
    auto [it, inserted] = m_textures.emplace(path, texture);
    return inserted ? &it->second : nullptr;
}

Model* AssetManager::LoadModelCached(const std::string& path) {
    if (auto it = m_models.find(path); it != m_models.end()) {
        return &it->second;
    }

    if (!FileSystem::Exists(path)) {
        Logger::Warn("Model not found: " + path);
        return nullptr;
    }

    Model model = LoadModel(path.c_str());
    auto [it, inserted] = m_models.emplace(path, model);
    return inserted ? &it->second : nullptr;
}

Shader* AssetManager::LoadShaderCached(const std::string& vertexPath, const std::string& fragmentPath) {
    const std::string key = vertexPath + "|" + fragmentPath;
    if (auto it = m_shaders.find(key); it != m_shaders.end()) {
        return &it->second;
    }

    if ((!vertexPath.empty() && !FileSystem::Exists(vertexPath)) ||
        (!fragmentPath.empty() && !FileSystem::Exists(fragmentPath))) {
        Logger::Warn("Shader file missing: " + key);
        return nullptr;
    }

    Shader shader = LoadShader(vertexPath.empty() ? nullptr : vertexPath.c_str(),
                               fragmentPath.empty() ? nullptr : fragmentPath.c_str());
    auto [it, inserted] = m_shaders.emplace(key, shader);
    return inserted ? &it->second : nullptr;
}

void AssetManager::UnloadAll() {
    for (auto& [_, shader] : m_shaders) {
        UnloadShader(shader);
    }
    m_shaders.clear();

    for (auto& [_, model] : m_models) {
        UnloadModel(model);
    }
    m_models.clear();

    for (auto& [_, texture] : m_textures) {
        UnloadTexture(texture);
    }
    m_textures.clear();
}

} // namespace fw
