cmake_minimum_required(VERSION 3.10)
project(QuakeGame C)

# Set C standard
set(CMAKE_C_STANDARD 99)

# Find required packages
find_package(OpenGL REQUIRED)

# Define source files
set(SOURCES
    src/main.c
    src/game.c
    src/physics.c
    src/renderer.c
    src/old_school_renderer.c
    src/modern_renderer.c
)

# Define header files for IDEs
set(HEADERS
    include/game.h
    include/physics.h
    include/renderer.h
    include/old_school_renderer.h
    include/modern_renderer.h
)

# Create executable
add_executable(quake_game ${SOURCES})

# Set include directories
target_include_directories(quake_game PRIVATE include)

# Link libraries (math library is needed for math functions)
target_link_libraries(quake_game m)

# Compiler flags
target_compile_options(quake_game PRIVATE -Wall -Wextra -O2)

# Copy shaders to build directory
file(COPY shaders/ DESTINATION ${CMAKE_BINARY_DIR}/shaders)

# Install target
install(TARGETS quake_game DESTINATION bin)