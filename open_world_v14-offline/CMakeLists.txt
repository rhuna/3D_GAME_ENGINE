cmake_minimum_required(VERSION 3.20)
project(3D_GAME_ENGINE LANGUAGES C CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(FW_OFFLINE_ONLY "Disable network dependency resolution for offline repo setups" OFF)

include(FetchContent)
if(FW_OFFLINE_ONLY)
  add_subdirectory(third_party/raylib)
else()
  FetchContent_Declare(
    raylib
    GIT_REPOSITORY https://github.com/raysan5/raylib.git
    GIT_TAG 5.5
  )
  FetchContent_MakeAvailable(raylib)
endif()

file(GLOB_RECURSE PROJECT_SOURCES CONFIGURE_DEPENDS
  "src/*.cpp"
)

# Exclude legacy sources from the pre-ECS runtime that conflict with the current engine architecture.
list(FILTER PROJECT_SOURCES EXCLUDE REGEX ".*/src/scene/Scene\.cpp$")
list(FILTER PROJECT_SOURCES EXCLUDE REGEX ".*/src/scene/SandboxScene\.cpp$")
list(FILTER PROJECT_SOURCES EXCLUDE REGEX ".*/src/render/DebugDraw\.cpp$")
list(FILTER PROJECT_SOURCES EXCLUDE REGEX ".*/src/game/scenes/OpenWorldFoundationScene\.cpp$")
list(FILTER PROJECT_SOURCES EXCLUDE REGEX ".*/src/game/content/validation/ContentValidator\.cpp$")
list(FILTER PROJECT_SOURCES EXCLUDE REGEX ".*/src/gameplay/WorldSerializer\.cpp$")
list(FILTER PROJECT_SOURCES EXCLUDE REGEX ".*/src/gameplay/systems/MovementSystem\.cpp$")
list(FILTER PROJECT_SOURCES EXCLUDE REGEX ".*/src/gameplay/systems/CollisionSystem\.cpp$")
list(FILTER PROJECT_SOURCES EXCLUDE REGEX ".*/src/gameplay/prefabs/SpawnFactory\.cpp$")
list(REMOVE_ITEM PROJECT_SOURCES "${CMAKE_SOURCE_DIR}/src/gameplay/prefabs/SpawnFactory.cpp")

add_executable(3D_GAME_ENGINE ${PROJECT_SOURCES})
target_include_directories(3D_GAME_ENGINE PRIVATE src)
target_link_libraries(3D_GAME_ENGINE PRIVATE raylib)
set_target_properties(3D_GAME_ENGINE PROPERTIES OUTPUT_NAME "3D_GAME_ENGINE")
