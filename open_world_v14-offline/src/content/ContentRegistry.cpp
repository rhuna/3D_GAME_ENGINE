#include "content/ContentRegistry.h"

#include <algorithm>
#include <cctype>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <unordered_set>

#include "core/FileSystem.h"
#include "core/Logger.h"
#include "util/StringUtil.h"

namespace fs = std::filesystem;

namespace fw {
namespace {

std::string ToLowerCopy(std::string value) {
    std::transform(value.begin(), value.end(), value.begin(), [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
    return value;
}

std::string StemString(const std::string& path) {
    return fs::path(path).stem().string();
}

std::string ParentCategory(const std::string& root, const std::string& path) {
    std::error_code ec;
    const fs::path base(root);
    const fs::path file(path);
    const fs::path relative = fs::relative(file.parent_path(), base, ec);
    if (ec || relative.empty() || relative == ".") {
        return fs::path(root).filename().string();
    }
    return relative.generic_string();
}

std::vector<std::string> ReadTagsFromKit(const std::string& kitPath) {
    std::vector<std::string> tags;
    std::ifstream file(kitPath);
    std::string line;
    while (std::getline(file, line)) {
        line = StringUtil::Trim(line);
        if (line.rfind("tags=", 0) == 0) {
            auto parts = StringUtil::Split(line.substr(5), ',');
            for (auto& part : parts) {
                const std::string value = StringUtil::Trim(part);
                if (!value.empty()) tags.push_back(value);
            }
            break;
        }
    }
    return tags;
}

bool MatchesSearch(const ContentEntry& entry, const std::string& needle) {
    if (needle.empty()) return true;
    const std::string id = ToLowerCopy(entry.id);
    const std::string path = ToLowerCopy(entry.path);
    const std::string category = ToLowerCopy(entry.category);
    if (id.find(needle) != std::string::npos || path.find(needle) != std::string::npos || category.find(needle) != std::string::npos) return true;
    for (const auto& tag : entry.tags) {
        if (ToLowerCopy(tag).find(needle) != std::string::npos) return true;
    }
    return false;
}

} // namespace

void ContentRegistry::Clear() {
    m_entries.clear();
}

const char* ContentRegistry::KindToString(ContentKind kind) {
    switch (kind) {
        case ContentKind::Prefab: return "prefab";
        case ContentKind::Variant: return "variant";
        case ContentKind::Scene: return "scene";
        case ContentKind::Kit: return "kit";
        case ContentKind::Template: return "template";
        case ContentKind::Project: return "project";
        case ContentKind::Model: return "model";
        case ContentKind::Texture: return "texture";
        case ContentKind::Audio: return "audio";
        case ContentKind::Unknown: default: return "unknown";
    }
}

ContentKind ContentRegistry::KindFromString(const std::string& value) {
    const std::string lowered = ToLowerCopy(value);
    if (lowered == "prefab") return ContentKind::Prefab;
    if (lowered == "variant") return ContentKind::Variant;
    if (lowered == "scene") return ContentKind::Scene;
    if (lowered == "kit") return ContentKind::Kit;
    if (lowered == "template") return ContentKind::Template;
    if (lowered == "project") return ContentKind::Project;
    if (lowered == "model") return ContentKind::Model;
    if (lowered == "texture") return ContentKind::Texture;
    if (lowered == "audio") return ContentKind::Audio;
    return ContentKind::Unknown;
}

void ContentRegistry::AddEntry(ContentEntry entry) {
    if (entry.id.empty()) entry.id = StemString(entry.path);
    if (entry.category.empty()) entry.category = "misc";
    m_entries.push_back(std::move(entry));
}

void ContentRegistry::ScanDirectory(const std::string& root, const std::vector<std::string>& extensions, ContentKind kind, const std::vector<std::string>& extraTags) {
    for (const std::string& filePath : FileSystem::ListFilesRecursive(root, extensions)) {
        ContentEntry entry;
        entry.id = StemString(filePath);
        entry.kind = kind;
        entry.path = filePath;
        entry.category = ParentCategory(root, filePath);
        entry.tags = extraTags;
        if (kind == ContentKind::Kit) {
            entry.tags.push_back("generated");
            for (const auto& t : ReadTagsFromKit(filePath)) entry.tags.push_back(t);
        }
        AddEntry(std::move(entry));
    }
}

bool ContentRegistry::RebuildFromProject() {
    Clear();

    // Scan both the classic and newer low-code authoring folders so the
    // registry reflects the real uploaded repo structure instead of only the
    // earlier subset of content roots.
    ScanDirectory("assets/prefabs", {".prefab"}, ContentKind::Prefab);
    ScanDirectory("assets/prefab_variants", {".variant", ".prefabvariant", ".txt"}, ContentKind::Variant);
    ScanDirectory("assets/scenes", {".scene"}, ContentKind::Scene);
    ScanDirectory("assets/scenes/generated", {".kit"}, ContentKind::Kit);

    ScanDirectory("assets/template", {".json", ".template"}, ContentKind::Template);
    ScanDirectory("assets/templates", {".json", ".template"}, ContentKind::Template);

    ScanDirectory("assets/game", {".project"}, ContentKind::Project);
    ScanDirectory("assets/projects", {".project"}, ContentKind::Project);
    if (FileSystem::Exists("assets/game.project")) {
        AddEntry(ContentEntry{"game", ContentKind::Project, "assets/game.project", "root", {"startup"}});
    }

    ScanDirectory("assets/models", {".glb", ".gltf", ".fbx", ".obj", ".iqm"}, ContentKind::Model);
    ScanDirectory("assets/textures", {".png", ".jpg", ".jpeg", ".bmp", ".tga", ".webp"}, ContentKind::Texture);

    // Keep legacy audio plus the newer ambience/music folders in one registry category.
    ScanDirectory("assets/audio", {".wav", ".ogg", ".mp3"}, ContentKind::Audio, {"audio"});
    ScanDirectory("assets/ambience", {".wav", ".ogg", ".mp3", ".ambience"}, ContentKind::Audio, {"ambience"});
    ScanDirectory("assets/music", {".wav", ".ogg", ".mp3", ".musiczone"}, ContentKind::Audio, {"music"});

    std::sort(m_entries.begin(), m_entries.end(), [](const ContentEntry& a, const ContentEntry& b) {
        if (a.kind != b.kind) return std::string(KindToString(a.kind)) < std::string(KindToString(b.kind));
        return a.id < b.id;
    });

    Logger::Info("V97", "Rebuilt local content registry with " + std::to_string(m_entries.size()) + " entries.");
    return !m_entries.empty();
}

bool ContentRegistry::SaveToFile(const std::string& filePath) const {
    std::ostringstream out;
    out << "# V71 local content registry\n";
    for (const auto& entry : m_entries) {
        out << "entry=id=" << entry.id
            << ";kind=" << KindToString(entry.kind)
            << ";path=" << entry.path
            << ";category=" << entry.category;
        if (!entry.tags.empty()) {
            out << ";tags=";
            for (std::size_t i = 0; i < entry.tags.size(); ++i) {
                if (i > 0) out << ',';
                out << entry.tags[i];
            }
        }
        out << "\n";
    }
    return FileSystem::WriteTextFile(filePath, out.str());
}

bool ContentRegistry::LoadFromFile(const std::string& filePath) {
    Clear();
    std::ifstream file(filePath);
    if (!file.is_open()) return false;

    std::string line;
    while (std::getline(file, line)) {
        line = StringUtil::Trim(line);
        if (line.empty() || line[0] == '#') continue;
        const std::size_t equals = line.find('=');
        if (equals == std::string::npos) continue;
        if (StringUtil::Trim(line.substr(0, equals)) != "entry") continue;

        ContentEntry entry;
        for (const auto& rawToken : StringUtil::Split(line.substr(equals + 1), ';')) {
            const std::string token = StringUtil::Trim(rawToken);
            if (token.empty()) continue;
            const std::size_t tokenEq = token.find('=');
            if (tokenEq == std::string::npos) continue;
            const std::string key = StringUtil::Trim(token.substr(0, tokenEq));
            const std::string value = StringUtil::Trim(token.substr(tokenEq + 1));
            if (key == "id") entry.id = value;
            else if (key == "kind") entry.kind = KindFromString(value);
            else if (key == "path") entry.path = value;
            else if (key == "category") entry.category = value;
            else if (key == "tags") {
                for (const auto& tag : StringUtil::Split(value, ',')) {
                    const std::string trimmed = StringUtil::Trim(tag);
                    if (!trimmed.empty()) entry.tags.push_back(trimmed);
                }
            }
        }
        if (!entry.id.empty() && !entry.path.empty()) AddEntry(std::move(entry));
    }

    return !m_entries.empty();
}

std::size_t ContentRegistry::CountByKind(ContentKind kind) const {
    return static_cast<std::size_t>(std::count_if(m_entries.begin(), m_entries.end(), [kind](const ContentEntry& entry) {
        return entry.kind == kind;
    }));
}

std::vector<ContentEntry> ContentRegistry::FindByKind(ContentKind kind) const {
    std::vector<ContentEntry> results;
    for (const auto& entry : m_entries) {
        if (entry.kind == kind) results.push_back(entry);
    }
    return results;
}

std::vector<ContentEntry> ContentRegistry::Search(const std::string& text) const {
    const std::string needle = ToLowerCopy(text);
    std::vector<ContentEntry> results;
    for (const auto& entry : m_entries) {
        if (MatchesSearch(entry, needle)) results.push_back(entry);
    }
    return results;
}

// -----------------------------------------------------------------------------
// SaveDiagnosticsReport
// -----------------------------------------------------------------------------
// Write a small registry diagnostics report to disk. This function must live
// inside the fw namespace and on the ContentRegistry class, otherwise MSVC will
// parse it as a free function and every class/member reference after that point
// will collapse into cascading compile errors.
// -----------------------------------------------------------------------------
bool ContentRegistry::SaveDiagnosticsReport(const std::string& filePath) const {
    std::ostringstream out;

    // Write a stable plain-text diagnostics report using escaped newline
    // sequences so the generated text stays readable while the C++ source stays
    // valid for MSVC and other compilers.
    out << "# V111 content registry diagnostics\\n";
    out << "entryCount=" << m_entries.size() << "\\n";
    out << "prefabs=" << CountByKind(ContentKind::Prefab) << "\\n";
    out << "variants=" << CountByKind(ContentKind::Variant) << "\\n";
    out << "scenes=" << CountByKind(ContentKind::Scene) << "\\n";
    out << "kits=" << CountByKind(ContentKind::Kit) << "\\n";
    out << "templates=" << CountByKind(ContentKind::Template) << "\\n";
    out << "projects=" << CountByKind(ContentKind::Project) << "\\n";
    out << "models=" << CountByKind(ContentKind::Model) << "\\n";
    out << "textures=" << CountByKind(ContentKind::Texture) << "\\n";
    out << "audio=" << CountByKind(ContentKind::Audio) << "\\n";

    return FileSystem::WriteTextFile(filePath, out.str());
}

} // namespace fw
