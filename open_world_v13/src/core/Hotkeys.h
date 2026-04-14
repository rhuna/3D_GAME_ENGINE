#pragma once

#include "raylib.h"

namespace fw::hotkeys {

inline bool CtrlDown() {
    return ::IsKeyDown(KEY_LEFT_CONTROL) || ::IsKeyDown(KEY_RIGHT_CONTROL);
}

inline bool ShiftDown() {
    return ::IsKeyDown(KEY_LEFT_SHIFT) || ::IsKeyDown(KEY_RIGHT_SHIFT);
}

inline bool AltDown() {
    return ::IsKeyDown(KEY_LEFT_ALT) || ::IsKeyDown(KEY_RIGHT_ALT);
}

inline bool Pressed(int key) {
    return ::IsKeyPressed(key);
}

inline bool CtrlPressed(int key) {
    return CtrlDown() && ::IsKeyPressed(key);
}

inline bool CtrlShiftPressed(int key) {
    return CtrlDown() && ShiftDown() && ::IsKeyPressed(key);
}

inline bool AltPressed(int key) {
    return AltDown() && ::IsKeyPressed(key);
}

inline bool DialogueAdvancePressed() {
    return ::IsKeyPressed(KEY_ENTER);
}

inline bool DialogueClosePressed() {
    return ::IsKeyPressed(KEY_ESCAPE);
}

inline bool ToggleQuestLogPressed() {
    return ::IsKeyPressed(KEY_J);
}

inline bool ToggleEquipmentPressed() {
    return ::IsKeyPressed(KEY_C);
}

inline bool SaveProfilePressed() {
    return ::IsKeyPressed(KEY_F5);
}

inline bool LoadProfilePressed() {
    return ::IsKeyPressed(KEY_F9);
}

inline bool SaveWorldStatePressed() {
    return CtrlShiftPressed(KEY_S);
}

inline bool LoadWorldStatePressed() {
    return CtrlShiftPressed(KEY_L);
}

inline bool ResetCurrentRegionPressed() {
    return CtrlShiftPressed(KEY_BACKSPACE);
}

inline bool RestoreStarterScaffoldPressed() {
    return CtrlShiftPressed(KEY_F6);
}

inline bool DebugRegionPressed(int slotKey) {
    return CtrlPressed(slotKey);
}

inline bool ToggleDebugOverlayPressed() {
    return ::IsKeyPressed(KEY_F1);
}

inline bool SelectNextEditorEntityPressed() {
    return CtrlPressed(KEY_F2);
}

inline bool ExportScenePressed() {
    return CtrlPressed(KEY_F3);
}

inline bool RunValidationPressed() {
    return CtrlPressed(KEY_F4);
}

inline bool ReloadRuntimeScenePressed() {
    return CtrlPressed(KEY_F5);
}

inline bool SaveSnapshotPressed() {
    return CtrlPressed(KEY_F6);
}

inline bool LoadSnapshotPressed() {
    return CtrlPressed(KEY_F7);
}

inline bool ToggleInspectorPressed() {
    return ::IsKeyPressed(KEY_F10);
}

inline bool ToggleTranslateGizmoPressed() {
    return AltPressed(KEY_W);
}

inline bool ToggleRotateGizmoPressed() {
    return AltPressed(KEY_E);
}

inline bool ToggleScaleGizmoPressed() {
    return AltPressed(KEY_R);
}

} // namespace fw::hotkeys
