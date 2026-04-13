# 3D Game Engine – Content-Ready RPG Framework

## Overview

This project is a content-driven open-world RPG framework built in C++ using an ECS architecture. It supports regions, quests, NPCs, crafting, combat, merchants, and persistent world state.

The framework is designed to:

* Build gameplay primarily through **data and content**, not engine rewrites
* Scale from a small starter zone to a full open world
* Provide a reusable base for multiple game types

---

## Current Status

* ✅ Boilerplate Complete (v52)
* ✅ Content Pipeline Established (v53+)
* ✅ Playable Vertical Slice (Starter Vale)
* 🔄 Actively building game content

---

## Core Gameplay Loop

1. Explore region
2. Interact with NPCs
3. Gather resources
4. Craft items
5. Complete quests
6. Fight enemies
7. Earn rewards
8. Unlock new regions

---

## Project Structure

```
src/
  core/
  ecs/
  render/
  scene/
  gameplay/
  game/
    systems/
    world/
    ui/
    quests/
    dialogue/
    progression/

assets/
  content/
    packs/
      starter_vale/
        npcs/
        quests/
        dialogue/
        encounters/
        loot/
        rewards/
        regions/
        merchants/
        crafting/
        progression/
        savepoints/
        slices/
```

---

## How to Build

```
cmake -S . -B build
cmake --build build --config Release
```

Run:

```
build/Release/3D_GAME_ENGINE.exe
```

---

## How to Add Content

1. Create or update region file
2. Add NPC definitions
3. Add items/resources
4. Add quests and dialogue
5. Add encounters or merchants
6. Test gameplay loop
7. Verify persistence

👉 See `/docs/extensions/` for detailed guides.

---

## Starter Content: Starter Vale

Starter Vale is the first playable region and demonstrates:

* NPC interaction
* Gathering and crafting
* Quest progression
* Combat encounter
* Merchant system
* Save/load persistence
* Region unlocking

---

## Development Philosophy

* Prefer **data-driven content** over hardcoded logic
* Build **small complete loops**, not huge incomplete systems
* Test content end-to-end before expanding
* Avoid modifying core engine for simple content

---

## Next Steps

* Expand Starter Vale into multi-region gameplay
* Add new quests and encounters
* Improve UI/UX and feedback
* Replace placeholder content
* Balance progression

---

## Goal

Turn this framework into a fully playable RPG by building content on top of a stable base.

---
