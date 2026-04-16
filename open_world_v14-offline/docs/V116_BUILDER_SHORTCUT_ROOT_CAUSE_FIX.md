# V116 Builder Shortcut Root-Cause Fix

The builder had not been opening because the custom Input system never tracked KEY_F9, KEY_F10, KEY_B, or the Ctrl keys.

## Fixes
- Added KEY_B, KEY_LEFT_CONTROL, KEY_RIGHT_CONTROL, KEY_F9, KEY_F10, and KEY_F11 to the tracked key list in `src/core/Input.cpp`.
- Changed the builder toggle path in `src/core/Application.cpp` to use raw raylib `::IsKeyPressed` / `::IsKeyDown` so the builder no longer depends on the wrapper for its primary shortcut path.

## Expected behavior
- `F10` toggles builder
- `F9` toggles builder
- `Ctrl+B` toggles builder

This addresses the actual root cause instead of continuing to patch rendering around an input path that could never fire.
