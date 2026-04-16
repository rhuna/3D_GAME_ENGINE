# V114 Export Pipeline Staging and Builder Toggle Hardening Snapshot
# This root CMake file remains the only required build entry point for the repo.
# There is intentionally no src/CMakeLists.txt in this project structure.

cmake_minimum_required(VERSION 3.20)
project(3D_GAME_ENGINE VERSION 114.0 LANGUAGES C CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# V114 keeps the offline-friendly dependency discovery so the repo behaves better on offline
# machines. The build now tries these paths in order:
#   1. system-installed raylib package config
#   2. vendored third_party/raylib checkout
#   3. FetchContent (only when offline-only mode is disabled)
option(FW_OFFLINE_ONLY "Require local raylib sources or an installed raylib package" ON)

set(FW_RAYLIB_TARGET "")
find_package(raylib CONFIG QUIET)
if (TARGET raylib)
  message(STATUS "V114: Using installed raylib package")
  set(FW_RAYLIB_TARGET raylib)
elseif (EXISTS "${CMAKE_SOURCE_DIR}/third_party/raylib/CMakeLists.txt")
  message(STATUS "V114: Using vendored third_party/raylib")
  add_subdirectory(third_party/raylib)
  set(FW_RAYLIB_TARGET raylib)
elseif (FW_OFFLINE_ONLY)
  message(FATAL_ERROR
    "V114: raylib was not found locally.\n"
    "Provide one of the following before configuring again:\n"
    "  - install raylib so find_package(raylib CONFIG) succeeds\n"
    "  - place a checkout in third_party/raylib\n"
    "  - rerun with -DFW_OFFLINE_ONLY=OFF to allow FetchContent\n")
else()
  include(FetchContent)
  message(STATUS "V114: Local raylib not found, using FetchContent")
  FetchContent_Declare(
    raylib
    GIT_REPOSITORY https://github.com/raysan5/raylib.git
    GIT_TAG 5.5
  )
  FetchContent_MakeAvailable(raylib)
  set(FW_RAYLIB_TARGET raylib)
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
target_link_libraries(3D_GAME_ENGINE PRIVATE ${FW_RAYLIB_TARGET})
set_target_properties(3D_GAME_ENGINE PROPERTIES OUTPUT_NAME "3D_GAME_ENGINE")
