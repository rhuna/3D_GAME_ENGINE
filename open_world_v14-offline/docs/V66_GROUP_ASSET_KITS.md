# V66 - Group Asset Kits

V66 adds a grounded, repo-compatible in-engine asset workshop.

## Goals
- Keep the current engine buildable.
- Make grouped primitive blockouts reusable.
- Let authored groups become spawnable scene kits.

## Workflow
- Press `F9` to open the workshop.
- Press `B` to toggle primitive stamping.
- Press `1/2/3/4` for cube, sphere, cylinder, plane.
- Left click the ground to stamp pieces.
- Press `G` to add or remove the selected entity from the current group.
- Press `Ctrl+D` to duplicate the selected piece.
- Press `M` to mirror the grouped pieces across X.
- Type a kit name while the workshop is open.
- Press `Ctrl+P` to save the current group.

## Save Result
Saving writes:
- `assets/prefabs/generated/<kit>_piece_###.prefab` for generated primitive pieces
- `assets/scenes/generated/<kit>.scene` for the grouped kit itself

## Reuse
A saved kit can be used inside any scene with:

spawn=kit=my_asset_kit;position=10,0,5

## Notes
- Kit placement currently applies a position offset. Rotation and non-uniform scale are still stored per piece, but the kit root itself is position-first in V66.
- The patch keeps older `RenderComponent` fields so existing gameplay systems still compile.
