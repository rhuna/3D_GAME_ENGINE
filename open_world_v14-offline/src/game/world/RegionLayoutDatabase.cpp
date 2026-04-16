#include "game/world/RegionLayoutDatabase.h"

#include <algorithm>
#include <cctype>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

namespace fs = std::filesystem;

namespace fw {
namespace {

Color MakeColor(unsigned char r, unsigned char g, unsigned char b) {
    return Color{r, g, b, 255};
}

std::string Trim(const std::string& value) {
    const std::string whitespace = " \t\r\n";
    const std::size_t begin = value.find_first_not_of(whitespace);
    if (begin == std::string::npos) return {};
    const std::size_t end = value.find_last_not_of(whitespace);
    return value.substr(begin, end - begin + 1);
}

std::string ReadFile(const fs::path& path) {
    std::ifstream in(path, std::ios::binary);
    if (!in.is_open()) return {};
    std::ostringstream buffer;
    buffer << in.rdbuf();
    return buffer.str();
}

std::size_t FindMatching(const std::string& text, std::size_t openPos, char openChar, char closeChar) {
    if (openPos == std::string::npos || openPos >= text.size() || text[openPos] != openChar) {
        return std::string::npos;
    }

    int depth = 0;
    bool inString = false;
    for (std::size_t i = openPos; i < text.size(); ++i) {
        const char c = text[i];
        if (c == '"' && (i == 0 || text[i - 1] != '\\')) {
            inString = !inString;
        }
        if (inString) continue;
        if (c == openChar) ++depth;
        else if (c == closeChar) {
            --depth;
            if (depth == 0) return i;
        }
    }

    return std::string::npos;
}

std::string ExtractJsonString(const std::string& text, const std::string& key) {
    const std::string token = "\"" + key + "\"";
    const std::size_t keyPos = text.find(token);
    if (keyPos == std::string::npos) return {};
    const std::size_t colonPos = text.find(':', keyPos + token.size());
    if (colonPos == std::string::npos) return {};
    const std::size_t firstQuote = text.find('"', colonPos + 1);
    if (firstQuote == std::string::npos) return {};
    const std::size_t secondQuote = text.find('"', firstQuote + 1);
    if (secondQuote == std::string::npos) return {};
    return text.substr(firstQuote + 1, secondQuote - firstQuote - 1);
}

bool ExtractJsonBool(const std::string& text, const std::string& key, bool defaultValue) {
    const std::string token = "\"" + key + "\"";
    const std::size_t keyPos = text.find(token);
    if (keyPos == std::string::npos) return defaultValue;
    const std::size_t colonPos = text.find(':', keyPos + token.size());
    if (colonPos == std::string::npos) return defaultValue;
    const std::string tail = Trim(text.substr(colonPos + 1, 8));
    if (tail.rfind("true", 0) == 0) return true;
    if (tail.rfind("false", 0) == 0) return false;
    return defaultValue;
}

std::string ExtractJsonObject(const std::string& text, const std::string& key) {
    const std::string token = "\"" + key + "\"";
    const std::size_t keyPos = text.find(token);
    if (keyPos == std::string::npos) return {};
    const std::size_t colonPos = text.find(':', keyPos + token.size());
    if (colonPos == std::string::npos) return {};
    const std::size_t openBrace = text.find('{', colonPos + 1);
    if (openBrace == std::string::npos) return {};
    const std::size_t closeBrace = FindMatching(text, openBrace, '{', '}');
    if (closeBrace == std::string::npos) return {};
    return text.substr(openBrace, closeBrace - openBrace + 1);
}

std::string ExtractJsonArray(const std::string& text, const std::string& key) {
    const std::string token = "\"" + key + "\"";
    const std::size_t keyPos = text.find(token);
    if (keyPos == std::string::npos) return {};
    const std::size_t colonPos = text.find(':', keyPos + token.size());
    if (colonPos == std::string::npos) return {};
    const std::size_t openBracket = text.find('[', colonPos + 1);
    if (openBracket == std::string::npos) return {};
    const std::size_t closeBracket = FindMatching(text, openBracket, '[', ']');
    if (closeBracket == std::string::npos) return {};
    return text.substr(openBracket, closeBracket - openBracket + 1);
}

std::vector<float> ParseFloatArray(const std::string& text) {
    std::vector<float> values;
    std::string current;
    bool inside = false;
    for (char c : text) {
        if (c == '[') {
            inside = true;
            continue;
        }
        if (!inside) continue;
        if ((c >= '0' && c <= '9') || c == '-' || c == '+' || c == '.') {
            current.push_back(c);
        } else {
            if (!current.empty()) {
                values.push_back(std::stof(current));
                current.clear();
            }
            if (c == ']') break;
        }
    }
    if (!current.empty()) values.push_back(std::stof(current));
    return values;
}

Vector3 ParseVector3(const std::string& text, const Vector3& fallback = Vector3{0.0f, 1.0f, 0.0f}) {
    const std::vector<float> values = ParseFloatArray(text);
    if (values.size() >= 3) {
        return Vector3{values[0], values[1], values[2]};
    }
    return fallback;
}

Color ParseColor(const std::string& text, const Color& fallback = Color{200, 200, 200, 255}) {
    const std::vector<float> values = ParseFloatArray(text);
    if (values.size() >= 3) {
        const unsigned char r = static_cast<unsigned char>(std::clamp(static_cast<int>(values[0]), 0, 255));
        const unsigned char g = static_cast<unsigned char>(std::clamp(static_cast<int>(values[1]), 0, 255));
        const unsigned char b = static_cast<unsigned char>(std::clamp(static_cast<int>(values[2]), 0, 255));
        const unsigned char a = values.size() >= 4 ? static_cast<unsigned char>(std::clamp(static_cast<int>(values[3]), 0, 255)) : 255;
        return Color{r, g, b, a};
    }
    return fallback;
}

std::vector<std::string> SplitTopLevelObjects(const std::string& arrayText) {
    std::vector<std::string> objects;
    if (arrayText.empty()) return objects;

    const std::size_t open = arrayText.find('[');
    const std::size_t close = arrayText.rfind(']');
    if (open == std::string::npos || close == std::string::npos || close <= open) return objects;

    const std::string body = arrayText.substr(open + 1, close - open - 1);
    bool inString = false;
    int depth = 0;
    std::size_t start = std::string::npos;

    for (std::size_t i = 0; i < body.size(); ++i) {
        const char c = body[i];
        if (c == '"' && (i == 0 || body[i - 1] != '\\')) inString = !inString;
        if (inString) continue;
        if (c == '{') {
            if (depth == 0) start = i;
            ++depth;
        } else if (c == '}') {
            --depth;
            if (depth == 0 && start != std::string::npos) {
                objects.push_back(body.substr(start, i - start + 1));
                start = std::string::npos;
            }
        }
    }

    return objects;
}

std::vector<std::string> SplitTopLevelArrays(const std::string& arrayText) {
    std::vector<std::string> arrays;
    if (arrayText.empty()) return arrays;

    const std::size_t open = arrayText.find('[');
    const std::size_t close = arrayText.rfind(']');
    if (open == std::string::npos || close == std::string::npos || close <= open) return arrays;

    const std::string body = arrayText.substr(open + 1, close - open - 1);
    bool inString = false;
    int depth = 0;
    std::size_t start = std::string::npos;

    for (std::size_t i = 0; i < body.size(); ++i) {
        const char c = body[i];
        if (c == '"' && (i == 0 || body[i - 1] != '\\')) inString = !inString;
        if (inString) continue;
        if (c == '[') {
            if (depth == 0) start = i;
            ++depth;
        } else if (c == ']') {
            --depth;
            if (depth == 0 && start != std::string::npos) {
                arrays.push_back(body.substr(start, i - start + 1));
                start = std::string::npos;
            }
        }
    }

    return arrays;
}

bool ParseJsonRegionLayoutFile(const fs::path& path, RegionLayout& layout) {
    const std::string text = ReadFile(path);
    if (text.empty()) return false;

    layout.regionId = ExtractJsonString(text, "id");
    if (layout.regionId.empty()) return false;

    const std::string layoutObject = ExtractJsonObject(text, "layout");
    if (layoutObject.empty()) return false;

    const std::string playerSpawnArray = ExtractJsonArray(layoutObject, "playerSpawn");
    if (!playerSpawnArray.empty()) {
        layout.playerSpawn = ParseVector3(playerSpawnArray, layout.playerSpawn);
    }

    for (const std::string& propObject : SplitTopLevelObjects(ExtractJsonArray(layoutObject, "props"))) {
        LayoutPrimitive primitive;
        const std::string type = ExtractJsonString(propObject, "type");
        primitive.type = (type == "cylinder") ? LayoutPrimitiveType::Cylinder : LayoutPrimitiveType::Box;

        const std::string pos = ExtractJsonArray(propObject, "position");
        const std::string size = ExtractJsonArray(propObject, "size");
        const std::string color = ExtractJsonArray(propObject, "color");

        if (!pos.empty()) primitive.position = ParseVector3(pos, primitive.position);
        if (!size.empty()) primitive.size = ParseVector3(size, primitive.size);
        if (!color.empty()) primitive.color = ParseColor(color, primitive.color);
        layout.props.push_back(primitive);
    }

    for (const std::string& npcObject : SplitTopLevelObjects(ExtractJsonArray(layoutObject, "npcSpawns"))) {
        NpcSpawn spawn;
        spawn.npcId = ExtractJsonString(npcObject, "npcId");
        const std::string pos = ExtractJsonArray(npcObject, "position");
        if (!pos.empty()) spawn.position = ParseVector3(pos, spawn.position);
        layout.npcSpawns.push_back(spawn);
    }

    for (const std::string& pointArray : SplitTopLevelArrays(ExtractJsonArray(layoutObject, "gatherPoints"))) {
        layout.gatherPoints.push_back(ParseVector3(pointArray));
    }

    for (const std::string& travelObject : SplitTopLevelObjects(ExtractJsonArray(layoutObject, "travelPoints"))) {
        TravelPoint point;
        point.targetRegion = ExtractJsonString(travelObject, "targetRegion");
        point.anchorId = ExtractJsonString(travelObject, "anchorId");
        const std::string pos = ExtractJsonArray(travelObject, "position");
        const std::string dest = ExtractJsonArray(travelObject, "destinationSpawn");
        if (!pos.empty()) point.position = ParseVector3(pos, point.position);
        if (!dest.empty()) point.destinationSpawn = ParseVector3(dest, point.destinationSpawn);
        layout.travelPoints.push_back(point);
    }

    for (const std::string& enemyObject : SplitTopLevelObjects(ExtractJsonArray(layoutObject, "enemySpawns"))) {
        EnemySpawn spawn;
        spawn.type = ExtractJsonString(enemyObject, "type");
        const std::string pos = ExtractJsonArray(enemyObject, "position");
        if (!pos.empty()) spawn.position = ParseVector3(pos, spawn.position);
        if (!spawn.type.empty()) layout.enemySpawns.push_back(spawn);
    }

    return true;
}

} // namespace

bool RegionLayoutDatabase::LoadFromDirectory(const std::string& dir) {
    m_layouts.clear();
    if (!fs::exists(dir)) return false;

    bool loadedAny = false;
    for (const auto& entry : fs::recursive_directory_iterator(dir)) {
        if (!entry.is_regular_file()) continue;
        if (entry.path().extension() != ".json") continue;
        if (entry.path().parent_path().filename() != "regions") continue;

        RegionLayout layout;
        if (!ParseJsonRegionLayoutFile(entry.path(), layout)) continue;
        m_layouts[layout.regionId] = std::move(layout);
        loadedAny = true;
    }

    if (!loadedAny) {
        LoadDefaults();
    }

    return loadedAny;
}

void RegionLayoutDatabase::LoadDefaults() {
    m_layouts.clear();

    RegionLayout village{};
    village.regionId = "village_region";
    village.playerSpawn = {0, 1, 0};
    village.props.push_back({LayoutPrimitiveType::Box, {0, 0.5f, 0}, {50, 1, 50}, MakeColor(95, 125, 85)});
    village.props.push_back({LayoutPrimitiveType::Box, {-8, 2, -6}, {6, 4, 6}, MakeColor(150, 110, 70)});
    village.props.push_back({LayoutPrimitiveType::Box, {10, 2, 4}, {5, 4, 5}, MakeColor(140, 100, 60)});
    village.props.push_back({LayoutPrimitiveType::Cylinder, {4, 2, -10}, {1.5f, 4, 1.5f}, MakeColor(90, 150, 80)});
    village.npcSpawns.push_back({"runtime_npc_0", {2, 1, -2}});
    village.npcSpawns.push_back({"runtime_npc_1", {-5, 1, 3}});
    village.gatherPoints.push_back({8, 1, 8});
    village.travelPoints.push_back({{20, 1, 0}, "forest_region", "east_gate", {0, 1, 0}});
    village.travelPoints.push_back({{-20, 1, 0}, "forest_region", "west_gate", {0, 1, 0}});
    m_layouts[village.regionId] = village;

    RegionLayout forest{};
    forest.regionId = "forest_region";
    forest.playerSpawn = {0, 1, 0};
    forest.props.push_back({LayoutPrimitiveType::Box, {0, 0.5f, 0}, {70, 1, 70}, MakeColor(55, 105, 60)});
    forest.props.push_back({LayoutPrimitiveType::Cylinder, {-10, 2, -10}, {1.2f, 4, 1.2f}, MakeColor(80, 130, 70)});
    forest.props.push_back({LayoutPrimitiveType::Cylinder, {6, 2, -14}, {1.6f, 4, 1.6f}, MakeColor(75, 140, 75)});
    forest.props.push_back({LayoutPrimitiveType::Cylinder, {14, 2, 8}, {1.3f, 4, 1.3f}, MakeColor(70, 135, 70)});
    forest.props.push_back({LayoutPrimitiveType::Box, {-4, 0.6f, 12}, {4, 1.2f, 3}, MakeColor(120, 90, 60)});
    forest.npcSpawns.push_back({"runtime_npc_0", {-6, 1, 4}});
    forest.gatherPoints.push_back({3, 1, 6});
    forest.gatherPoints.push_back({10, 1, -2});
    forest.gatherPoints.push_back({-12, 1, -4});
    forest.travelPoints.push_back({{-25, 1, 0}, "village_region", "west_gate", {0, 1, 0}});
    forest.travelPoints.push_back({{25, 1, 0}, "ruins_region", "east_gate", {0, 1, 0}});
    forest.enemySpawns.push_back({{8, 1, 10}, "wolf"});
    forest.enemySpawns.push_back({{-10, 1, -8}, "wolf"});
    forest.enemySpawns.push_back({{12, 1, -12}, "raider"});
    m_layouts[forest.regionId] = forest;

    RegionLayout ruins{};
    ruins.regionId = "ruins_region";
    ruins.playerSpawn = {0, 1, 0};
    ruins.props.push_back({LayoutPrimitiveType::Box, {0, 0.5f, 0}, {60, 1, 60}, MakeColor(90, 90, 95)});
    ruins.props.push_back({LayoutPrimitiveType::Box, {-10, 2, 0}, {4, 4, 10}, MakeColor(115, 115, 125)});
    ruins.props.push_back({LayoutPrimitiveType::Box, {10, 2, -4}, {6, 4, 4}, MakeColor(130, 130, 140)});
    ruins.props.push_back({LayoutPrimitiveType::Box, {0, 1.5f, 10}, {8, 3, 2}, MakeColor(120, 120, 130)});
    ruins.gatherPoints.push_back({2, 1, -6});
    ruins.travelPoints.push_back({{0, 1, -22}, "village_region", "south_gate", {0, 1, 0}});
    ruins.enemySpawns.push_back({{5, 1, 6}, "brute"});
    ruins.enemySpawns.push_back({{-8, 1, -10}, "raider"});
    m_layouts[ruins.regionId] = ruins;
}

const RegionLayout* RegionLayoutDatabase::Find(const std::string& regionId) const {
    const auto it = m_layouts.find(regionId);
    return it == m_layouts.end() ? nullptr : &it->second;
}

} // namespace fw
