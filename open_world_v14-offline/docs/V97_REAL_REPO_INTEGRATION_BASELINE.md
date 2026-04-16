# V97 Real Repo Integration Baseline

## Purpose
Use the uploaded repo as the source of truth and restore the real integration layer
instead of layering additional placeholder milestone files over it.

## Key corrections
1. Keep the root `CMakeLists.txt` as the build entry point
2. Restore the real `GameBuilderPanel`
3. Align `Application` with the builder and registry
4. Align `ContentRegistry` with the repo's real asset folders
5. Write registry diagnostics during startup

## Local verification goals
1. Configure from the root
2. Build Release
3. Launch the executable
4. Press F10 to open the builder
5. Confirm `assets/registry/content_registry.txt` updates
6. Confirm `assets/registry/v97_registry_diagnostics.txt` exists
