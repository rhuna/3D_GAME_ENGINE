# V129 Notes

## Milestone
Builder-to-playable-area placement integration.

## What changed
- Added a reusable placement section to the builder with extensive explanatory comments.
- Added target scene / region fields plus transform entry fields.
- Added buttons that append spawn entries into the chosen scene file so authored content can be tested in the current playable area.
- Added Create-tab placement buttons for prefabs and NPC shells.
- Added Fight-tab placement button for enemy spawns.

## Why this matters
Previous builder versions could author data, but there was no direct bridge from authored data into the active playable scene. V129 adds that bridge by writing scene-file spawn entries in the format the existing scene loader already understands.

## Expected workflow
1. Author content in the builder.
2. Choose a target scene in the placement section.
3. Enter spawn position / rotation / scale.
4. Click an Add ... To Scene button.
5. Run the playable area and verify the placed content appears.
