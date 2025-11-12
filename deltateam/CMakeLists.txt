cmake_minimum_required(VERSION 3.10)
project(DeltaTeam)

set(CMAKE_C_STANDARD 99)

# Find required packages
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(GLEW REQUIRED)

# Include directories
include_directories(include)

# Source files
set(SOURCES
    src/main.c
    src/physics.c
    src/game_mechanics.c
)

# Create executable
add_executable(deltateam ${SOURCES})

# Link libraries
target_link_libraries(deltateam 
    OpenGL::GL 
    glfw 
    GLEW::GLEW
    m  # math library
)

# Copy shader files to build directory
file(GLOB SHADER_FILES "shaders/*.glsl")
foreach(SHADER_FILE ${SHADER_FILES})
    get_filename_component(SHADER_NAME ${SHADER_FILE} NAME)
    configure_file(${SHADER_FILE} ${CMAKE_BINARY_DIR}/shaders/${SHADER_NAME} COPYONLY)
endforeach()

# Set build type to Release by default
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# Compiler-specific options
if(CMAKE_COMPILER_IS_GNUCC OR CMAKE_C_COMPILER_ID MATCHES "Clang")
    target_compile_options(deltateam PRIVATE -Wall -Wextra)
endif()

# For Release builds
set_target_properties(deltateam PROPERTIES
    CXX_VISIBILITY_PRESET hidden
    VISIBILITY_INLINES_HIDDEN ON
)