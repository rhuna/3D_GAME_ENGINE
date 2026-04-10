#include "render/DebugOverlay.h"

#include <algorithm>
#include <cstdio>
#include <string>
#include <vector>

namespace fw {
namespace {

constexpr int kMargin = 12;
constexpr int kPanelGap = 10;
constexpr int kTitleFont = 20;
constexpr int kTextFont = 18;
constexpr int kLineHeight = 22;

void DrawPanel(int x, int y, int width, int height, const char* title) {
    DrawRectangle(x, y, width, height, Color{0, 0, 0, 170});
    DrawRectangleLines(x, y, width, height, Fade(SKYBLUE, 0.65f));
    DrawRectangle(x, y, width, 28, Color{20, 30, 45, 220});
    DrawText(title, x + 10, y + 5, kTitleFont, RAYWHITE);
}

void DrawLinesInPanel(int x, int y, int width, const std::vector<std::string>& lines, Color color = RAYWHITE) {
    int drawY = y + 34;
    for (const std::string& line : lines) {
        DrawText(line.c_str(), x + 10, drawY, kTextFont, color);
        drawY += kLineHeight;
    }
    (void)width;
}

std::string FitText(const std::string& text, int maxChars) {
    if (static_cast<int>(text.size()) <= maxChars) return text;
    if (maxChars <= 3) return text.substr(0, std::max(0, maxChars));
    return text.substr(0, maxChars - 3) + "...";
}

Color ValidationColor(const ValidationMessage& message) {
    return message.severity == "error" || message.severity == "ERROR" ? ORANGE : LIGHTGRAY;
}

} // namespace

void DebugOverlay::Draw(const Time& time, const World& world, const Camera3D& camera, const std::string& sceneName, Entity selectedEntity, const std::vector<ValidationMessage>& validationMessages, const std::string& lastExportPath, bool inspectorVisible) const {
    const int screenWidth = GetScreenWidth();
    const int screenHeight = GetScreenHeight();
    const int halfWidth = (screenWidth - (kMargin * 2) - kPanelGap) / 2;
    const int leftX = kMargin;
    const int rightX = leftX + halfWidth + kPanelGap;
    const int topY = kMargin;
    const int upperHeight = 152;
    const int lowerY = topY + upperHeight + kPanelGap;
    const int lowerHeight = 118;

    DrawPanel(leftX, topY, halfWidth, upperHeight, "Engine / Scene");
    DrawPanel(rightX, topY, halfWidth, upperHeight, "Selection / Camera");
    DrawPanel(leftX, lowerY, halfWidth, lowerHeight, "Validation / Export");
    DrawPanel(rightX, lowerY, halfWidth, lowerHeight, "Controls");

    char buffer[512];
    std::vector<std::string> engineLines;
    std::snprintf(buffer, sizeof(buffer), "Scene: %s", sceneName.c_str());
    engineLines.emplace_back(buffer);
    std::snprintf(buffer, sizeof(buffer), "FPS: %d", GetFPS());
    engineLines.emplace_back(buffer);
    std::snprintf(buffer, sizeof(buffer), "Delta: %.4f | Fixed: %.4f", time.DeltaTime(), time.FixedDeltaTime());
    engineLines.emplace_back(buffer);
    std::snprintf(buffer, sizeof(buffer), "Window: %d x %d | F11 Fullscreen", screenWidth, screenHeight);
    engineLines.emplace_back(buffer);
    std::snprintf(buffer, sizeof(buffer), "Entities: %d | Collisions: %d", static_cast<int>(world.Entities().size()), static_cast<int>(world.Collisions().size()));
    engineLines.emplace_back(buffer);
    DrawLinesInPanel(leftX, topY, halfWidth, engineLines);

    std::vector<std::string> selectionLines;
    std::snprintf(buffer, sizeof(buffer), "Selected: %u", selectedEntity);
    selectionLines.emplace_back(buffer);
    std::snprintf(buffer, sizeof(buffer), "Inspector: %s", inspectorVisible ? "visible" : "hidden");
    selectionLines.emplace_back(buffer);
    std::snprintf(buffer, sizeof(buffer), "Camera Pos: %.2f, %.2f, %.2f", camera.position.x, camera.position.y, camera.position.z);
    selectionLines.emplace_back(buffer);
    std::snprintf(buffer, sizeof(buffer), "Camera Target: %.2f, %.2f, %.2f", camera.target.x, camera.target.y, camera.target.z);
    selectionLines.emplace_back(buffer);
    selectionLines.emplace_back("RMB hold = look camera | MMB = focus selected");
    DrawLinesInPanel(rightX, topY, halfWidth, selectionLines);

    std::vector<std::string> validationLines;
    std::snprintf(buffer, sizeof(buffer), "Validation messages: %zu", validationMessages.size());
    validationLines.emplace_back(buffer);
    validationLines.emplace_back("Last export:");
    validationLines.emplace_back(FitText(lastExportPath.empty() ? std::string("<none>") : lastExportPath, std::max(18, (halfWidth - 20) / 9)));
    DrawLinesInPanel(leftX, lowerY, halfWidth, validationLines, validationMessages.empty() ? LIGHTGRAY : YELLOW);

    std::vector<std::string> controlLines = {
        "F1 Debug | Tab Inspector | F5 Reload | F8 Save Scene",
        "LMB Pick | Shift+LMB Projectile | 1/2/3 Gizmo",
        "J/L/I/K/U/O Edit | [/] Variant | N Spawn | Del Remove"
    };
    DrawLinesInPanel(rightX, lowerY, halfWidth, controlLines, LIGHTGRAY);

    if (!validationMessages.empty()) {
        const int panelHeight = std::min(180, screenHeight - (lowerY + lowerHeight + kPanelGap + kMargin));
        if (panelHeight >= 90) {
            const int panelY = screenHeight - panelHeight - kMargin;
            DrawPanel(leftX, panelY, screenWidth - (kMargin * 2), panelHeight, "Validation Messages");
            int drawY = panelY + 34;
            for (size_t i = 0; i < validationMessages.size() && drawY <= panelY + panelHeight - 24; ++i) {
                const ValidationMessage& message = validationMessages[i];
                std::snprintf(buffer, sizeof(buffer), "- [%s] %s", message.severity.c_str(), message.text.c_str());
                DrawText(FitText(buffer, std::max(20, (screenWidth - 60) / 9)).c_str(), leftX + 10, drawY, kTextFont, ValidationColor(message));
                drawY += kLineHeight;
            }
        }
    }
}

} // namespace fw
