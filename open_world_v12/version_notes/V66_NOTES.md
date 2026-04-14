# V66 Notes

## Summary
V66 stabilizes the engine after the V64/V65 drift and adds a proper grouped asset-kit workflow that fits the uploaded repo.

## Core fixes
- Restored backward-compatible `RenderComponent` fields.
- Expanded `FileSystem` with write and recursive listing helpers.
- Added primitive render parsing and drawing without removing old cube/sphere behavior.
- Added scene includes and scene kit spawning support.

## New workflow
- Build grouped primitive kits in-engine.
- Save them as reusable scene kits.
- Spawn them later with `spawn=kit=<name>;position=x,y,z`.
