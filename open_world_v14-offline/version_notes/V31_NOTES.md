# V31 - Spawn and interaction stabilization

This snapshot builds on the v30 patched base and focuses on stabilizing the runtime merge.

Included in this snapshot:
- interaction pickup path updated so inventory string items no longer push a structured initializer into `std::vector<std::string>`
- intended focus: unify spawn/prefab compatibility and clear remaining compile blockers around legacy prefab calls

Recommended next steps:
1. Finish clearing `SpawnFactory` legacy compile paths.
2. Keep only one real spawn implementation.
3. Use this as the baseline for inventory data cleanup in v32.

Note: this package is a source snapshot and was not end-to-end compile-verified in the container.
