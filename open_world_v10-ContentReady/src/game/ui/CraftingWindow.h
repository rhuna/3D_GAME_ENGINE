#pragma once

namespace fw {

class CraftingWindow {
public:
    void Draw();
    void SetOpen(bool open) { m_open = open; }
    bool IsOpen() const { return m_open; }

private:
    bool m_open = false;
};

} // namespace fw
