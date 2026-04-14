# v36 Notes

This version adds merchant and shop scaffolding on top of v35.

## Added
- src/game/components/MerchantComponent.h
- src/game/shops/MerchantInventory.h
- src/game/shops/ShopRuntimeState.h
- src/game/systems/MerchantSystem.h
- src/game/systems/MerchantSystem.cpp
- src/game/ui/ShopWindow.h
- src/game/ui/ShopWindow.cpp
- assets/prefabs/herbalist_shopkeeper.prefab
- docs/V36_MERCHANT_SYSTEM_PLAN.md

## Purpose
v36 starts the NPC shop / merchant gameplay layer without forcing a risky full runtime rewrite in one jump.
