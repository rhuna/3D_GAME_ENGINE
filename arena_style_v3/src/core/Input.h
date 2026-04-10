#pragma once

#include <unordered_set>

namespace fw {

class Input {
public:
    void BeginFrame();

    [[nodiscard]] bool IsKeyDown(int key) const;
    [[nodiscard]] bool IsKeyPressed(int key) const;
    [[nodiscard]] bool IsKeyReleased(int key) const;
    [[nodiscard]] bool IsMouseButtonDown(int button) const;
    [[nodiscard]] bool IsMouseButtonPressed(int button) const;
    [[nodiscard]] bool IsMouseButtonReleased(int button) const;

private:
    std::unordered_set<int> m_keysDown;
    std::unordered_set<int> m_keysPressed;
    std::unordered_set<int> m_keysReleased;
    std::unordered_set<int> m_mouseDown;
    std::unordered_set<int> m_mousePressed;
    std::unordered_set<int> m_mouseReleased;
};

} // namespace fw
