# V57 Notes

This version starts the large-world and high-detail scaling branch.

## Added
- LOD component and level definitions
- Visibility component and culling system scaffold
- Chunk model and streaming system scaffold
- Material budget profile definitions
- Animation rig + clip state + animation system scaffold
- Streaming debug overlay and asset budget audit tooling
- Large world scalability manifest
- Starter Vale chunk sample and material budget sample

## Recommended next work
- integrate LOD selection into the world renderer
- evaluate visibility before render submission
- connect chunk streaming to region scene loading
- add asset validation rules for material budgets
- wire animated actor prefabs into the runtime
