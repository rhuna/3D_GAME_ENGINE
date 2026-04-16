# V92.1 Local Compile And Runtime Checklist

Use this checklist on the Windows machine that owns the real build toolchain.

## Configure
1. Delete any old `build/` directory if it is from an incompatible version.
2. Run CMake configure with the preferred generator.
3. Confirm that raylib resolves from `third_party/` when using offline mode.

## Build
1. Build Debug.
2. Build Release.
3. Capture and fix compile errors first.
4. Capture and fix link errors second.
5. Re-run until the project is clean.

## Runtime
1. Launch the editor/runtime executable.
2. Confirm the main application window opens.
3. Confirm the visual builder can be toggled.
4. Confirm the game builder can be toggled.
5. Confirm the content registry rebuilds.
6. Confirm template, profile, and export files are visible.

## Verification targets
- one playable starter slice
- one registry rebuild
- one export bundle
- one full startup/menu flow
- one persistence flow smoke test
