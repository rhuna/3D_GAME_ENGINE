cmake_minimum_required(VERSION 3.21)
project(3D_GAME_ENGINE VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(ENGINE_TARGET 3D_GAME_ENGINE)
option(ENGINE_BUILD_TESTS "Build tests" OFF)

include(FetchContent)

FetchContent_Declare(
  raylib
  GIT_REPOSITORY https://github.com/raysan5/raylib.git
  GIT_TAG 5.5
)
FetchContent_MakeAvailable(raylib)

set(ENGINE_SOURCES
  src/main.cpp
  src/core/Application.cpp
  src/core/EngineConfig.cpp
  src/core/Time.cpp
  src/core/Input.cpp
  src/core/Logger.cpp
  src/core/FileSystem.cpp
  src/assets/AssetManager.cpp
  src/ecs/World.cpp
  src/ecs/components/EditorMetadataComponent.cpp
  src/editor/runtime/EditorSelection.cpp
  src/editor/runtime/EditorPicking.cpp
  src/editor/runtime/EditorGizmo.cpp
  src/editor/runtime/EditorSceneAuthoring.cpp
  src/editor/runtime/InspectorPanel.cpp
  src/editor/serialization/SceneExporter.cpp
  src/editor/validation/ContentValidator.cpp
  src/ecs/systems/CollisionSystem.cpp
  src/ecs/systems/MovementSystem.cpp
  src/gameplay/GameplayTags.cpp
  src/gameplay/prefabs/PrefabDefinition.cpp
  src/gameplay/prefabs/PrefabVariantDefinition.cpp
  src/gameplay/prefabs/PrefabLibrary.cpp
  src/gameplay/prefabs/SpawnFactory.cpp
  src/gameplay/registry/SystemRegistry.cpp
  src/gameplay/systems/ProjectileCleanupSystem.cpp

  src/game/rules/WaveDirector.cpp
  src/game/progression/CampaignProfile.cpp
  src/game/audio/AudioBus.cpp
  src/game/ui/MainMenu.cpp
  src/game/ui/ArenaSelectMenu.cpp
  src/game/ui/EndScreen.cpp
  src/game/ui/GameHud.cpp
  src/game/systems/PickupSystem.cpp
  src/game/systems/PowerupSystem.cpp
  src/game/systems/CombatFeedbackSystem.cpp
  src/game/systems/RangedEnemyAISystem.cpp
  src/game/systems/PlayerControllerSystem.cpp
  src/game/systems/EnemyAISystem.cpp
  src/game/systems/DamageSystem.cpp
  src/game/scenes/ArenaGameScene.cpp
  src/serialization/WorldSerializer.cpp
  src/render/Renderer.cpp
  src/render/DebugOverlay.cpp
  src/scene/SceneManager.cpp
  src/scene/data/SceneDefinition.cpp
  src/scene/data/SceneLibrary.cpp
  src/scene/spawning/SceneSpawner.cpp
  src/scene/scenes/SandboxScene.cpp
  src/util/StringUtil.cpp
)

add_executable(${ENGINE_TARGET} ${ENGINE_SOURCES})

target_include_directories(${ENGINE_TARGET} PRIVATE src)
target_link_libraries(${ENGINE_TARGET} PRIVATE raylib)

if (MSVC)
  target_compile_options(${ENGINE_TARGET} PRIVATE /W4 /permissive-)
else()
  target_compile_options(${ENGINE_TARGET} PRIVATE -Wall -Wextra -Wpedantic)
endif()

if (WIN32)
  target_link_libraries(${ENGINE_TARGET} PRIVATE winmm)
endif()

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${ENGINE_TARGET})

set_target_properties(${ENGINE_TARGET} PROPERTIES
  OUTPUT_NAME "3D_GAME_ENGINE"
  RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
  RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}/Debug"
  RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/Release"
)

add_custom_command(TARGET ${ENGINE_TARGET} POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory
          ${CMAKE_SOURCE_DIR}/assets
          $<TARGET_FILE_DIR:${ENGINE_TARGET}>/assets)
