# V37 Gather / Loot / Crafting Plan

## Goal
Create the first complete world progression loop:
- gather resources in the world
- receive loot from nodes and containers
- convert materials into useful crafted items

## Scaffolding added
- `GatherNodeComponent`
- `LootDrop` and `LootTableDefinition`
- `CraftingRecipe` and `CraftingDatabase`
- `GatheringNodeSystem`
- `CraftingWindow`
- starter gather prefab
- starter loot table
- starter recipe

## Next integration steps
1. Bind gather interaction in `InteractionSystem`.
2. Feed gathered items into the inventory model.
3. Resolve loot tables through a weighted picker.
4. Hook the crafting window to merchant/interior stations.
5. Persist harvested node state through world save/load.
