# V99 Notes

## What V99 is
V99 is a full repo snapshot built from the uploaded real project tree to fix the
first actual compile blocker in `src/content/ContentRegistry.cpp`.

## What V99 fixes
- moves `ContentRegistry::SaveDiagnosticsReport` back inside `namespace fw`
- restores `SaveDiagnosticsReport` as a real `ContentRegistry` class method
- removes the MSVC cascade caused by the out-of-namespace function definition
- updates the root `CMakeLists.txt` to prefer vendored `third_party/raylib`
- keeps the root `CMakeLists.txt` as the one true build entry point

## Main changed files
- `CMakeLists.txt`
- `src/content/ContentRegistry.cpp`
- `version_notes/VERSION.txt`

## Why the previous build failed
The build log shows the first real compiler failure at
`src/content/ContentRegistry.cpp(247,6)` where MSVC reports that
`ContentRegistry` is not a class or namespace name. That happens because the
`SaveDiagnosticsReport` definition had been appended after the namespace closed,
so the compiler no longer parsed it as `fw::ContentRegistry::SaveDiagnosticsReport`.
Once that happened, every use of `m_entries`, `ContentKind`, and `CountByKind`
turned into cascade errors.
