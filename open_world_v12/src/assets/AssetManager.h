#pragma once

#include <string>
#include <unordered_map>
#include <vector>

#include "raylib.h"

namespace fw {

struct AssetBrowserEntry {
    std::string category;
    std::string name;
    std::string path;
};

class AssetManager {
public:
    ~AssetManager();

    Texture2D* LoadTextureCached(const std::string& path);
    Model* LoadModelCached(const std::string& path);
    Shader* LoadShaderCached(const std::string& vertexPath, const std::string& fragmentPath);

    // Compatibility wrappers for older call sites.
    Texture2D* GetTexture(const std::string& path);
    Model* GetModel(const std::string& path);
    Shader* GetShader(const std::string& vertexPath, const std::string& fragmentPath);
    void PreloadAssets(const std::vector<std::string>& assetPaths);

    void ScanAssetBrowser(const std::string& rootDirectory = "assets");
    const std::vector<AssetBrowserEntry>& AssetBrowserEntries() const { return m_assetBrowserEntries; }

    void UnloadAll();

private:
    static std::string GuessAssetCategory(const std::string& path);
    static std::string BasenameWithoutExtension(const std::string& path);

    std::unordered_map<std::string, Texture2D> m_textures;
    std::unordered_map<std::string, Model> m_models;
    std::unordered_map<std::string, Shader> m_shaders;
    std::vector<AssetBrowserEntry> m_assetBrowserEntries;
};

} // namespace fw
