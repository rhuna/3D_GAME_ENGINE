#include "game/world/RegionLayoutDatabase.h"

#include <cctype>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <string>

namespace fs = std::filesystem;

namespace fw {
namespace {

static Color MakeColor(unsigned char r, unsigned char g, unsigned char b) {
    return Color{r, g, b, 255};
}

static std::string ReadTextFile(const fs::path& path) {
    std::ifstream in(path, std::ios::binary);
    if (!in.is_open()) return {};
    std::ostringstream buffer;
    buffer << in.rdbuf();
    return buffer.str();
}

static std::size_t FindMatching(const std::string& text, std::size_t openPos, char openCh, char closeCh) {
    if (openPos == std::string::npos || openPos >= text.size() || text[openPos] != openCh) {
        return std::string::npos;
    }
    int depth = 0;
    bool inString = false;
    for (std::size_t i = openPos; i < text.size(); ++i) {
        const char ch = text[i];
        const bool escaped = i > 0 && text[i - 1] == '\\';
        if (ch == '"' && !escaped) {
            inString = !inString;
            continue;
        }
        if (inString) continue;
        if (ch == openCh) ++depth;
        else if (ch == closeCh) {
            --depth;
            if (depth == 0) return i;
        }
    }
    return std::string::npos;
}

static std::size_t FindKey(const std::string& text, const std::string& key) {
    return text.find("\"" + key + "\"");
}

static std::string ExtractString(const std::string& text, const std::string& key) {
    const std::size_t keyPos = FindKey(text, key);
    if (keyPos == std::string::npos) return {};
    const std::size_t colonPos = text.find(':', keyPos);
    if (colonPos == std::string::npos) return {};
    const std::size_t firstQuote = text.find('"', colonPos + 1);
    if (firstQuote == std::string::npos) return {};
    const std::size_t secondQuote = text.find('"', firstQuote + 1);
    if (secondQuote == std::string::npos) return {};
    return text.substr(firstQuote + 1, secondQuote - firstQuote - 1);
}

static bool ExtractBool(const std::string& text, const std::string& key, bool defaultValue) {
    const std::size_t keyPos = FindKey(text, key);
    if (keyPos == std::string::npos) return defaultValue;
    const std::size_t colonPos = text.find(':', keyPos);
    if (colonPos == std::string::npos) return defaultValue;
    const std::string tail = text.substr(colonPos + 1, 8);
    if (tail.find("true") != std::string::npos) return true;
    if (tail.find("false") != std::string::npos) return false;
    return defaultValue;
}

static std::string ExtractObject(const std::string& text, const std::string& key) {
    const std::size_t keyPos = FindKey(text, key);
    if (keyPos == std::string::npos) return {};
    const std::size_t openBrace = text.find('{', keyPos);
    if (openBrace == std::string::npos) return {};
    const std::size_t closeBrace = FindMatching(text, openBrace, '{', '}');
    if (closeBrace == std::string::npos) return {};
    return text.substr(openBrace, closeBrace - openBrace + 1);
}

static std::string ExtractArray(const std::string& text, const std::string& key) {
    const std::size_t keyPos = FindKey(text, key);
    if (keyPos == std::string::npos) return {};
    const std::size_t openBracket = text.find('[', keyPos);
    if (openBracket == std::string::npos) return {};
    const std::size_t closeBracket = FindMatching(text, openBracket, '[', ']');
    if (closeBracket == std::string::npos) return {};
    return text.substr(openBracket, closeBracket - openBracket + 1);
}

static std::vector<float> ParseNumbers(const std::string& text) {
    std::vector<float> out;
    std::string current;
    auto flush = [&]() {
        if (!current.empty()) {
            out.push_back(std::stof(current));
            current.clear();
        }
    };
    for (char ch : text) {
        if (std::isdigit(static_cast<unsigned char>(ch)) || ch == '-' || ch == '+' || ch == '.') {
            current.push_back(ch);
        } else {
            flush();
        }
    }
    flush();
    return out;
}

static Vector3 ParseVector3Text(const std::string& text, const Vector3& fallback = {0, 0, 0}) {
    const auto nums = ParseNumbers(text);
    if (nums.size() < 3) return fallback;
    return Vector3{nums[0], nums[1], nums[2]};
}

static Color ParseColorText(const std::string& text, const Color& fallback = Color{200, 200, 200, 255}) {
    const auto nums = ParseNumbers(text);
    if (nums.size() < 3) return fallback;
    return Color{
        static_cast<unsigned char>(nums[0]),
        static_cast<unsigned char>(nums[1]),
        static_cast<unsigned char>(nums[2]),
        static_cast<unsigned char>(nums.size() >= 4 ? nums[3] : 255)};
}

static std::vector<std::string> SplitTopLevelObjects(const std::string& arrayText) {
    std::vector<std::string> out;
    if (arrayText.empty() || arrayText.front() != '[') return out;
    std::size_t cursor = 1;
    while (cursor < arrayText.size()) {
        const std::size_t openBrace = arrayText.find('{', cursor);
        if (openBrace == std::string::npos) break;
        const std::size_t closeBrace = FindMatching(arrayText, openBrace, '{', '}');
        if (closeBrace == std::string::npos) break;
        out.push_back(arrayText.substr(openBrace, closeBrace - openBrace + 1));
        cursor = closeBrace + 1;
    }
    return out;
}

static std::vector<std::string> SplitTopLevelVectors(const std::string& arrayText) {
    std::vector<std::string> out;
    if (arrayText.empty() || arrayText.front() != '[') return out;
    std::size_t cursor = 1;
    while (cursor < arrayText.size()) {
        const std::size_t openBracket = arrayText.find('[', cursor);
        if (openBracket == std::string::npos) break;
        const std::size_t closeBracket = FindMatching(arrayText, openBracket, '[', ']');
        if (closeBracket == std::string::npos) break;
        out.push_back(arrayText.substr(openBracket, closeBracket - openBracket + 1));
        cursor = closeBracket + 1;
    }
    return out;
}

static bool LoadJsonLayoutFile(const fs::path& path, RegionLayout& layout) {
    const std::string text = ReadTextFile(path);
    if (text.empty()) return false;

    layout.regionId = ExtractString(text, "id");
    if (layout.regionId.empty()) return false;

    const std::string layoutObject = ExtractObject(text, "layout");
    const std::string source = layoutObject.empty() ? text : layoutObject;

    const std::string playerSpawn = ExtractArray(source, "playerSpawn");
    if (!playerSpawn.empty()) layout.playerSpawn = ParseVector3Text(playerSpawn, {0, 1, 0});

    const std::string propsArray = ExtractArray(source, "props");
    for (const auto& objectText : SplitTopLevelObjects(propsArray)) {
        LayoutPrimitive primitive{};
        const std::string type = ExtractString(objectText, "type");
        primitive.type = type == "cylinder" ? LayoutPrimitiveType::Cylinder : LayoutPrimitiveType::Box;
        primitive.position = ParseVector3Text(ExtractArray(objectText, "position"), {0, 0, 0});
        primitive.size = ParseVector3Text(ExtractArray(objectText, "size"), {1, 1, 1});
        primitive.color = ParseColorText(ExtractArray(objectText, "color"), primitive.color);
        layout.props.push_back(primitive);
    }

    const std::string npcArray = ExtractArray(source, "npcSpawns");
    for (const auto& vecText : SplitTopLevelVectors(npcArray)) layout.npcSpawns.push_back(ParseVector3Text(vecText, {0, 1, 0}));

    const std::string gatherArray = ExtractArray(source, "gatherPoints");
    for (const auto& vecText : SplitTopLevelVectors(gatherArray)) layout.gatherPoints.push_back(ParseVector3Text(vecText, {0, 1, 0}));

    const std::string travelArray = ExtractArray(source, "travelPoints");
    for (const auto& vecText : SplitTopLevelVectors(travelArray)) layout.travelPoints.push_back(ParseVector3Text(vecText, {0, 1, 0}));

    const std::string enemyArray = ExtractArray(source, "enemySpawns");
    for (const auto& objectText : SplitTopLevelObjects(enemyArray)) {
        EnemySpawn spawn{};
        spawn.position = ParseVector3Text(ExtractArray(objectText, "position"), {0, 1, 0});
        spawn.type = ExtractString(objectText, "type");
        if (!spawn.type.empty()) layout.enemySpawns.push_back(spawn);
    }

    return true;
}

} // namespace

void RegionLayoutDatabase::LoadDefaults() {
    m_layouts.clear();

    RegionLayout village{};
    village.regionId = "village_region";
    village.playerSpawn = {0, 1, 0};
    village.props.push_back({LayoutPrimitiveType::Box, {0, 0.5f, 0}, {50, 1, 50}, MakeColor(95, 125, 85)});
    village.props.push_back({LayoutPrimitiveType::Box, {-8, 2, -6}, {6, 4, 6}, MakeColor(150, 110, 70)});
    village.props.push_back({LayoutPrimitiveType::Box, {10, 2, 4}, {5, 4, 5}, MakeColor(140, 100, 60)});
    village.props.push_back({LayoutPrimitiveType::Cylinder, {4, 2, -10}, {1.5f, 4, 1.5f}, MakeColor(90, 150, 80)});
    village.npcSpawns = {{2, 1, -2}, {-5, 1, 3}};
    village.gatherPoints = {{8, 1, 8}};
    village.travelPoints = {{20, 1, 0}, {-20, 1, 0}};
    m_layouts[village.regionId] = village;

    RegionLayout forest{};
    forest.regionId = "forest_region";
    forest.playerSpawn = {0, 1, 0};
    forest.props.push_back({LayoutPrimitiveType::Box, {0, 0.5f, 0}, {70, 1, 70}, MakeColor(55, 105, 60)});
    forest.props.push_back({LayoutPrimitiveType::Cylinder, {-10, 2, -10}, {1.2f, 4, 1.2f}, MakeColor(80, 130, 70)});
    forest.props.push_back({LayoutPrimitiveType::Cylinder, {6, 2, -14}, {1.6f, 4, 1.6f}, MakeColor(75, 140, 75)});
    forest.props.push_back({LayoutPrimitiveType::Cylinder, {14, 2, 8}, {1.3f, 4, 1.3f}, MakeColor(70, 135, 70)});
    forest.props.push_back({LayoutPrimitiveType::Box, {-4, 0.6f, 12}, {4, 1.2f, 3}, MakeColor(120, 90, 60)});
    forest.npcSpawns = {{-6, 1, 4}};
    forest.gatherPoints = {{3, 1, 6}, {10, 1, -2}, {-12, 1, -4}};
    forest.travelPoints = {{-25, 1, 0}, {25, 1, 0}};
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
    ruins.gatherPoints = {{2, 1, -6}};
    ruins.travelPoints = {{0, 1, -22}};
    ruins.enemySpawns.push_back({{5, 1, 6}, "brute"});
    ruins.enemySpawns.push_back({{-8, 1, -10}, "raider"});
    m_layouts[ruins.regionId] = ruins;
}

bool RegionLayoutDatabase::LoadFromDirectory(const std::string& dir) {
    m_layouts.clear();
    if (!fs::exists(dir)) return false;

    bool loadedAny = false;
    for (const auto& entry : fs::recursive_directory_iterator(dir)) {
        if (!entry.is_regular_file()) continue;
        if (entry.path().extension() != ".json") continue;
        if (entry.path().parent_path().filename() != "regions") continue;

        RegionLayout layout{};
        if (!LoadJsonLayoutFile(entry.path(), layout) || layout.regionId.empty()) continue;

        m_layouts[layout.regionId] = std::move(layout);
        loadedAny = true;
    }
    return loadedAny;
}

void RegionLayoutDatabase::LoadDefaultsIfEmpty() {
    if (m_layouts.empty()) LoadDefaults();
}

const RegionLayout* RegionLayoutDatabase::Find(const std::string& regionId) const {
    auto it = m_layouts.find(regionId);
    return it == m_layouts.end() ? nullptr : &it->second;
}

} // namespace fw
