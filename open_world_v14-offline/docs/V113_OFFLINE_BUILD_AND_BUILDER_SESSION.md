# V113 Offline Build Bootstrap and Builder Session Persistence

## What changed
- `CMakeLists.txt` now tries an installed `raylib` package before falling back to `third_party/raylib`.
- Offline-only mode now fails with a direct and actionable error message instead of dropping straight into an opaque clone failure path.
- `GameBuilderPanel` now saves `visible`, `active_tab`, and `status` to `assets/saves/builder_session.cfg`.
- `Application` restores builder session state during initialization and saves it again during shutdown.
- Version metadata now reports V113 instead of stale V103 text.

## Why this milestone matters
V112 still depended on a fragile dependency bootstrap when the local machine did not already have a vendored raylib tree. V113 makes the next local compile step more predictable and also improves day-to-day builder workflow by restoring the prior builder state.

## Local verification checklist
1. Clean configure the repo.
2. Confirm CMake now reports either installed raylib, vendored raylib, or a clear offline-only error.
3. Launch the executable.
4. Open the builder, switch tabs, close the app, reopen it.
5. Confirm the builder visibility/tab state restores from `assets/saves/builder_session.cfg`.
