cmake_minimum_required(VERSION 3.20)
project(3D_GAME_ENGINE LANGUAGES C CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
  raylib
  GIT_REPOSITORY https://github.com/raysan5/raylib.git
  GIT_TAG 5.5
)
FetchContent_MakeAvailable(raylib)

add_executable(3D_GAME_ENGINE
  src/main.cpp
  src/game/scenes/OpenWorldFoundationScene.cpp
  src/game/ui/OpenWorldHud.cpp
  src/game/ui/DialogueBox.cpp
  src/game/ui/QuestLogPanel.cpp
  src/game/ui/EquipmentPanel.cpp
  src/game/save/SaveGameProfile.cpp
  src/game/quests/QuestState.cpp
  src/game/content/QuestDatabase.cpp
  src/game/content/DialogueDatabase.cpp
  src/game/content/ItemDatabase.cpp
  src/game/content/NpcDatabase.cpp
  src/game/content/EncounterDatabase.cpp
  src/game/content/LootTableDatabase.cpp
  src/game/content/RegionDatabase.cpp
  src/game/content/ContentPipeline.cpp
  src/game/content/validation/ContentValidator.cpp
  src/game/simulation/WorldClock.cpp
  src/game/simulation/RoutineDatabase.cpp
  src/game/simulation/NpcRoutineSystem.cpp
  src/game/world/RegionSimulationState.cpp
  src/game/world/RegionLayoutDatabase.cpp
  src/game/render/WorldRenderer.cpp
  src/game/gameplay/InteractionSystem.cpp
  src/game/gameplay/QuestGameplaySystem.cpp
  src/game/gameplay/GatheringSystem.cpp
  src/game/combat/EnemySystem.cpp
  src/game/combat/CombatSystem.cpp
  src/game/progression/EquipmentSystem.cpp
  src/game/progression/StatSystem.cpp
  src/game/progression/ProgressionSystem.cpp
  src/game/loot/LootSystem.cpp
)

target_include_directories(3D_GAME_ENGINE PRIVATE src)
target_link_libraries(3D_GAME_ENGINE PRIVATE raylib)
set_target_properties(3D_GAME_ENGINE PROPERTIES OUTPUT_NAME "3D_GAME_ENGINE")
