#pragma once

#include <string>
#include <unordered_map>
#include <vector>

#include "raylib.h"

namespace fw {

class AssetManager {
public:
    ~AssetManager();

    Texture2D* LoadTextureCached(const std::string& path);
    Model* LoadModelCached(const std::string& path);
    Shader* LoadShaderCached(const std::string& vertexPath, const std::string& fragmentPath);
    void PreloadAssets(const std::vector<std::string>& assetPaths);

    void UnloadAll();

private:
    std::unordered_map<std::string, Texture2D> m_textures;
    std::unordered_map<std::string, Model> m_models;
    std::unordered_map<std::string, Shader> m_shaders;
};

} // namespace fw
