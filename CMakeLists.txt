cmake_minimum_required(VERSION 3.21)
project(FireWizard3DEngine VERSION 0.2.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(FW_BUILD_TESTS "Build tests" OFF)

include(FetchContent)

FetchContent_Declare(
  raylib
  GIT_REPOSITORY https://github.com/raysan5/raylib.git
  GIT_TAG 5.5
)
FetchContent_MakeAvailable(raylib)

set(FW_SOURCES
  src/main.cpp
  src/core/Application.cpp
  src/core/EngineConfig.cpp
  src/core/Time.cpp
  src/core/Input.cpp
  src/core/Logger.cpp
  src/core/FileSystem.cpp
  src/assets/AssetManager.cpp
  src/ecs/World.cpp
  src/render/Renderer.cpp
  src/render/DebugOverlay.cpp
  src/scene/SceneManager.cpp
  src/scene/SandboxScene.cpp
  src/util/StringUtil.cpp
)

add_executable(3D_GAME_ENGINE ${FW_SOURCES})
target_include_directories(3D_GAME_ENGINE PRIVATE src)
target_link_libraries(3D_GAME_ENGINE PRIVATE raylib)

if (WIN32)
  target_link_libraries(3D_GAME_ENGINE PRIVATE winmm)
endif()

add_custom_command(TARGET 3D_GAME_ENGINE POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory
          ${CMAKE_SOURCE_DIR}/assets
          $<TARGET_FILE_DIR:3D_GAME_ENGINE>/assets)
