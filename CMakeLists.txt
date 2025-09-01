# --- Project Definition ---
# Sets the minimum required version of CMake and defines the project.
cmake_minimum_required(VERSION 3.10)
project(RippleEffectEngine LANGUAGES CXX)

# --- Compiler Settings ---
# Enforce the C++17 standard for modern features.
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# --- Project Structure ---
# Tell the compiler where to find all header files.
# It will look in our project's 'include' directory and also in the
# 'vendor/Logitech' directory for the SDK header.
include_directories(
    include
    vendor/Logitech 
)

# --- Source Files ---
# Create a list of all .cpp source files that need to be compiled.
set(SOURCES
    # Main Application
    src/main.cpp

    # Core Engine Modules
    src/Core/Effects/RippleEffect.cpp
    src/Core/Keyboard/Key.cpp
    src/Core/Keyboard/Keyboard.cpp
    src/Core/Lighting/LightingManager.cpp

    # Hardware Abstraction Layer Modules
    src/Hardware/Simulator.cpp
    src/Hardware/LogitechLed.cpp
)

# --- Executable Target ---
# Define the final executable to be built from our source files.
add_executable(RippleEffectEngine ${SOURCES} "include/Core/Lighting/EffectPool.h" "src/Core/Lighting/EffectPool.cpp"  )

# --- Linker Settings ---
# Tell the linker where to find the Logitech library file (.lib).
# ${CMAKE_CURRENT_SOURCE_DIR} is a CMake variable for the project's root directory.
target_link_directories(RippleEffectEngine PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/vendor/Logitech)

# Tell the linker to actually link the 64-bit Logitech library to our executable.
# The name must match the .lib file exactly (without the extension).
target_link_libraries(RippleEffectEngine PRIVATE LogitechLEDLib)

# --- Optional: Add Compiler Warnings (Good Practice) ---
# This helps catch potential bugs by enabling more thorough code checking.
if(MSVC)
    # Warnings for Microsoft Visual C++ compiler
    target_compile_options(RippleEffectEngine PRIVATE /W4)
else()
    # Warnings for GCC/Clang compilers
    target_compile_options(RippleEffectEngine PRIVATE -Wall -Wextra -Wpedantic)
endif()