# V78 Quest Graph Suite

## Summary
V78 expands the low-code creation layer by adding quest graph authoring on top of the existing dialogue and world workflow. The builder can now create readable quest files, quest graph assets, and quest bundles that link quests back to NPC and dialogue content.

## Main changes
- Added a new **Quest** tab to `GameBuilderPanel`
- Added low-code fields for objective type, objective target, reward gold, and next quest id
- Added generation for:
  - `assets/quests/<quest>.quest`
  - `assets/questgraphs/<quest>.questgraph`
  - `assets/links/<quest>_quest.links`
- Added sample authored outputs for `first_steps`

## Intent
This version reduces coding for the second major gameplay-content bottleneck after dialogue: building quest flow and progression.
