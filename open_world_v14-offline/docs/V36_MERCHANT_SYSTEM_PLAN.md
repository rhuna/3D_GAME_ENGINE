# v36 Merchant / Shop System Plan

## Goal
Add the first merchant gameplay loop on top of the v35 enterable-building scaffold.

## Added scaffolding
- MerchantComponent
- MerchantInventory / MerchantStockEntry
- ShopRuntimeState
- MerchantSystem
- ShopWindow
- herbalist_shopkeeper prefab example

## Intended next wiring
1. Attach MerchantComponent to merchant NPC entities.
2. Open ShopWindow when interacting with a merchant.
3. Read stock from prefab/content definitions.
4. Buy/sell items against player inventory and currency.
5. Persist limited stock if desired in later versions.
