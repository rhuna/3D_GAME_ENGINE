# V92 Local Compile and Runtime Verification Plan

Use this snapshot as the baseline for a real local verification pass.

## Build verification
1. Remove the old build folder.
2. Configure with your preferred generator.
3. Verify the project resolves raylib either from `third_party/raylib` or the configured fallback path.
4. Build in Debug.
5. Build in Release.
6. Fix any compile or link warnings that indicate type or path drift.

## Runtime verification
1. Launch the editor/runtime build.
2. Open the builder with the assigned hotkey.
3. Verify registry scanning sees templates, projects, ambience, music, and legacy content folders.
4. Create or load one playable slice.
5. Verify dialogue, quest, encounter, economy, interior, startup, and release assets are visible.
6. Export one staged bundle and confirm output folders are created correctly.

## Why this matters
V92 is intended to be the strongest documented baseline before the next production-oriented milestone.
