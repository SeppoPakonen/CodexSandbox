cmake_minimum_required(VERSION 3.10)
project(rollercoaster)

set(CMAKE_C_STANDARD 99)

# Find required packages
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(GLEW REQUIRED)

# Add executable
add_executable(rollercoaster 
    src/game.c
    src/rollercoaster_physics.c
)

# Include directories
target_include_directories(rollercoaster PRIVATE include)

# Link libraries
target_link_libraries(rollercoaster 
    OpenGL::GL 
    glfw 
    GLEW::GLEW
    m  # math library
)

# Compiler-specific options
if(CMAKE_C_COMPILER_ID STREQUAL "GNU" OR CMAKE_C_COMPILER_ID STREQUAL "Clang")
    target_compile_options(rollercoaster PRIVATE -Wall -Wextra)
elseif(CMAKE_C_COMPILER_ID STREQUAL "MSVC")
    target_compile_options(rollercoaster PRIVATE /W4)
endif()

# Binutils - for object file analysis, etc.
# Add any specific binutils related flags if needed