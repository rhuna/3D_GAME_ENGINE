#include "assets/AssetManager.h"

#include <algorithm>
#include <filesystem>

#include "core/FileSystem.h"
#include "core/Logger.h"

namespace fw {

namespace {

bool HasExtension(const std::string& path, const char* ext) {
    const std::filesystem::path p(path);
    return p.has_extension() && p.extension() == ext;
}

} // namespace

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


Texture2D* AssetManager::GetTexture(const std::string& path) {
    return LoadTextureCached(path);
}

Model* AssetManager::GetModel(const std::string& path) {
    return LoadModelCached(path);
}

Shader* AssetManager::GetShader(const std::string& vertexPath, const std::string& fragmentPath) {
    return LoadShaderCached(vertexPath, fragmentPath);
}

void AssetManager::PreloadAssets(const std::vector<std::string>& assetPaths) {
    for (const std::string& path : assetPaths) {
        if (path.empty()) {
            continue;
        }

        const std::string category = GuessAssetCategory(path);
        if (category == "Model") {
            (void)LoadModelCached(path);
        } else if (category == "Texture") {
            (void)LoadTextureCached(path);
        }
    }
}
void AssetManager::ScanAssetBrowser(const std::string& rootDirectory) {
    m_assetBrowserEntries.clear();

    for (const std::string& file : FileSystem::ListFilesRecursive(rootDirectory)) {
        const std::string category = GuessAssetCategory(file);
        if (category.empty()) continue;
        m_assetBrowserEntries.push_back(AssetBrowserEntry{category, BasenameWithoutExtension(file), file});
    }

    std::sort(m_assetBrowserEntries.begin(), m_assetBrowserEntries.end(), [](const AssetBrowserEntry& a, const AssetBrowserEntry& b) {
        if (a.category == b.category) return a.path < b.path;
        return a.category < b.category;
    });

    Logger::Info("Asset browser entries scanned: " + std::to_string(m_assetBrowserEntries.size()));
}

std::string AssetManager::GuessAssetCategory(const std::string& path) {
    if (HasExtension(path, ".glb") || HasExtension(path, ".gltf") || HasExtension(path, ".obj") || HasExtension(path, ".fbx") || HasExtension(path, ".iqm")) return "Model";
    if (HasExtension(path, ".png") || HasExtension(path, ".jpg") || HasExtension(path, ".jpeg") || HasExtension(path, ".bmp") || HasExtension(path, ".tga")) return "Texture";
    if (HasExtension(path, ".prefab")) return "Prefab";
    if (HasExtension(path, ".variant")) return "Variant";
    if (HasExtension(path, ".scene")) return "Scene";
    if (HasExtension(path, ".wav") || HasExtension(path, ".ogg") || HasExtension(path, ".mp3")) return "Audio";
    return {};
}

std::string AssetManager::BasenameWithoutExtension(const std::string& path) {
    const std::filesystem::path p(path);
    return p.stem().string();
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
