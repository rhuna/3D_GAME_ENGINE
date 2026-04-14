#include "core/Input.h"

#include "raylib.h"

namespace fw {

namespace {
constexpr int kTrackedKeys[] = {
    KEY_W, KEY_A, KEY_S, KEY_D,
    KEY_Q, KEY_E, KEY_R, KEY_C, KEY_M,
    KEY_LEFT_SHIFT, KEY_RIGHT_SHIFT,
    KEY_LEFT_CONTROL, KEY_RIGHT_CONTROL,
    KEY_LEFT_ALT, KEY_RIGHT_ALT,
    KEY_SPACE,
    KEY_ESCAPE,
    KEY_F1, KEY_F2, KEY_F3, KEY_F4, KEY_F5, KEY_F6, KEY_F7, KEY_F8, KEY_F9, KEY_F10, KEY_F11,
    KEY_TAB,
    KEY_ONE, KEY_TWO, KEY_THREE,
    KEY_J, KEY_K, KEY_L, KEY_I, KEY_U, KEY_O,
    KEY_LEFT_BRACKET, KEY_RIGHT_BRACKET,
    KEY_N, KEY_INSERT,
    KEY_DELETE, KEY_BACKSPACE,
    KEY_ENTER, KEY_P, KEY_S
};

constexpr int kTrackedMouseButtons[] = {
    MOUSE_BUTTON_LEFT,
    MOUSE_BUTTON_RIGHT,
    MOUSE_BUTTON_MIDDLE
};
} // namespace

void Input::BeginFrame() {
    m_keysPressed.clear();
    m_keysReleased.clear();
    m_mousePressed.clear();
    m_mouseReleased.clear();

    for (int key : kTrackedKeys) {
        if (::IsKeyPressed(key)) {
            m_keysPressed.insert(key);
            m_keysDown.insert(key);
        }
        if (::IsKeyReleased(key)) {
            m_keysReleased.insert(key);
            m_keysDown.erase(key);
        }
        if (::IsKeyDown(key)) {
            m_keysDown.insert(key);
        }
    }

    for (int button : kTrackedMouseButtons) {
        if (::IsMouseButtonPressed(button)) {
            m_mousePressed.insert(button);
            m_mouseDown.insert(button);
        }
        if (::IsMouseButtonReleased(button)) {
            m_mouseReleased.insert(button);
            m_mouseDown.erase(button);
        }
        if (::IsMouseButtonDown(button)) {
            m_mouseDown.insert(button);
        }
    }
}

bool Input::IsKeyDown(int key) const { return m_keysDown.contains(key); }
bool Input::IsKeyPressed(int key) const { return m_keysPressed.contains(key); }
bool Input::IsKeyReleased(int key) const { return m_keysReleased.contains(key); }
bool Input::IsMouseButtonDown(int button) const { return m_mouseDown.contains(button); }
bool Input::IsMouseButtonPressed(int button) const { return m_mousePressed.contains(button); }
bool Input::IsMouseButtonReleased(int button) const { return m_mouseReleased.contains(button); }

} // namespace fw
