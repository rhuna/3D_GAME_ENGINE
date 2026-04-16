# V71 - Local Content Registry

V71 is the offline backbone upgrade.

## What V71 adds
- New `ContentRegistry` system in `src/content/`
- Project-wide scanning of local content from `assets/`
- Registry snapshot export to `assets/registry/content_registry.txt`
- New **Registry** tab in `GameBuilderPanel`
- F10 toggle for the builder panel
- Automatic registry refresh after content generation
- `.kit` metadata sidecar output when saving grouped kits from `BuildAssetWorkshop`
- Optional `FW_OFFLINE_ONLY` CMake mode for local vendored raylib via `third_party/raylib`

## Main new files
- `src/content/ContentRegistry.h`
- `src/content/ContentRegistry.cpp`

## Main modified files
- `src/core/Application.h`
- `src/core/Application.cpp`
- `src/editor/ui/GameBuilderPanel.h`
- `src/editor/ui/GameBuilderPanel.cpp`
- `src/editor/runtime/BuildAssetWorkshop.cpp`
- `CMakeLists.txt`

## Registry coverage
The registry indexes these local content buckets:
- Prefabs
- Variants
- Scenes
- Kits
- Templates
- Project files
- Models
- Textures
- Audio

## Why this matters
This is the first real project database layer for the editor. It makes the engine much easier to scale offline because later importer, thumbnail, export, and starter-world tools can all build on one local source of truth.

## Next best upgrade
- V72: Offline Import Pipeline + thumbnails + one-click asset ingest
