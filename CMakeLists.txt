# Set the minimum version of CMake required
cmake_minimum_required(VERSION 3.16)

# Define the project
project(
  Breakout
  VERSION 1.0
  DESCRIPTION "Clone of the arcade game Breakout, made with C++ and SDL3"
  LANGUAGES CXX
)

# Set the C++ standard to C++20
set(CMAKE_CXX_STANDARD 20)
list(APPEND CMAKE_PREFIX_PATH "C:/Users/Utente/AppData/Local/bin/NASM")

set(
  CMAKE_RUNTIME_OUTPUT_DIRECTORY
  "${CMAKE_BINARY_DIR}/$<CONFIGURATION>"
)

set(
  CMAKE_LIBRARY_OUTPUT_DIRECTORY
  "${CMAKE_BINARY_DIR}/$<CONFIGURATION>"
)

add_executable(Breakout
  "main.cpp"
  
  "Editor/src/Button.cpp"
  "Editor/src/Blocks.cpp"
  "Editor/src/Actor.cpp"
  "Editor/src/ActorTooltip.cpp"
  "Editor/src/Level.cpp"
  
  "Engine/src/Scene.cpp"
  "Engine/ECS/src/Component.cpp"
  "Engine/ECS/src/CollisionComponent.cpp"
  "Engine/ECS/src/ImageComponent.cpp"
  "Engine/ECS/src/PhysicsComponent.cpp"
  "Engine/ECS/src/InputComponent.cpp"
  "Engine/ECS/src/Commands.cpp"
  "Breakout/BreakoutScene.h"
  "Breakout/Ball.h"
  "Breakout/src/BreakoutScene.cpp"
  "Breakout/Wall.h"
  "Breakout/src/Ball.cpp"
 "Breakout/PaddleInputComponent.h")
 
 target_compile_definitions(
	Breakout PUBLIC
	WITH_EDITOR
	CHECK_ERRORS
 )
 
 target_include_directories(
    Breakout PUBLIC ${PROJECT_SOURCE_DIR}
)

if(WIN32)
  set(MATH_LIBRARY "" CACHE STRING "" FORCE)
endif()

if(APPLE)
  set_target_properties(Breakout PROPERTIES
    INSTALL_RPATH "@executable_path;@loader_path"
    BUILD_WITH_INSTALL_RPATH TRUE
    MACOSX_RPATH TRUE
  )
elseif(UNIX)
  set_target_properties(Breakout PROPERTIES
    INSTALL_RPATH "$ORIGIN"
    BUILD_WITH_INSTALL_RPATH TRUE
  )
endif()

# Specify that we want to build SDL_TTF's dependencies
set(SDLTTF_VENDORED ON) 

# Set a variable for our vendor directory
set(VENDOR_DIR "${PROJECT_SOURCE_DIR}/vendor") 

# Add the SDL libraries from our submodules
add_subdirectory(${VENDOR_DIR}/SDL) 
add_subdirectory(${VENDOR_DIR}/SDL_image) 
add_subdirectory(${VENDOR_DIR}/SDL_ttf)

target_link_libraries(Breakout 
  SDL3::SDL3 
  SDL3_image::SDL3_image 
  SDL3_ttf::SDL3_ttf 
)

set(AssetDirectory "${PROJECT_SOURCE_DIR}/Assets")
add_custom_command(
  TARGET Breakout POST_BUILD
  COMMAND
  ${CMAKE_COMMAND} -E copy_if_different
    "$<TARGET_FILE:SDL3::SDL3>"
    "$<TARGET_FILE:SDL3_image::SDL3_image>"
    "$<TARGET_FILE:SDL3_ttf::SDL3_ttf>"
    "$<TARGET_FILE_DIR:Breakout>"

  COMMAND
  ${CMAKE_COMMAND} -E copy_directory_if_different
    "${AssetDirectory}"
    "$<TARGET_FILE_DIR:Breakout>/Assets"
  VERBATIM
)