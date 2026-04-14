# V57 Large World + High Detail Foundations

This version starts the runtime scaling branch needed for larger, more detailed games.

## Goals
1. LOD system
2. Visibility culling
3. Chunk/stream loading
4. Texture and material budgets
5. Stronger animation pipeline
6. Better tooling

## What this version is
A scaffolding milestone that defines the contracts, runtime data structures, and system entry points
for scaling the existing framework.

## What this version is not
A fully optimized shipping renderer. The systems introduced here are starter implementations and extension
points meant to guide the next production phases.

## Upgrade themes

### LOD
- Entity-level LOD groups
- Distance thresholds
- Preferred lower-cost asset path by tier

### Culling
- Camera-driven visibility tests
- Frustum and distance culling hooks
- Render submission filtering

### Chunk streaming
- Region split into chunk cells
- Chunk state tracking
- Preload/load/unload thresholds

### Texture/material discipline
- Per-material budget tagging
- Validation categories
- Asset review checklist

### Animation pipeline
- Clip state
- Rig metadata
- Runtime animation state separation

### Tooling
- Runtime diagnostics
- World streaming overlay hooks
- Asset budget visibility
