#include "game/content/RegionDatabase.h"

#include <filesystem>
#include <fstream>
#include <sstream>
#include <string>

namespace fs = std::filesystem;

namespace fw {
namespace {

std::string Trim(const std::string& value) {
    const std::string whitespace = " \t\r\n";
    const std::size_t begin = value.find_first_not_of(whitespace);
    if (begin == std::string::npos) {
        return {};
    }
    const std::size_t end = value.find_last_not_of(whitespace);
    return value.substr(begin, end - begin + 1);
}

std::string ReadFile(const fs::path& path) {
    std::ifstream in(path, std::ios::binary);
    if (!in.is_open()) {
        return {};
    }
    std::ostringstream buffer;
    buffer << in.rdbuf();
    return buffer.str();
}

std::string ExtractJsonString(const std::string& text, const std::string& key) {
    const std::string token = "\"" + key + "\"";
    const std::size_t keyPos = text.find(token);
    if (keyPos == std::string::npos) {
        return {};
    }
    const std::size_t colonPos = text.find(':', keyPos + token.size());
    if (colonPos == std::string::npos) {
        return {};
    }
    const std::size_t firstQuote = text.find('"', colonPos + 1);
    if (firstQuote == std::string::npos) {
        return {};
    }
    const std::size_t secondQuote = text.find('"', firstQuote + 1);
    if (secondQuote == std::string::npos) {
        return {};
    }
    return text.substr(firstQuote + 1, secondQuote - firstQuote - 1);
}

bool ExtractJsonBool(const std::string& text, const std::string& key, bool defaultValue) {
    const std::string token = "\"" + key + "\"";
    const std::size_t keyPos = text.find(token);
    if (keyPos == std::string::npos) {
        return defaultValue;
    }
    const std::size_t colonPos = text.find(':', keyPos + token.size());
    if (colonPos == std::string::npos) {
        return defaultValue;
    }
    const std::string tail = Trim(text.substr(colonPos + 1, 8));
    if (tail.rfind("true", 0) == 0) return true;
    if (tail.rfind("false", 0) == 0) return false;
    return defaultValue;
}

void ParseJsonLinks(const std::string& text, RegionDefinition& definition) {
    const std::string token = "\"links\"";
    const std::size_t keyPos = text.find(token);
    if (keyPos == std::string::npos) {
        return;
    }
    const std::size_t openBracket = text.find('[', keyPos + token.size());
    const std::size_t closeBracket = text.find(']', openBracket + 1);
    if (openBracket == std::string::npos || closeBracket == std::string::npos) {
        return;
    }

    std::string list = text.substr(openBracket + 1, closeBracket - openBracket - 1);
    std::size_t cursor = 0;
    while (true) {
        const std::size_t openBrace = list.find('{', cursor);
        if (openBrace == std::string::npos) {
            break;
        }
        const std::size_t closeBrace = list.find('}', openBrace + 1);
        if (closeBrace == std::string::npos) {
            break;
        }

        const std::string objectText = list.substr(openBrace, closeBrace - openBrace + 1);
        RegionLinkDefinition link;
        link.targetRegion = ExtractJsonString(objectText, "targetRegion");
        link.anchorId = ExtractJsonString(objectText, "anchorId");
        if (!link.targetRegion.empty()) {
            definition.links.push_back(link);
        }

        cursor = closeBrace + 1;
    }
}

bool ParseLegacyRegionFile(const fs::path& path, RegionDefinition& definition) {
    std::ifstream in(path);
    if (!in.is_open()) {
        return false;
    }

    std::string line;
    while (std::getline(in, line)) {
        const std::size_t equals = line.find('=');
        if (equals == std::string::npos) {
            continue;
        }

        const std::string key = Trim(line.substr(0, equals));
        const std::string value = Trim(line.substr(equals + 1));
        if (key == "id") definition.id = value;
        else if (key == "displayName") definition.displayName = value;
        else if (key == "sceneFile") definition.sceneFile = value;
        else if (key == "ambientMusicId") definition.ambientMusicId = value;
        else if (key == "description") definition.description = value;
        else if (key == "safeZone") definition.safeZone = (value == "true");
    }

    return !definition.id.empty();
}

bool ParseJsonRegionFile(const fs::path& path, RegionDefinition& definition) {
    const std::string text = ReadFile(path);
    if (text.empty()) {
        return false;
    }

    definition.id = ExtractJsonString(text, "id");
    definition.displayName = ExtractJsonString(text, "displayName");
    definition.sceneFile = ExtractJsonString(text, "scene");
    definition.description = ExtractJsonString(text, "description");
    definition.ambientMusicId = ExtractJsonString(text, "ambientMusicId");
    definition.safeZone = ExtractJsonBool(text, "safeZone", false);
    ParseJsonLinks(text, definition);

    return !definition.id.empty();
}

bool ShouldParseRegionFile(const fs::path& path) {
    const std::string ext = path.extension().string();
    return ext == ".json" || ext == ".region";
}

} // namespace

bool RegionDatabase::LoadFromDirectory(const std::string& dir) {
    m.clear();
    if (!fs::exists(dir)) {
        return false;
    }

    bool loadedAny = false;
    for (const auto& entry : fs::recursive_directory_iterator(dir)) {
        if (!entry.is_regular_file()) {
            continue;
        }
        if (!ShouldParseRegionFile(entry.path())) {
            continue;
        }

        RegionDefinition definition{};
        bool loaded = false;

        if (entry.path().extension() == ".json") {
            const std::string parent = entry.path().parent_path().filename().string();
            if (parent != "regions") {
                continue;
            }
            loaded = ParseJsonRegionFile(entry.path(), definition);
        } else {
            loaded = ParseLegacyRegionFile(entry.path(), definition);
        }

        if (loaded && !definition.id.empty()) {
            m[definition.id] = std::move(definition);
            loadedAny = true;
        }
    }

    return loadedAny;
}

const RegionDefinition* RegionDatabase::Find(const std::string& id) const {
    const auto it = m.find(id);
    return it == m.end() ? nullptr : &it->second;
}

} // namespace fw
