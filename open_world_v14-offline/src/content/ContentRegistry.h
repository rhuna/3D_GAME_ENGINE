#pragma once

#include <cstddef>
#include <string>
#include <vector>

namespace fw {

enum class ContentKind {
    Unknown,
    Prefab,
    Variant,
    Scene,
    Kit,
    Template,
    Project,
    Model,
    Texture,
    Audio
};

struct ContentEntry {
    std::string id;
    ContentKind kind = ContentKind::Unknown;
    std::string path;
    std::string category;
    std::vector<std::string> tags;
};

class ContentRegistry {
public:
    void Clear();
    bool RebuildFromProject();
    bool SaveToFile(const std::string& filePath) const;
    bool LoadFromFile(const std::string& filePath);
    bool SaveDiagnosticsReport(const std::string& filePath) const;

    const std::vector<ContentEntry>& Entries() const { return m_entries; }
    std::size_t Count() const { return m_entries.size(); }
    std::size_t CountByKind(ContentKind kind) const;
    std::vector<ContentEntry> FindByKind(ContentKind kind) const;
    std::vector<ContentEntry> Search(const std::string& text) const;

    static const char* KindToString(ContentKind kind);
    static ContentKind KindFromString(const std::string& value);

private:
    void AddEntry(ContentEntry entry);
    void ScanDirectory(const std::string& root, const std::vector<std::string>& extensions, ContentKind kind, const std::vector<std::string>& extraTags = {});
    std::vector<ContentEntry> m_entries;
};

} // namespace fw
