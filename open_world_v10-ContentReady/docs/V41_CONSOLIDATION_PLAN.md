# V41 Consolidation / Cleanup Pass

## Goal
Turn the runtime-merge line into a cleaner reusable framework by reducing architectural drift.

## Primary consolidation targets
- One spawn path
- One interaction/inventory item shape
- One quest runtime type model
- One persistence write/read path
- One open-world runtime entry point
- Cleaner CMake source ownership and exclusion rules

## Recommended tasks
1. Move legacy compatibility wrappers into a clearly marked compatibility layer.
2. Replace lingering string-only gameplay payloads with typed runtime structs.
3. Split large scene/runtime files into state + systems + UI coordinators.
4. Standardize component construction to explicit field assignment where shapes have been changing.
5. Add smoke-test scenes and a starter vertical-slice checklist.
6. Document how to add a region, prefab, merchant, gather node, and quest.

## Done criteria
- Build graph no longer compiles dead legacy sources.
- Runtime-spawned entities all use the same spawn interface.
- No duplicated gameplay data models for inventory, quest state, or persistence.
- Version can serve as the baseline for the boilerplate-ready push.
