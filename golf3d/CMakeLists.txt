cmake_minimum_required(VERSION 3.10)
project(golf3d VERSION 1.0.0)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Find required packages
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(GLEW REQUIRED)

# Define source files
set(SOURCES
    src/main.c
    src/renderer.c
    src/physics.c
)

set(HEADERS
    include/game.h
    include/renderer.h
    include/physics.h
)

# Create executable
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

# Link libraries
target_link_libraries(${PROJECT_NAME} 
    OpenGL::GL 
    glfw 
    GLEW::GLEW
    m  # Math library
    GLU  # GLU library for gluPerspective, gluLookAt
    ${CMAKE_DL_LIBS}
)

# Include directories
target_include_directories(${PROJECT_NAME} PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Compiler-specific options
if(CMAKE_C_COMPILER_ID STREQUAL "GNU" OR CMAKE_C_COMPILER_ID STREQUAL "Clang")
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra)
endif()

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra)
endif()

# Copy shader files to build directory
file(COPY shaders/ DESTINATION ${CMAKE_BINARY_DIR}/shaders)