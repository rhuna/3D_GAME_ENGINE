# V90 Real Engine Fixes

This snapshot fixes the biggest structural problems in the uploaded V90 zip.

## Fixed
- Replaced placeholder `GameBuilderPanel` with a real integrated implementation.
- Added builder integration to `Application` and toggle on `F10`.
- Added `ContentRegistry` integration to the application lifecycle.
- Updated `ContentRegistry` to scan `assets/templates`, `assets/projects`, `assets/ambience`, and `assets/music`.
- Added optional offline CMake flow using `third_party/raylib` when available.
- Corrected version metadata to report V90 consistently.

## Next milestone
- V91 Runtime Integration + Hardening
- Real compile-clean pass on Windows
- End-to-end playable slice validation
- End-to-end export validation
