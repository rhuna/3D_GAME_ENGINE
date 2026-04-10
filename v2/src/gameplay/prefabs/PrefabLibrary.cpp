#include "gameplay/prefabs/PrefabLibrary.h"

#include <filesystem>
#include <fstream>
#include <utility>

#include "core/Logger.h"
#include "util/StringUtil.h"

namespace fw {

namespace {

bool ParseBool(const std::string& value) {
    return value == "1" || value == "true" || value == "TRUE" || value == "True";
}

float ParseFloat(const std::string& value) {
    try { return std::stof(value); } catch (...) { return 0.0f; }
}

Vector3 ParseVector3(const std::string& value) {
    const auto parts = StringUtil::Split(value, ',');
    if (parts.size() != 3) return Vector3{0.0f, 0.0f, 0.0f};
    return Vector3{
        ParseFloat(StringUtil::Trim(parts[0])),
        ParseFloat(StringUtil::Trim(parts[1])),
        ParseFloat(StringUtil::Trim(parts[2]))
    };
}

Color ParseColor(const std::string& value) {
    const auto parts = StringUtil::Split(value, ',');
    if (parts.size() != 4) return WHITE;
    return Color{
        static_cast<unsigned char>(ParseFloat(StringUtil::Trim(parts[0]))),
        static_cast<unsigned char>(ParseFloat(StringUtil::Trim(parts[1]))),
        static_cast<unsigned char>(ParseFloat(StringUtil::Trim(parts[2]))),
        static_cast<unsigned char>(ParseFloat(StringUtil::Trim(parts[3])))
    };
}

void ApplyCommonPrefabKey(PrefabDefinition& definition, const std::string& key, const std::string& value) {
    if (key == "name") definition.name = value;
    else if (key == "tag") definition.tag.value = value;
    else if (key == "position") definition.transform.position = ParseVector3(value);
    else if (key == "rotationEuler") definition.transform.rotationEuler = ParseVector3(value);
    else if (key == "scale") definition.transform.scale = ParseVector3(value);
    else if (key == "color") definition.render.tint = ParseColor(value);
    else if (key == "visible") definition.render.visible = ParseBool(value);
    else if (key == "drawCube") definition.render.drawCube = ParseBool(value);
    else if (key == "drawSphere") definition.render.drawSphere = ParseBool(value);
    else if (key == "cubeSize") definition.render.cubeSize = ParseFloat(value);
    else if (key == "sphereRadius") definition.render.sphereRadius = ParseFloat(value);
    else if (key == "useModel") definition.render.useModel = ParseBool(value);
    else if (key == "modelPath") definition.render.modelPath = value;
    else if (key == "colliderHalfExtents") { definition.hasCollider = true; definition.collider.halfExtents = ParseVector3(value); }
    else if (key == "colliderIsTrigger") { definition.hasCollider = true; definition.collider.isTrigger = ParseBool(value); }
    else if (key == "colliderIsStatic") { definition.hasCollider = true; definition.collider.isStatic = ParseBool(value); }
    else if (key == "rigidbodyVelocity") { definition.hasRigidbody = true; definition.rigidbody.velocity = ParseVector3(value); }
    else if (key == "rigidbodyAcceleration") { definition.hasRigidbody = true; definition.rigidbody.acceleration = ParseVector3(value); }
    else if (key == "rigidbodyDrag") { definition.hasRigidbody = true; definition.rigidbody.drag = ParseFloat(value); }
    else if (key == "rigidbodyUseGravity") { definition.hasRigidbody = true; definition.rigidbody.useGravity = ParseBool(value); }
    else if (key == "rigidbodyKinematic") { definition.hasRigidbody = true; definition.rigidbody.kinematic = ParseBool(value); }
    else if (key == "lifetime") { definition.hasLifetime = true; definition.lifetime.secondsRemaining = ParseFloat(value); }
}

void ApplyVariantKey(PrefabVariantDefinition& definition, const std::string& key, const std::string& value) {
    if (key == "name") definition.name = value;
    else if (key == "base") definition.basePrefab = value;
    else if (key == "basePrefab") definition.basePrefab = value;
    else if (key == "baseVariant") definition.baseVariant = value;
    else if (key == "tag") { definition.overrideTag = true; definition.tag.value = value; }
    else if (key == "position") { definition.overridePosition = true; definition.transform.position = ParseVector3(value); }
    else if (key == "rotationEuler") { definition.overrideRotation = true; definition.transform.rotationEuler = ParseVector3(value); }
    else if (key == "scale") { definition.overrideScale = true; definition.transform.scale = ParseVector3(value); }
    else if (key == "color") { definition.overrideRender = true; definition.render.tint = ParseColor(value); }
    else if (key == "visible") { definition.overrideRender = true; definition.render.visible = ParseBool(value); }
    else if (key == "drawCube") { definition.overrideRender = true; definition.render.drawCube = ParseBool(value); }
    else if (key == "drawSphere") { definition.overrideRender = true; definition.render.drawSphere = ParseBool(value); }
    else if (key == "cubeSize") { definition.overrideRender = true; definition.render.cubeSize = ParseFloat(value); }
    else if (key == "sphereRadius") { definition.overrideRender = true; definition.render.sphereRadius = ParseFloat(value); }
    else if (key == "useModel") { definition.overrideRender = true; definition.render.useModel = ParseBool(value); }
    else if (key == "modelPath") { definition.overrideRender = true; definition.render.modelPath = value; }
    else if (key == "colliderHalfExtents") { definition.overrideCollider = true; definition.collider.halfExtents = ParseVector3(value); }
    else if (key == "colliderIsTrigger") { definition.overrideCollider = true; definition.collider.isTrigger = ParseBool(value); }
    else if (key == "colliderIsStatic") { definition.overrideCollider = true; definition.collider.isStatic = ParseBool(value); }
    else if (key == "rigidbodyVelocity") { definition.overrideRigidbody = true; definition.rigidbody.velocity = ParseVector3(value); }
    else if (key == "rigidbodyAcceleration") { definition.overrideRigidbody = true; definition.rigidbody.acceleration = ParseVector3(value); }
    else if (key == "rigidbodyDrag") { definition.overrideRigidbody = true; definition.rigidbody.drag = ParseFloat(value); }
    else if (key == "rigidbodyUseGravity") { definition.overrideRigidbody = true; definition.rigidbody.useGravity = ParseBool(value); }
    else if (key == "rigidbodyKinematic") { definition.overrideRigidbody = true; definition.rigidbody.kinematic = ParseBool(value); }
    else if (key == "lifetime") { definition.overrideLifetime = true; definition.lifetime.secondsRemaining = ParseFloat(value); }
}

void ApplyVariantToPrefab(const PrefabVariantDefinition& variant, PrefabDefinition& prefab) {
    if (variant.overrideTag) prefab.tag = variant.tag;
    if (variant.overridePosition) prefab.transform.position = variant.transform.position;
    if (variant.overrideRotation) prefab.transform.rotationEuler = variant.transform.rotationEuler;
    if (variant.overrideScale) prefab.transform.scale = variant.transform.scale;
    if (variant.overrideRender) prefab.render = variant.render;
    if (variant.overrideCollider) { prefab.hasCollider = true; prefab.collider = variant.collider; }
    if (variant.overrideRigidbody) { prefab.hasRigidbody = true; prefab.rigidbody = variant.rigidbody; }
    if (variant.overrideLifetime) { prefab.hasLifetime = true; prefab.lifetime = variant.lifetime; }
}

} // namespace

void PrefabLibrary::Clear() {
    m_prefabs.clear();
    m_variants.clear();
}

bool PrefabLibrary::LoadFromDirectory(const std::string& directoryPath) {
    m_prefabs.clear();
    namespace fs = std::filesystem;
    if (!fs::exists(directoryPath)) {
        Logger::Warn("Prefab directory not found: " + directoryPath);
        return false;
    }
    bool loadedAny = false;
    for (const auto& entry : fs::directory_iterator(directoryPath)) {
        if (!entry.is_regular_file() || entry.path().extension() != ".prefab") continue;
        loadedAny = LoadPrefabFile(entry.path().string()) || loadedAny;
    }
    Logger::Info("Loaded prefab count: " + std::to_string(m_prefabs.size()));
    return loadedAny;
}

bool PrefabLibrary::LoadVariantsFromDirectory(const std::string& directoryPath) {
    m_variants.clear();
    namespace fs = std::filesystem;
    if (!fs::exists(directoryPath)) {
        Logger::Warn("Prefab variant directory not found: " + directoryPath);
        return false;
    }
    bool loadedAny = false;
    for (const auto& entry : fs::directory_iterator(directoryPath)) {
        if (!entry.is_regular_file() || entry.path().extension() != ".variant") continue;
        loadedAny = LoadVariantFile(entry.path().string()) || loadedAny;
    }
    Logger::Info("Loaded prefab variant count: " + std::to_string(m_variants.size()));
    return loadedAny;
}

void PrefabLibrary::Register(PrefabDefinition definition) { if (!definition.name.empty()) m_prefabs[definition.name] = std::move(definition); }
void PrefabLibrary::RegisterVariant(PrefabVariantDefinition definition) { if (!definition.name.empty()) m_variants[definition.name] = std::move(definition); }

const PrefabDefinition* PrefabLibrary::Find(const std::string& name) const {
    const auto it = m_prefabs.find(name);
    return it != m_prefabs.end() ? &it->second : nullptr;
}

const PrefabVariantDefinition* PrefabLibrary::FindVariant(const std::string& name) const {
    const auto it = m_variants.find(name);
    return it != m_variants.end() ? &it->second : nullptr;
}

bool PrefabLibrary::BuildPrefabFromVariant(const std::string& variantName, PrefabDefinition& outPrefab) const {
    std::unordered_set<std::string> stack;
    return BuildPrefabFromVariantRecursive(variantName, outPrefab, stack);
}

bool PrefabLibrary::BuildPrefabFromVariantRecursive(const std::string& variantName, PrefabDefinition& outPrefab, std::unordered_set<std::string>& stack) const {
    if (stack.contains(variantName)) {
        Logger::Warn("Prefab variant cycle detected at: " + variantName);
        return false;
    }

    const PrefabVariantDefinition* variant = FindVariant(variantName);
    if (!variant) return false;

    stack.insert(variantName);

    if (!variant->baseVariant.empty()) {
        if (!BuildPrefabFromVariantRecursive(variant->baseVariant, outPrefab, stack)) {
            stack.erase(variantName);
            return false;
        }
    } else {
        const PrefabDefinition* base = Find(variant->basePrefab);
        if (!base) {
            Logger::Warn("Variant base prefab not found: " + variant->basePrefab);
            stack.erase(variantName);
            return false;
        }
        outPrefab = *base;
    }

    ApplyVariantToPrefab(*variant, outPrefab);
    outPrefab.name = variant->name;
    if (outPrefab.tag.value.empty()) outPrefab.tag.value = variant->name;
    stack.erase(variantName);
    return true;
}

bool PrefabLibrary::LoadPrefabFile(const std::string& filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) { Logger::Warn("Failed to open prefab file: " + filePath); return false; }
    PrefabDefinition definition;
    std::string line;
    while (std::getline(file, line)) {
        line = StringUtil::Trim(line);
        if (line.empty() || line[0] == '#') continue;
        const std::size_t equals = line.find('=');
        if (equals == std::string::npos) continue;
        ApplyCommonPrefabKey(definition, StringUtil::Trim(line.substr(0, equals)), StringUtil::Trim(line.substr(equals + 1)));
    }
    if (definition.name.empty()) { Logger::Warn("Prefab missing name: " + filePath); return false; }
    if (definition.tag.value.empty()) definition.tag.value = definition.name;
    Register(std::move(definition));
    Logger::Info("Loaded prefab: " + filePath);
    return true;
}

bool PrefabLibrary::LoadVariantFile(const std::string& filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) { Logger::Warn("Failed to open prefab variant file: " + filePath); return false; }
    PrefabVariantDefinition definition;
    std::string line;
    while (std::getline(file, line)) {
        line = StringUtil::Trim(line);
        if (line.empty() || line[0] == '#') continue;
        const std::size_t equals = line.find('=');
        if (equals == std::string::npos) continue;
        ApplyVariantKey(definition, StringUtil::Trim(line.substr(0, equals)), StringUtil::Trim(line.substr(equals + 1)));
    }
    if (definition.name.empty() || (definition.basePrefab.empty() && definition.baseVariant.empty())) {
        Logger::Warn("Prefab variant missing name/base: " + filePath);
        return false;
    }
    RegisterVariant(std::move(definition));
    Logger::Info("Loaded prefab variant: " + filePath);
    return true;
}

} // namespace fw
