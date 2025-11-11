cmake_minimum_required(VERSION 3.10)
project(BridgeBuilder)

set(CMAKE_C_STANDARD 11)

# Find required packages
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(GLEW REQUIRED)

# Include directories
include_directories(include)

# Define source files
set(SOURCES
    src/main.c
    src/physics.c
    src/renderer.c
    src/input.c
    src/bridge_builder.c
    src/shader_loader.c
)

# Define header files
set(HEADERS
    include/physics.h
    include/renderer.h
    include/input.h
    include/bridge_builder.h
    include/utils.h
)

# Create executable
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

# Link libraries
target_link_libraries(${PROJECT_NAME} 
    OpenGL::GL 
    glfw 
    GLEW::GLEW
)

# Add compile definitions for shader renderer
option(USE_SHADER_RENDERER "Enable shader-based renderer" ON)
if(USE_SHADER_RENDERER)
    target_compile_definitions(${PROJECT_NAME} PRIVATE USE_SHADER_RENDERER)
endif()

# Compiler-specific options
if(CMAKE_COMPILER_IS_GNUCC OR CMAKE_C_COMPILER_ID MATCHES "Clang")
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -O2)
endif()

# Define binutils path and add custom assembly linking if needed
find_program(BINUTILS_OBJCOPY objcopy)
find_program(BINUTILS_STRIP strip)

if(BINUTILS_OBJCOPY)
    message(STATUS "Found objcopy: ${BINUTILS_OBJCOPY}")
endif()

if(BINUTILS_STRIP)
    message(STATUS "Found strip: ${BINUTILS_STRIP}")
endif()

# Optional: Add custom target for binutils operations
add_custom_target(strip_binary
    COMMAND ${BINUTILS_STRIP} --strip-unneeded $<TARGET_FILE:${PROJECT_NAME}>
    DEPENDS ${PROJECT_NAME}
    COMMENT "Stripping symbols from executable"
)