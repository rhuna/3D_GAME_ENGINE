# v30 upgrade notes

This branch builds on the stabilized v29 runtime merge and adds the next open-world integration layer:

- scene-authored travel markers (`travel_marker` / `region_exit`) are converted into `TravelTriggerComponent`
- scene-authored save markers (`save_point`) are converted into `SavePointComponent`
- gather markers now receive `GatherNodeComponent`
- region-spawned ECS entities receive `PersistentIdComponent`
- `WorldStatePersistence` now tracks harvested node ids and used save point ids
- open world interactions now read travel targets from ECS trigger components instead of hardcoded region cycling
- save points perform both profile save and world-state snapshot save

This is a bridge version: it keeps the existing open-world gameplay loop intact while moving runtime world markers onto ECS components so the next step can be full entity-based persistence and scene-authored quest/NPC logic.
