# V34 - Region runtime split plan

This snapshot adds a starter runtime-state header and captures the intended split of the current `OpenWorldRuntimeScene`.

Planned runtime modules:
- `OpenWorldRuntimeState`
- `WorldInteractionSystem`
- `RegionTransitionSystem`
- `WorldPersistenceBridge`
- `OpenWorldUiController`

Immediate objective:
- reduce the amount of direct scene-owned gameplay logic and move it into focused systems.

This snapshot is a structural planning milestone on top of the v30 patched runtime-merge base.
