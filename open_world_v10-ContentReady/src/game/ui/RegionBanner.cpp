#include "game/ui/RegionBanner.h"

#include "raylib.h"

namespace fw {

void RegionBanner::Draw(const std::string& label, float alpha) const {
    if (label.empty()) return;
    const int width = MeasureText(label.c_str(), 26) + 40;
    const int x = (GetScreenWidth() - width) / 2;
    DrawRectangle(x, 20, width, 42, Fade(BLACK, 0.55f * alpha));
    DrawRectangleLines(x, 20, width, 42, Fade(RAYWHITE, alpha));
    DrawText(label.c_str(), x + 20, 30, 26, Fade(RAYWHITE, alpha));
}

} // namespace fw
