# V32 - Inventory and equipment data model cleanup

This snapshot introduces the intended item shape for the next inventory migration.

Added:
- `src/game/inventory/InventoryItem.h`

Target migration for the next working pass:
- replace `std::vector<std::string> items` with `std::vector<InventoryItem> items`
- update interaction, loot, equipment, and UI code to use item ids + quantities
- make equipment panels resolve item metadata from item ids

This snapshot keeps the current runtime source layout intact and adds the data model scaffold without forcing the full migration yet.
